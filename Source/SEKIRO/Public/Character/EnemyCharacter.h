#pragma once

#include "CoreMinimal.h"
#include "Character/CharacterBase.h"
#include "Enemy/ITargetableInterface.h"
#include "EnemyCharacter.generated.h"

UCLASS()
class SEKIRO_API AEnemyCharacter : public ACharacterBase, public IITargetableInterface
{
	GENERATED_BODY()

public:
	AEnemyCharacter();

	// 発動中のAbilityを取得
	UFUNCTION(BlueprintCallable, Category = "Abilities")
	void GetActiveAbilitiesWithTags(const FGameplayTagContainer AbilityTags, TArray<UGameplayAbility*>& ActiveAbilities) const;

	// LockOn可能か
	UFUNCTION(BlueprintCallable, BlueprintPure)
	virtual bool IsTargetable() const override { return CharacterState != ECharacterState::Dead; }
	
	virtual FVector GetTargetLocation() const override { return GetActorLocation(); }
	
	UFUNCTION(BlueprintCallable, BlueprintPure)
	virtual bool GetIsTargeting() const override { return IsTargeted; }
};
