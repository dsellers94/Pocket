// Copyright Daniel Sellers 2024.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "PlannerStructs.h"
#include "ActionSetData.h"
#include "PlannerAIController.generated.h"

class UPlannerComponent;

UCLASS()
class PLANNER_API APlannerAIController : public AAIController
{
	GENERATED_BODY()

protected:

	UPROPERTY()
	TMap<EWorldStateKey, bool> WorldState;

	UPROPERTY()
	TArray<FAction> ActionSet = TArray<FAction>();

	UPROPERTY()
	TObjectPtr<UPlannerComponent> ControlledPlannerComponent = nullptr;


protected:
	
	virtual void OnPossess(APawn* InPawn) override;

private:

	virtual void BeginPlay() override;
	
};
