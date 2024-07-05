#pragma once

#include "CoreMinimal.h"

UENUM(BlueprintType)
enum class EAbilityInputType : uint8
{
	None		UMETA(DisplayName = "None"),
	Confirm		UMETA(DisplayName = "Confirm"),
	Cancel		UMETA(DisplayName = "Cancel"),
	Defense		UMETA(DisplayName = "Defense")
};
