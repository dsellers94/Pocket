// Copyright Daniel Sellers 2024.


#include "PlannerSubsystem.h"


TArray<FAction> UPlannerSubsystem::GeneratePlan(
	APlannerAIController* Agent, 
	TArray<FAction> ActionSet,
	TMap<EWorldStateKey, bool> WorldState, 
	EWorldStateKey GoalKey,
	bool GoalValue)
{
	TArray<FAction> Plan = TArray<FAction>();

	CurrentActionSet = ActionSet;


	return Plan;
}

TArray<FAction> UPlannerSubsystem::ReconstructPlan(FGuid FirstActionID, EWorldStateKey GoalKey, bool GoalValue)
{
	TArray<FAction> Plan = TArray<FAction>();

	TMap<EWorldStateKey, bool> CurrentActionEffects = FetchActionByGuid(FirstActionID).Effects;

	FGuid CurrentActionID;

	while (true)
	{
		FAction CurrentAction = 
		if (CurrentActionEffects.Contains(GoalKey) && CurrentActionEffects[GoalKey] == GoalValue) break;
	}

	return Plan;
}

FAction UPlannerSubsystem::FetchActionByGuid(FGuid ActionID)
{
	for (FAction Action : CurrentActionSet)
	{
		if (Action.ActionID == ActionID)
		{
			return Action;
		}
	}
	
	UE_LOG(LogPlanner, Error, TEXT("No Action Found for given ActionID! Likely a fatal error!"));
	
	return FAction();
}


