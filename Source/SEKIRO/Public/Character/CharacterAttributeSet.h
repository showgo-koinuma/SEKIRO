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

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FDynamicMulticastDelegate);

UCLASS()
class SEKIRO_API UCharacterAttributeSet : public UAttributeSet
{
	GENERATED_BODY()

public:
	UCharacterAttributeSet();
	
	// 値が変更されたときに値のClampと判定を行う
	virtual void PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue) override;

	// Hpが0になると発火
	UPROPERTY(BlueprintAssignable, Category = "Event")
	FDynamicMulticastDelegate OnDead;

	// 体幹が0になると発火
	UPROPERTY(BlueprintAssignable, Category = "Event")
	FDynamicMulticastDelegate OnBrokePosture;

	// 体力
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FGameplayAttributeData Hp;
	ATTRIBUTE_ACCESSORS(UCharacterAttributeSet, Hp);

	// 体幹
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FGameplayAttributeData PosturePoint;
	ATTRIBUTE_ACCESSORS(UCharacterAttributeSet, PosturePoint);

	// 体幹回復速度(秒)
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FGameplayAttributeData PostureRegeneration;
	ATTRIBUTE_ACCESSORS(UCharacterAttributeSet, PostureRegeneration);

	// 防御中増加体幹回復速度割合
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FGameplayAttributeData DefensePostureRegenerationRate;
	ATTRIBUTE_ACCESSORS(UCharacterAttributeSet, DefensePostureRegenerationRate);

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
