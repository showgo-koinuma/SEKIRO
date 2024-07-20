#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "CharacterAttributeSet.generated.h"

#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

UCLASS()
class SEKIRO_API UCharacterAttributeSet : public UAttributeSet
{
	GENERATED_BODY()

public:
	UCharacterAttributeSet();

	// 体力
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FGameplayAttributeData Hp;
	ATTRIBUTE_ACCESSORS(UCharacterAttributeSet, Hp);

	// 体幹
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FGameplayAttributeData Posture;
	ATTRIBUTE_ACCESSORS(UCharacterAttributeSet, Posture);

	// 最大速度
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FGameplayAttributeData MaxSpeed;
	ATTRIBUTE_ACCESSORS(UCharacterAttributeSet, MaxSpeed);

	// 加速度
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FGameplayAttributeData Acceleration;
	ATTRIBUTE_ACCESSORS(UCharacterAttributeSet, Acceleration);

	// 回転速度
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FGameplayAttributeData TurnSpeed;
	ATTRIBUTE_ACCESSORS(UCharacterAttributeSet, TurnSpeed);
};
