#include "Character/CharacterBase.h"
#include "AbilitySystemComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"

ACharacterBase::ACharacterBase()
{
 	PrimaryActorTick.bCanEverTick = true;

	AbilitySystemComponent = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	CharacterAttributeSet = CreateDefaultSubobject<UCharacterAttributeSet>(TEXT("CharacterAttributeSet"));
}

void ACharacterBase::BeginPlay()
{
	Super::BeginPlay();
		
	// 通常AbilityのGiveAbility
	for (auto Ability : PlayerGameplayAbilities)
	{
		AbilitySystemComponent->GiveAbility(FGameplayAbilitySpec(Ability.GetDefaultObject(), 0, -1, this));
	}

	// WaitInputのGiveAbility
	if (AbilitySystemComponent)
	{
		if (HasAuthority() && WaitInputAbilities.Num() > 0)
		{
			for (auto Ability : WaitInputAbilities)
			{
				if (Ability)
				{
					if (static_cast<int32>(Ability.GetDefaultObject()->AbilityInputType) != 0)
					{
						AbilitySystemComponent->GiveAbility(FGameplayAbilitySpec(Ability.GetDefaultObject(), 1, static_cast<int32>(Ability.GetDefaultObject()->AbilityInputType), this));
					}
					else
					{
						AbilitySystemComponent->GiveAbility(FGameplayAbilitySpec(Ability.GetDefaultObject(), 1, -1, this));
					}
				}
			}
		}
		
		AbilitySystemComponent->InitAbilityActorInfo(this, this);
	}
}

void ACharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	ApplyCharacterState();
	SetLocalVelocity();
}

void ACharacterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	BindASCInput();
}

void ACharacterBase::BindASCInput()
{
	if (!bASCInputBound && IsValid(AbilitySystemComponent) && IsValid(InputComponent))
	{
		AbilitySystemComponent->BindAbilityActivationToInputComponent(InputComponent, FGameplayAbilityInputBinds(FString("ConfirmTarget"),
			FString("CancelTarget"), FString("EAbilityInputType"), static_cast<int32>(EAbilityInputType::Confirm), static_cast<int32>(EAbilityInputType::Cancel)));

		bASCInputBound = true;
	}
}

void ACharacterBase::ApplyCharacterState()
{
	if (AbilitySystemComponent->HasMatchingGameplayTag(FGameplayTag::RequestGameplayTag(FName("Ability.State.Attack"))))
	{
		CharacterState = ECharacterState::Attack;
	}
	else if (AbilitySystemComponent->HasMatchingGameplayTag(FGameplayTag::RequestGameplayTag(FName("Ability.State.Defense"))))
	{
		CharacterState = ECharacterState::Defense;
	}
	else if (AbilitySystemComponent->HasMatchingGameplayTag(FGameplayTag::RequestGameplayTag(FName("Ability.State.Dodge"))))
	{
		CharacterState = ECharacterState::Dodge;
	}
	else
	{
		CharacterState = ECharacterState::Normal;
	}
}

bool ACharacterBase::HitAttack_Implementation(float Damage, float Impact, FVector AttackDirection)
{
	return false;
}

void ACharacterBase::SetLocalVelocity()
{
	// PlayerのForwardからのRotatorのYawを逆にしてLocalとする
	FRotator Rotator = UKismetMathLibrary::MakeRotFromXY(GetActorForwardVector(), GetActorRightVector());
	Rotator.Roll = 0;
	Rotator.Pitch = 0;
	Rotator.Yaw *= -1;
	LocalVelocity = Rotator.RotateVector(GetCharacterMovement()->Velocity);
}
