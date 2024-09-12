#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "EnemyAICParameter.generated.h"

/**
 * AICで必要な追加のパラメーター
 */
UCLASS()
class SEKIRO_API UEnemyAICParameter : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	// 横移動時の基礎スピードからの割合
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Movement")
	float MoveSidewaysSpeedRate;
};
