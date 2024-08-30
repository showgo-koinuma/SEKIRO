#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "ITargetableInterface.generated.h"

UINTERFACE(MinimalAPI)
class UITargetableInterface : public UInterface
{
	GENERATED_BODY()
};

// ロックオン可能にするインターフェイス
class SEKIRO_API IITargetableInterface
{
	GENERATED_BODY()

protected:
	// ロックオンされているか
	bool IsTargeted = false;

public:
	// ロックオン可能か
	virtual bool IsTargetable() const { return false; }

	virtual FVector GetTargetLocation() const { return FVector(); }
	
	void SetIsTargeting(const bool IsTargeting)
	{
		IsTargeted = IsTargeting;
	}

	virtual bool GetIsTargeting() const = 0;
};
