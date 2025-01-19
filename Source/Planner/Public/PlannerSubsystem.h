// Copyright Daniel Sellers 2024.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "PlannerStructs.h"
#include "PlannerAIController.h"
#include "Planner/Planner.h"
#include "PlannerSubsystem.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnPlanningComplete);

UCLASS()
class PLANNER_API UPlannerSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintAssignable)
	FOnPlanningComplete OnPlanningComplete;
	
	UFUNCTION()
	TArray<FAction> GeneratePlan(
		APlannerAIController* Agent,
		TArray<FAction> ActionSet,
		TMap<EWorldStateKey, bool> WorldState, 
		EWorldStateKey GoalKey,
		bool GoalValue);

private:

	UPROPERTY()
	TArray<FGuid> OpenActions = TArray<FGuid>();

	UPROPERTY()
	TArray<FGuid> ClosedActions = TArray<FGuid>();

	UPROPERTY()
	TArray<FAction> CurrentActionSet = TArray<FAction>();

	UFUNCTION()
	TArray<FAction> ReconstructPlan(FGuid FirstActionID, EWorldStateKey GoalKey, bool GoalValue);

	UFUNCTION()
	FAction FetchActionFromCurrentSetByID(FGuid ActionID);
	
};
