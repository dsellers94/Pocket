// Copyright Daniel Sellers 2024.

#pragma once

#include "CoreMinimal.h"
#include "PlannerAIController.h"
#include "PocketAIController.generated.h"

class UBehaviorTree;
class UBlackboardComponent;

UCLASS()
class POCKET_API APocketAIController : public APlannerAIController
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere)
	TObjectPtr<UBehaviorTree> BehaviorTree;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UBlackboardComponent> BlackboardComponent;

	UFUNCTION(BlueprintCallable)
	void SetGetNextGoalFlag();

private:

	virtual void BeginPlay() override;
	
};
