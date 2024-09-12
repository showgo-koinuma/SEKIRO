#include "Character/CharacterBase.h"
#include "AbilitySystemComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"

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
	for (auto Ability : CharacterGameplayAbilities)
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

	// Attributeのパラメーターによるアクションにバインド
	if (CharacterAttributeSet) // nullになるはず無いんだけどなんかエラーでたんだよね
	{
		CharacterAttributeSet->OnDead.AddDynamic(this, &ACharacterBase::OnDead);
		CharacterAttributeSet->OnBrokePosture.AddDynamic(this, &ACharacterBase::OnBrokePosture);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("CharacterAttribute is null"));
	}

	// ターンスピードの設定
	GetCharacterMovement()->RotationRate = FRotator(0, CharacterParameter->TurnSpeed, 0);
}

void ACharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	ApplyCharacterState();
	Movement();
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

void ACharacterBase::HitAttack_Implementation(const UCharacterAttackParam* AttackParam, FVector Direction,
	FVector HitPoint, const ACharacterBase* Attacker)
{
}

void ACharacterBase::ApplyPostureEffect_Implementation(const float AddValue) const
{
}

void ACharacterBase::ApplyCharacterState()
{
	if (CharacterState == ECharacterState::Dead) return;
	
	if (AbilitySystemComponent->HasMatchingGameplayTag(FGameplayTag::RequestGameplayTag(FName("Ability.State.Dead"))))
	{
		CharacterState = ECharacterState::Dead;
	}
	else if (AbilitySystemComponent->HasMatchingGameplayTag(FGameplayTag::RequestGameplayTag(FName("Ability.State.DuringMotion"))))
	{
		CharacterState = ECharacterState::DuringMotion;
	}
	else if (AbilitySystemComponent->HasMatchingGameplayTag(FGameplayTag::RequestGameplayTag(FName("Ability.State.BrokePosture"))))
	{
		CharacterState = ECharacterState::BrokePosture;
	}
	else if (AbilitySystemComponent->HasMatchingGameplayTag(FGameplayTag::RequestGameplayTag(FName("Ability.State.Hit"))))
	{
		CharacterState = ECharacterState::Hit;
	}
	else if (AbilitySystemComponent->HasMatchingGameplayTag(FGameplayTag::RequestGameplayTag(FName("Ability.State.Attack"))))
	{
		CharacterState = ECharacterState::Attack;
	}
	else if (AbilitySystemComponent->HasMatchingGameplayTag(FGameplayTag::RequestGameplayTag(FName("Ability.State.Dodge"))))
	{
		CharacterState = ECharacterState::Dodge;
	}
	else if (AbilitySystemComponent->HasMatchingGameplayTag(FGameplayTag::RequestGameplayTag(FName("Ability.State.Defense"))))
	{
		CharacterState = ECharacterState::Defense;
	}
	else
	{
		CharacterState = ECharacterState::Normal;
	}
}

void ACharacterBase::Movement()
{
}

void ACharacterBase::SetRotationToTarget(FVector TargetDirection, const float DeltaTime)
{
	if (!LookingAtTarget()) return;

	UE_LOG(LogTemp, Log, TEXT("SetRotationToTarget"));
	FVector BaseVector = GetActorForwardVector();
	TargetDirection.Z = 0;
	TargetDirection.Normalize();
	// 現在の角度からTargetまでのなす角
	const float Degree = FMath::RadiansToDegrees(FMath::Acos(FVector::DotProduct(BaseVector, TargetDirection)));
	// フレームでの回転量
	const float RotateAngle = CharacterParameter->TurnSpeed * DeltaTime;

	// フレームでの回転量がなす角を超えていたらTargetを向く
	if (Degree <= RotateAngle)
	{
		SetActorRotation(TargetDirection.Rotation(), ETeleportType::None);
		return;
	}
	
	const FVector NextDirection = FVector::SlerpVectorToDirection(BaseVector, TargetDirection, RotateAngle / Degree);
	SetActorRotation(NextDirection.Rotation(), ETeleportType::None);
}

bool ACharacterBase::LookingAtTarget()
{
	switch (CharacterState)
	{
	case ECharacterState::Normal:
	case ECharacterState::Defense:
	case ECharacterState::Attack:
		break;
	default:
		return false;
	}

	// todo tagなどによる攻撃中の回転制御
	
	return true;
}

void ACharacterBase::ApplyHitGameplayEffect_Implementation(float Damage, float Impact)
{
}

void ACharacterBase::OnDead_Implementation()
{
	UKismetSystemLibrary::PrintString(this, "called on dead");

	CharacterState = ECharacterState::Dead;
	//StopAnimMontage(GetCurrentMontage()); // montageを停止
}

void ACharacterBase::OnBrokePosture_Implementation()
{
	UKismetSystemLibrary::PrintString(this, "called on broke posture");
}

bool ACharacterBase::Parried_Implementation(float Impact)
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
