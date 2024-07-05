#pragma once

#include "CoreMinimal.h"
#include "SEKIRO/SEKIRO.h"
#include "Abilities/GameplayAbility.h"
#include "WaitInputGameplayAbility.generated.h"

// WaitInputを使えるGameplayAbility
UCLASS()
class SEKIRO_API UWaitInputGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AbilityInputType")
	EAbilityInputType AbilityInputType = EAbilityInputType::None;
};
