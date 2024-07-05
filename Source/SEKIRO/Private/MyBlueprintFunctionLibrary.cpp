// Fill out your copyright notice in the Description page of Project Settings.


#include "MyBlueprintFunctionLibrary.h"

void UMyBlueprintFunctionLibrary::AddAbilityToComponent(AActor* Target, UAbilitySystemComponent* AbilitySystem,
	TSubclassOf<UGameplayAbility> Ability)
{
	if (Target && AbilitySystem && Ability){
		AbilitySystem->GiveAbility(FGameplayAbilitySpec(Ability.GetDefaultObject(), 1, 1));
	}
}
