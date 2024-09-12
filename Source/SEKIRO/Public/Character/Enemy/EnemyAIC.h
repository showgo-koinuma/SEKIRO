#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "EnemyAIC.generated.h"

/**
 * AIControllerもC++で書こうと思ったけどよくわからんから一旦放置
 */
UCLASS()
class SEKIRO_API AEnemyAIC : public AAIController
{
	GENERATED_BODY()

	AEnemyAIC();

	virtual void BeginPlay() override;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "BT");
	TObjectPtr<UBehaviorTree> BehaviorTree;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "BT")
	TObjectPtr<UBehaviorTreeComponent> BehaviorTreeComponent;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "BT")
	TObjectPtr<UBlackboardComponent> BlackboardComponent;
};
