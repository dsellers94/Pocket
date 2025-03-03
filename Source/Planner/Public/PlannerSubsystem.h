// Copyright Daniel Sellers 2024.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "PlannerStructs.h"
#include "PlannerAIController.h"
#include "Planner/Planner.h"
#include "PlannerSubsystem.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnPlanningComplete, FGuid, PlanID, TArray<FAction>, Plan);

UCLASS()
class PLANNER_API UPlannerSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintAssignable)
	FOnPlanningComplete OnPlanningComplete;

	UFUNCTION()
	void RequestPlan(
		APlannerAIController* Agent,
		const TArray<FAction>& ActionSet,
		TArray<FWorldStatePair> WorldState,
		FWorldStatePair GoalState,
		FGuid PlanID);
	
	UFUNCTION()
	TArray<FAction> GeneratePlan(
		APlannerAIController* Agent,
		const TArray<FAction>& ActionSet,
		TArray<FWorldStatePair> WorldState, 
		FWorldStatePair GoalState);

private:

	UFUNCTION()
	TArray<FAction> ReconstructPlan(
		FGuid FirstActionID, 
		TArray<FAction> InActionSet, 
		FWorldStatePair GoalState,
		FGuid RootID);

	UFUNCTION()
	FAction FetchActionByID(FGuid ActionID, TArray<FAction>& CurrentActionSet);

	UFUNCTION()
	bool CheckConditionsAgainstWorldState(const TArray<FWorldStatePair>& InUnsatisfiedConditions, const TArray<FWorldStatePair>& InWorldState);

	UFUNCTION()
	bool CheckSingleConditionAgainstWorldState(FWorldStatePair Condition, const TArray<FWorldStatePair>& InWorldState);

	UFUNCTION()
	bool CheckAndUpdateBestCost(FAction Action, int& OutBestCost);

	UFUNCTION()
	void AppendMapNonDestructive(TArray<FWorldStatePair>& TargetMap, TArray<FWorldStatePair>& SourceMap);

	UFUNCTION()
	void AppendConditionsNoDuplicates(TArray<FWorldStatePair>& TargetArray, TArray<FWorldStatePair>& SourceArray);
	
};
