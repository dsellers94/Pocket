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

	// Set CurrentActionID and the effects to be checked to the last action in the plan
	TMap<EWorldStateKey, bool> CurrentActionEffects = FetchActionFromCurrentSetByGuid(FirstActionID).Effects;
	FGuid CurrentActionID = FirstActionID;

	// Until we detect that the effects of the current action satisfy the goal state, Get the CurrentAction by it's ID and add it to the plan.
	// Then check if we should break. If not, set the CurrentActionID to that of the ParentActionID of the CurrentActionNode.
	while (true)
	{
		FAction CurrentAction = FetchActionFromCurrentSetByGuid(CurrentActionID);
		FActionNode CurrentActionNode = FetchActionNodeByGuid(CurrentActionID);
		CurrentActionEffects = CurrentAction.Effects;
		Plan.Add(CurrentAction);

		if (CurrentActionEffects.Contains(GoalKey) && CurrentActionEffects[GoalKey] == GoalValue) break;

		CurrentActionID = CurrentActionNode.ParentActionID;
	}

	return Plan;
}

FAction UPlannerSubsystem::FetchActionFromCurrentSetByGuid(FGuid ActionID)
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

FActionNode UPlannerSubsystem::FetchActionNodeByGuid(FGuid ActionID)
{
	for (FActionNode ActionNode : ActionNodes)
	{
		if (ActionNode.ActionID == ActionID)
		{
			return ActionNode;
		}
	}

	UE_LOG(LogPlanner, Error, TEXT("No ActionNode Found for given ActionID! Likely a fatal error!"));

	return FActionNode();
}


