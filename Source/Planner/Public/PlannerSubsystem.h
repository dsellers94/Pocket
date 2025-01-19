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
		const TArray<FAction>& ActionSet,
		TMap<EWorldStateKey, bool> WorldState, 
		EWorldStateKey GoalKey,
		bool GoalValue);

private:

	UPROPERTY()
	TArray<FGuid> OpenActions = TArray<FGuid>();

	UPROPERTY()
	TArray<FGuid> ClosedActions = TArray<FGuid>();

	UPROPERTY()
	TArray<FAction> CurrentActionSet;

	UPROPERTY()
	int BestCost = INT_MAX;

	UPROPERTY()
	FGuid RootID;

	TArray<FGuid> OpenSet = TArray<FGuid>();

	TArray<FGuid> ClosedSet = TArray<FGuid>();

	UFUNCTION()
	TArray<FAction> ReconstructPlan(FGuid FirstActionID, EWorldStateKey GoalKey, bool GoalValue);

	UFUNCTION()
	FAction FetchActionFromCurrentSetByID(FGuid ActionID);

	UFUNCTION()
	bool CheckConditionsAgainstWorldState(const TMap<EWorldStateKey, bool>& InUnsatisfiedConditions, const TMap<EWorldStateKey, bool>& InWorldState);

	UFUNCTION()
	bool CheckAndUpdateBestCost(FGuid ActionID);

	UFUNCTION()
	void AppendMapNonDestructive(TMap<EWorldStateKey, bool>& TargetMap, TMap<EWorldStateKey, bool>& SourceMap);
	
};
