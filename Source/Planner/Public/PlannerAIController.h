// Copyright Daniel Sellers 2024.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "PlannerStructs.h"
#include "PlannerAIController.generated.h"

/**
 * 
 */
UCLASS()
class PLANNER_API APlannerAIController : public AAIController
{
	GENERATED_BODY()

public:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<FDataTableRowHandle> ActionRows;

protected:

	UPROPERTY()
	TMap<EWorldStateKey, bool> WorldState;

	UPROPERTY()
	TArray<FAction> ActionSet = TArray<FAction>();

protected:
	
	virtual void OnPossess(APawn* InPawn) override;
	
};
