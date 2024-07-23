#include "Character/EnemyCharacter.h"

AEnemyCharacter::AEnemyCharacter()
{
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
