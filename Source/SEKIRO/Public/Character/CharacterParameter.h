#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "Engine/DataAsset.h"
#include "CharacterParameter.generated.h"

UCLASS(BlueprintType)
class SEKIRO_API UCharacterParameter : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	// 体力
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Status")
	float Hp;

	// 体幹
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Posture")
	float PosturePoint;

	// 体幹回復速度(秒)
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Posture")
	float PostureRegeneration;

	// 防御中の体幹回復速度増加割合
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Posture")
	float DefensePostureRegenerationRate;

	// 移動速度
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement")
	float MaxSpeed;

	// 加速度
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement")
	float Acceleration;

	// 振り向き速度
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement")
	float TurnSpeed;

	// GameplayEffectを発行し、ステータスを適用する
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void ApplyStatus(const UAbilitySystemComponent* Target);
};
