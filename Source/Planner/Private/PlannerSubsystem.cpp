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

	TArray<FGuid> OpenSet = TArray<FGuid>();

	TArray<FGuid> ClosedSet = TArray<FGuid>();

	CurrentActionSet = ActionSet;

	int CurrentCost = 0;

	for (FAction CheckAction : CurrentActionSet)
	{
		FGuid CheckActionID = CheckAction.ActionID;
		if (CheckAction.Effects.Contains(GoalKey) && CheckAction.Effects[GoalKey] == GoalValue)
		{
			CheckAction.Cost = 1;
			CheckAction.UnSatisfiedConditions = CheckAction.Preconditions;
			OpenSet.Add(CheckActionID);
		}
	}

	while (!OpenSet.IsEmpty())
	{
		FGuid CheckActionID = OpenSet.Pop();
		FAction CheckAction = FetchActionFromCurrentSetByID(CheckActionID);
		if (CheckAction.Effects.Contains(GoalKey) && CheckAction.Effects[GoalKey] == GoalValue)
		{
			//ToDo: This is where we might spawn a ContextCheckActor, or now that I think of it, call a more general Context Checker. Either way, ignoring that step for now
			// CONTINUE ALGORITHM FROM HERE
		}
	}




	return Plan;
}

TArray<FAction> UPlannerSubsystem::ReconstructPlan(FGuid FirstActionID, EWorldStateKey GoalKey, bool GoalValue)
{
	TArray<FAction> Plan = TArray<FAction>();

	// Set CurrentActionID and the effects to be checked to the last action in the plan
	TMap<EWorldStateKey, bool> CurrentActionEffects = FetchActionFromCurrentSetByID(FirstActionID).Effects;
	FGuid CurrentActionID = FirstActionID;

	// Until we detect that the effects of the current action satisfy the goal state, Get the CurrentAction by it's ID and add it to the plan.
	// Then break if the effects of the CurrentAction satisfy the goal state. 
	// If not, set the CurrentActionID to that of the ParentActionID of the CurrentActionNode and continue.
	while (true)
	{
		FAction CurrentAction = FetchActionFromCurrentSetByID(CurrentActionID);
		CurrentActionEffects = CurrentAction.Effects;
		Plan.Add(CurrentAction);

		if (CurrentActionEffects.Contains(GoalKey) && CurrentActionEffects[GoalKey] == GoalValue) break;

		CurrentActionID = CurrentAction.ParentActionID;
	}

	return Plan;
}

FAction UPlannerSubsystem::FetchActionFromCurrentSetByID(FGuid ActionID)
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



