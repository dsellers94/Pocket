// Copyright Daniel Sellers 2024.


#include "PlannerSubsystem.h"

// The goal is to convert this function to run in an async thread and have it broadcast an event with an FGuid parameter (or a specified agent?) to inform the requesting
// agent that the plan is ready. For now we're just going to run it synchronously while I test and improve the algorithm. 
// The requesting agent is a parameter because we may decide to load an actor from the TSoftClassPtr property to check some actions for validity given the game state,
// but for now that parameter is not being used in this function.
TArray<FAction> UPlannerSubsystem::GeneratePlan(
	APlannerAIController* Agent, 
	const TArray<FAction>& ActionSet,
	TMap<FName, bool> WorldState, 
	FName GoalKey,
	bool GoalValue)
{
	TArray<FAction> Plan = TArray<FAction>();
	
	CurrentActionSet = ActionSet; // Set global CurrentActionSet used by other methods in this class

	RootID = FGuid::NewGuid(); // Set global RootID used to detect when actions adjacent to the goals state have been reached during plan reconstruction.

	// Start by finding all actions whose effects satisfy the goal state (GoalKey and GoalValue), add them to the open set, set their costs, unsat. conds., and parent IDs
	for (FAction CheckAction : ActionSet)
	{
		FGuid CheckActionID = CheckAction.ActionID;
		if (CheckAction.Effects.Contains(GoalKey) && CheckAction.Effects[GoalKey] == GoalValue)
		{
			CheckAction.Cost = 1;
			CheckAction.UnSatisfiedConditions = CheckAction.Preconditions;
			CheckAction.ParentActionID = RootID;
			OpenSet.Add(CheckActionID);

			// Handle the case of a one step plan. 
			// We don't want to return early in this case: Our heauristic functions (each step costs 1 + a satisfied condition is closer to the target) are currently very simple,
			// but we may want to implement a more sophisticated cost/closeness function in the future, so it's worht checking the remaining open nodes for now.
			if (CheckConditionsAgainstWorldState(CheckAction.UnSatisfiedConditions, WorldState))
			{
				if (CheckAndUpdateBestCost(CheckActionID))
				{
					Plan = ReconstructPlan(CheckActionID, GoalKey, GoalValue);
				}
			}
		}
	}

	while (!OpenSet.IsEmpty())
	{
		FGuid CurrentActionID = OpenSet.Pop();
		ClosedSet.Add(CurrentActionID);
		FAction CurrentAction = FetchActionFromCurrentSetByID(CurrentActionID);

		// Iterate over unsatisfied conditions stored on the current action (accumulate from previous actions explored on this path, I think)
		for (auto UnSatPair : CurrentAction.UnSatisfiedConditions)
		{
			// Iterate over available actions
			for (FAction CheckAction : ActionSet)
			{
				// Check if the action satisfies any of our unsatisfied conditions.
				if (CheckAction.Effects.Contains(UnSatPair.Key) && CheckAction.Effects[UnSatPair.Key] == UnSatPair.Value)
				{
					//ToDo: This is where we might spawn a ContextCheckActor, or now that I think of it, call a more general Context Checker. Either way, ignoring that step for now
					
					// Handle the case of an action we've already closed (update cost and conditions if it would be less than the cost already stored on that action)
					if (ClosedSet.Contains(CheckAction.ActionID))
					{
						if (CurrentAction.Cost < CheckAction.Cost + 1)
						{
							CheckAction.Cost = CurrentAction.Cost + 1; // Set the new cost for the check action
							CheckAction.ParentActionID = CurrentAction.ActionID;
							CheckAction.UnSatisfiedConditions = CurrentAction.UnSatisfiedConditions; // Set the new set of unsat. conds. for the check action (inherited from current)
							AppendMapNonDestructive(CheckAction.UnSatisfiedConditions, CheckAction.Preconditions); // Add the action's own preconditions back to the unsat. list
							CheckAction.UnSatisfiedConditions.Remove(UnSatPair.Key); // Remove the unsat. cond. which we've just verified is satisfied by the check action
						}
					}
					else
					{
						OpenSet.Add(CheckAction.ActionID);
						CheckAction.Cost = CurrentAction.Cost + 1; // Set the new cost for the check action
						CheckAction.UnSatisfiedConditions = CurrentAction.UnSatisfiedConditions; // Set the new set of unsat. conds. for the check action (inherited from current)
						AppendMapNonDestructive(CheckAction.UnSatisfiedConditions, CheckAction.Preconditions); // Add the action's own preconditions back to the unsat. list
						CheckAction.UnSatisfiedConditions.Remove(UnSatPair.Key); // Remove the unsat. cond. which we've just verified is satisfied by the check action
						CheckAction.ParentActionID = CurrentActionID;
					}
				}
			}
		}
		// NOTE: We may have a problem above with an action's preconditions overwriting any direct conflicts with the unsatisfied conditions which it has inherited from
		// it's new parent action. If this becomes a problem before we decide to overhaul the precondition system we'll have to add some checks 
		// and probably invalidate the new action if it conflicts with an existing unsat condition from the parent path.

		if (CheckConditionsAgainstWorldState(CurrentAction.UnSatisfiedConditions, WorldState))
		{
			if (CheckAndUpdateBestCost(CurrentActionID))
			{
				Plan = ReconstructPlan(CurrentActionID, GoalKey, GoalValue);
			}
		}
	}

	return Plan;
}

TArray<FAction> UPlannerSubsystem::ReconstructPlan(FGuid FirstActionID, FName GoalKey, bool GoalValue)
{
	TArray<FAction> Plan = TArray<FAction>();

	// Set CurrentActionID
	FGuid CurrentActionID = FirstActionID;

	// Add the parent action of action to the plan until CurrentActionID is the RootID, 
	// indicating that we have reached the actions which produce the goal state and planning is done
	while (CurrentActionID != RootID)
	{
		FAction CurrentAction = FetchActionFromCurrentSetByID(CurrentActionID);
		Plan.Add(CurrentAction);
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

bool UPlannerSubsystem::CheckConditionsAgainstWorldState(const TMap<FName, bool>& InUnsatisfiedConditions, const TMap<FName, bool>& InWorldState)
{
	for (auto UnSatPair : InUnsatisfiedConditions)
	{
		if (!InWorldState.Contains(UnSatPair.Key))
		{
			return false;
		}
		else if (UnSatPair.Value != InWorldState[UnSatPair.Key])
		{
			return false;
		}
	}

	return true;
}

bool UPlannerSubsystem::CheckAndUpdateBestCost(FGuid ActionID)
{
	// If the cost set by GeneratePlan so far is not less than the current best known cost, return without reconstructing a plan.
	FAction CurrentAction = FetchActionFromCurrentSetByID(ActionID);
	if (CurrentAction.Cost < BestCost)
	{
		BestCost = CurrentAction.Cost;
		return true;
	}
	else
	{
		return false;
	}
}

void UPlannerSubsystem::AppendMapNonDestructive(TMap<FName, bool>& TargetMap, TMap<FName, bool>& SourceMap)
{
	for (const auto& pair : SourceMap)
	{
		TargetMap.Add(pair.Key, pair.Value);
	}
}




