#include "Character/EnemyCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"

AEnemyCharacter::AEnemyCharacter()
{
}

void AEnemyCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	SetEnemyRotate(DeltaSeconds);
}

void AEnemyCharacter::GetActiveAbilitiesWithTags(const FGameplayTagContainer AbilityTags,
                                                 TArray<UGameplayAbility*>& ActiveAbilities) const
{
	TArray<FGameplayAbilitySpec*> AbilitiesToActivate;
	AbilitySystemComponent->GetActivatableGameplayAbilitySpecsByAllMatchingTags(AbilityTags, AbilitiesToActivate, false);

	for (const auto Spec : AbilitiesToActivate)
	{
		TArray<UGameplayAbility*> AbilityInstances = Spec->GetAbilityInstances();

		for (const auto ActiveAbility : AbilityInstances)
		{
			ActiveAbilities.Add(Cast<UGameplayAbility>(ActiveAbility));
		}
	}
}

void AEnemyCharacter::SetEnemyRotate(float DeltaTime)
{
	// Targetがいるならその方向、いないなら進行方向を向く
	if (TargetActor)
	{
		GetCharacterMovement()->bOrientRotationToMovement = false;
		SetRotationToTarget(TargetActor->GetActorLocation() - GetActorLocation(), DeltaTime);
	}
	else
	{
		GetCharacterMovement()->bOrientRotationToMovement = true;
	}
}
