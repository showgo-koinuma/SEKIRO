#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "CharacterParameter.generated.h"

UCLASS(BlueprintType)
class SEKIRO_API UCharacterParameter : public UDataAsset
{
	GENERATED_BODY()

public:
	// 体力
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Status")
	float Hp;

	// 体幹
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Status")
	float Posture;

	// 移動速度
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement")
	float MaxSpeed;

	// 加速度
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement")
	float Acceleration;

	// 振り向き速度
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement")
	float TurnSpeed;
};
