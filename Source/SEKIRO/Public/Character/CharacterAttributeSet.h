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

	// 現在体力
	UPROPERTY(BlueprintReadWrite)
	FGameplayAttributeData Hp;
	ATTRIBUTE_ACCESSORS(UCharacterAttributeSet, Hp);

	// Max体力
	UPROPERTY(BlueprintReadWrite)
	FGameplayAttributeData MaxHp;
	ATTRIBUTE_ACCESSORS(UCharacterAttributeSet, MaxHp);

	// 現在体幹
	UPROPERTY(BlueprintReadWrite)
	FGameplayAttributeData PosturePoint;
	ATTRIBUTE_ACCESSORS(UCharacterAttributeSet, PosturePoint);

	// Max体幹
	UPROPERTY(BlueprintReadWrite)
	FGameplayAttributeData MaxPosturePoint;
	ATTRIBUTE_ACCESSORS(UCharacterAttributeSet, MaxPosturePoint);
	
	// 値が変更されたときに値のClampと判定を行う
	virtual void PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data) override;

	// Hpが0になると発火
	UPROPERTY(BlueprintAssignable, Category = "Event")
	FDynamicMulticastDelegate OnDead;

	// 体幹が0になると発火
	UPROPERTY(BlueprintAssignable, Category = "Event")
	FDynamicMulticastDelegate OnBrokePosture;
};
