#pragma once

#include "CoreMinimal.h"
#include "Character/CharacterBase.h"
#include "EnemyCharacter.generated.h"

UCLASS()
class SEKIRO_API AEnemyCharacter : public ACharacterBase
{
	GENERATED_BODY()

public:
	AEnemyCharacter();

	// 発動中のAbilityを取得
	UFUNCTION(BlueprintCallable, Category = "Abilities")
	void GetActiveAbilitiesWithTags(const FGameplayTagContainer AbilityTags, TArray<UGameplayAbility*>& ActiveAbilities) const;

	// LockOn可能か
	UFUNCTION(BlueprintCallable, BlueprintPure)
	bool IsTargetable() const { return CharacterState != ECharacterState::Dead; }
};
