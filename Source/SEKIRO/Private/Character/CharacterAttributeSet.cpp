#include "Character/CharacterAttributeSet.h"
#include "GameplayEffect.h"
#include "GameplayEffectExtension.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Math/UnrealMath.h"

UCharacterAttributeSet::UCharacterAttributeSet()
{
}

void UCharacterAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);

	if (Data.EvaluatedData.Attribute == GetHpAttribute())
	{
		SetHp(FMath::Clamp(GetHp(), 0.f, GetMaxHp()));

		if (GetHp() <= 0.f)
		{
			OnDead.Broadcast();
		}
	}
	else if (Data.EvaluatedData.Attribute == GetPosturePointAttribute())
	{
		SetPosturePoint(FMath::Clamp(GetPosturePoint(), 0.f, GetMaxPosturePoint()));

		if (GetPosturePoint() <= 0.f)
		{
			OnBrokePosture.Broadcast();
		}
	}
}
