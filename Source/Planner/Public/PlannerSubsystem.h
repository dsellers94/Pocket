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
		TMap<FName, bool> WorldState,
		FName GoalKey,
		bool GoalValue,
		FGuid PlanID);
	
	UFUNCTION()
	TArray<FAction> GeneratePlan(
		APlannerAIController* Agent,
		const TArray<FAction>& ActionSet,
		TMap<FName, bool> WorldState, 
		FName GoalKey,
		bool GoalValue);

private:

	UFUNCTION()
	TArray<FAction> ReconstructPlan(
		FGuid FirstActionID, 
		TArray<FAction> InActionSet, 
		FName GoalKey, 
		bool GoalValue,
		FGuid RootID);

	UFUNCTION()
	FAction FetchActionFromCurrentSetByID(FGuid ActionID, TArray<FAction>& CurrentActionSet);

	UFUNCTION()
	bool CheckConditionsAgainstWorldState(const TMap<FName, bool>& InUnsatisfiedConditions, const TMap<FName, bool>& InWorldState);

	UFUNCTION()
	bool CheckSingleConditionAgainstWorldState(FName ConditionKey, bool ConditionValue, const TMap<FName, bool>& InWorldState);

	UFUNCTION()
	bool CheckAndUpdateBestCost(FAction Action, int& OutBestCost);

	UFUNCTION()
	void AppendMapNonDestructive(TMap<FName, bool>& TargetMap, TMap<FName, bool>& SourceMap);
	
};
