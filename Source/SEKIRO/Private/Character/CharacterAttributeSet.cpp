#include "Character/CharacterAttributeSet.h"
#include "Character/CharacterBase.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Math/UnrealMath.h"

UCharacterAttributeSet::UCharacterAttributeSet()
{
}

void UCharacterAttributeSet::PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue)
{
	Super::PostAttributeChange(Attribute, OldValue, NewValue);

	// それぞれ変更されたときにClampと判定を行う
	if (Attribute == GetHpAttribute())
	{
		Hp.SetCurrentValue(FMath::Clamp(Hp.GetCurrentValue(), 0, Hp.GetBaseValue()));

		if (Hp.GetCurrentValue() <= 0)
		{
			OnDead.Broadcast();
		}
	}
	else if (Attribute == GetPosturePointAttribute())
	{
		PosturePoint.SetCurrentValue(FMath::Clamp(PosturePoint.GetCurrentValue(), 0, PosturePoint.GetBaseValue()));

		if (PosturePoint.GetCurrentValue() <= 0)
		{
			OnBrokePosture.Broadcast();
		}
	}
	else if (Attribute == GetMaxSpeedAttribute())
	{
		MaxSpeed.SetCurrentValue(FMath::Max(0, MaxSpeed.GetCurrentValue()));
	}
}
