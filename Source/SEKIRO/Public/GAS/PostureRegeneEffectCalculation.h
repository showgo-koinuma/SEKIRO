#pragma once

#include "CoreMinimal.h"
#include "GameplayModMagnitudeCalculation.h"
#include "PostureRegeneEffectCalculation.generated.h"

// 体幹の自動回復の量を求める
UCLASS()
class SEKIRO_API UPostureRegeneEffectCalculation : public UGameplayModMagnitudeCalculation
{
	GENERATED_BODY()

public:
	UPostureRegeneEffectCalculation();

	virtual float CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const override;

	// Hp Attribute
	FGameplayEffectAttributeCaptureDefinition HpDef;

	// MaxHp Attribute
	FGameplayEffectAttributeCaptureDefinition MaxHpDef;
};
