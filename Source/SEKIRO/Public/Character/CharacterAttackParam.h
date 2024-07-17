#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "CharacterAttackParam.generated.h"

// キャラクターの攻撃のパラメーター
UCLASS()
class SEKIRO_API UCharacterAttackParam : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	// ダメージ
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float Damage;

	// 衝撃の大きさ
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float Impact;
};
