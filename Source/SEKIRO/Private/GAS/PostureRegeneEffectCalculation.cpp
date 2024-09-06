#include "GAS/PostureRegeneEffectCalculation.h"

#include "Character/CharacterAttributeSet.h"
#include "Character/CharacterBase.h"

UPostureRegeneEffectCalculation::UPostureRegeneEffectCalculation()
{
	HpDef.AttributeToCapture = UCharacterAttributeSet::GetHpAttribute();
	HpDef.AttributeSource = EGameplayEffectAttributeCaptureSource::Target;
	HpDef.bSnapshot = false;

	MaxHpDef.AttributeToCapture = UCharacterAttributeSet::GetMaxHpAttribute();
	MaxHpDef.AttributeSource = EGameplayEffectAttributeCaptureSource::Target;
	MaxHpDef.bSnapshot = false;

	RelevantAttributesToCapture.Add(HpDef);
	RelevantAttributesToCapture.Add(MaxHpDef);
}

float UPostureRegeneEffectCalculation::CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const
{
	const ACharacterBase* TargetCharacter = static_cast<ACharacterBase*>(Spec.GetContext().GetEffectCauser());
	const UGameplayEffect* GameplayEffect = Spec.Def;

	if (TargetCharacter && GameplayEffect)
	{
		const float Regene = TargetCharacter->CharacterParameter->PostureRegeneration;
		const float RateOnDefense = TargetCharacter->CharacterParameter->DefensePostureRegenerationRate;
		const float Period = GameplayEffect->Period.Value;

		// 評価パラメータを取得
		const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
		const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();
		FAggregatorEvaluateParameters EvaluationParameters;
		EvaluationParameters.SourceTags = SourceTags;
		EvaluationParameters.TargetTags = TargetTags;
		
		float Hp = 0.f;
		GetCapturedAttributeMagnitude(HpDef, Spec, EvaluationParameters, Hp);
		float MaxHp = 0.f;
		GetCapturedAttributeMagnitude(MaxHpDef, Spec, EvaluationParameters, MaxHp);

		const float Value = Regene * Hp / MaxHp * Period * (TargetCharacter->CharacterState == ECharacterState::Defense ? RateOnDefense : 1.f);
		return Value;
	}

	UE_LOG(LogTemp, Error, TEXT("Actorを取得できなかった"));
	
	return 0;
}
