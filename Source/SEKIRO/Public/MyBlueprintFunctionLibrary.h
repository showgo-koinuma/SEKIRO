// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "AbilitySystemComponent.h"
#include "MyBlueprintFunctionLibrary.generated.h"

UCLASS()
class SEKIRO_API UMyBlueprintFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
	UFUNCTION(BlueprintCallable, meta = (DefaultToSelf = "Target"))
	static void AddAbilityToComponent(AActor* Target, UAbilitySystemComponent* AbilitySystem, TSubclassOf<class UGameplayAbility> Ability);	
};
