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

	virtual void Tick(float DeltaSeconds) override;

	// 発動中のAbilityを取得
	UFUNCTION(BlueprintCallable, Category = "Abilities")
	void GetActiveAbilitiesWithTags(const FGameplayTagContainer AbilityTags, TArray<UGameplayAbility*>& ActiveAbilities) const;

	// LockOn
	
	UFUNCTION(BlueprintCallable, BlueprintPure)
	virtual bool IsTargetable() const override { return CharacterState != ECharacterState::Dead; }
	
	virtual FVector GetTargetLocation() const override { return GetActorLocation(); }
	
	UFUNCTION(BlueprintCallable, BlueprintPure)
	virtual bool GetIsTargeting() const override { return IsTargeted; }

	//EnemyRotate

	// EnemyのRotationの設定
	UFUNCTION(BlueprintCallable)
	void SetEnemyRotate(float DeltaTime);
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<AActor> TargetActor = nullptr;
};
