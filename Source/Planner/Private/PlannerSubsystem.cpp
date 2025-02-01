// Copyright Daniel Sellers 2024.


#include "PlannerSubsystem.h"

void UPlannerSubsystem::RequestPlan(
	APlannerAIController* Agent, 
	const TArray<FAction>& ActionSet, 
	TMap<FName, bool> WorldState, 
	FName GoalKey, 
	bool GoalValue, 
	FGuid PlanID)
{
	// This is where we will run an GeneratePlan() as an asyn task and return the result via OnPlanningComplete.Broadcast();
}

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
	OpenSet.Empty();
	ClosedSet.Empty();
	BestCost = INT_MAX;

	if (!IsValid(Agent))
	{
		UE_LOG(LogPlanner, Error, TEXT("Invalid Agent, likely a fatal error"));
	}
	if (ActionSet.Num() == 0)
	{
		UE_LOG(LogPlanner, Error, TEXT("Action Set is empty, planning aborted"));
		return Plan;
	}
	if (WorldState.Num() == 0)
	{
		UE_LOG(LogPlanner, Error, TEXT("World State is empty, planning aborted"));
		return Plan;
	}

	//TO DO: Handle the edge case where the plan already matches the worldstate?
	
	CurrentActionSet = ActionSet; // Set global CurrentActionSet used by other methods in this class

	RootID = FGuid::NewGuid(); // Set global RootID used to detect when actions adjacent to the goal state have been reached during plan reconstruction.

	// Start by finding all actions whose effects satisfy the goal state (GoalKey and GoalValue), add them to the open set, set their costs, unsat. conds., and parent IDs
	for (FAction CheckAction : CurrentActionSet)
	{
		FGuid CheckActionID = CheckAction.ActionID;
		if (CheckAction.Effects.Contains(GoalKey) && CheckAction.Effects[GoalKey] == GoalValue)
		{
			CheckAction.CalculatedCost = 1;
			CheckAction.UnSatisfiedConditions = CheckAction.Preconditions;
			CheckAction.ParentActionID = RootID;
			OpenSet.Add(CheckAction);

			// Handle the case of a one step plan. 
			// We don't want to return early in this case: Our heauristic functions (each step costs 1 + a satisfied condition is closer to the target) are currently very simple,
			// but we may want to implement a more sophisticated cost/closeness function in the future, so it's worth checking the remaining open nodes for now.
			if (CheckConditionsAgainstWorldState(CheckAction.UnSatisfiedConditions, WorldState))
			{
				if (CheckAndUpdateBestCost(CheckAction))
				{
					Plan = ReconstructPlan(CheckActionID, OpenSet, GoalKey, GoalValue);
				}
			}
		}
	}

	// While we have unexplored nodes which may lead back to the current worldstate
	while (!OpenSet.IsEmpty())
	{
		FAction CurrentAction = OpenSet.Pop();
		ClosedSet.Add(CurrentAction);

		// Iterate over unsatisfied conditions stored on the current action (accumulated from previous actions explored on this path)
		for (auto UnSatPair : CurrentAction.UnSatisfiedConditions)
		{
			// If this particular precondition is satisfied by the worldstate, we don't need to explore actions which also satisfy it, continue instead
			if (CheckSingleConditionAgainstWorldState(UnSatPair.Key, UnSatPair.Value, WorldState))
			{
				continue;
			}
			// Iterate over available actions
			for (FAction CheckAction : CurrentActionSet)
			{
				// Check if the action satisfies the condition we're currently considering.
				if (CheckAction.Effects.Contains(UnSatPair.Key) && CheckAction.Effects[UnSatPair.Key] == UnSatPair.Value)
				{
					//ToDo: This is where we might spawn a ContextCheckActor, or now that I think of it, call a more general Context Checker. Either way, ignoring that step for now
					// The context check actors could pull double duty: check if an action is possible and check if it is effective

					// Handle the case of an action we've already closed (update cost and conditions if it would be less than the cost already stored on that action)
					if (ClosedSet.Contains(CheckAction))
					{
						if (CurrentAction.CalculatedCost + 1 < CheckAction.CalculatedCost)
						{
							CheckAction.CalculatedCost = CurrentAction.CalculatedCost + 1; // Set the new cost for the check action
							CheckAction.ParentActionID = CurrentAction.ActionID;
							CheckAction.UnSatisfiedConditions = CurrentAction.UnSatisfiedConditions; // Set the new set of unsat. conds. for the check action (inherited from current)
							AppendMapNonDestructive(CheckAction.UnSatisfiedConditions, CheckAction.Preconditions); // Add the action's own preconditions back to the unsat. list
							CheckAction.UnSatisfiedConditions.Remove(UnSatPair.Key); // Remove the unsat. cond. which we've just verified is satisfied by the check action
						}
					}
					else
					{
						CheckAction.CalculatedCost = CurrentAction.CalculatedCost + 1; // Set the new cost for the check action
						CheckAction.UnSatisfiedConditions = CurrentAction.UnSatisfiedConditions; // Set the new set of unsat. conds. for the check action (inherited from current)
						AppendMapNonDestructive(CheckAction.UnSatisfiedConditions, CheckAction.Preconditions); // Add the action's own preconditions back to the unsat. list
						CheckAction.UnSatisfiedConditions.Remove(UnSatPair.Key); // Remove the unsat. cond. which we've just verified is satisfied by the check action
						CheckAction.ParentActionID = CurrentAction.ActionID;
						OpenSet.Add(CheckAction);
					}
				}
			}
		}
		// NOTE: We may have a problem above with an action's preconditions overwriting any direct conflicts with the unsatisfied conditions which it has inherited from
		// it's new parent action. If this becomes a problem before we decide to overhaul the precondition system we'll have to add some checks 
		// and probably invalidate the new action if it conflicts with an existing unsat condition from the parent path.

		if (CheckConditionsAgainstWorldState(CurrentAction.UnSatisfiedConditions, WorldState))
		{
			if (CheckAndUpdateBestCost(CurrentAction))
			{
				Plan = ReconstructPlan(CurrentAction.ActionID, ClosedSet, GoalKey, GoalValue);
			}
		}
	}

	return Plan;
}

TArray<FAction> UPlannerSubsystem::ReconstructPlan(FGuid FirstActionID, TArray<FAction> InActionSet, FName GoalKey, bool GoalValue)
{
	TArray<FAction> Plan = TArray<FAction>();

	// Set CurrentActionID
	FGuid CurrentActionID = FirstActionID;

	while (CurrentActionID != RootID)
	{
		for (FAction Action : InActionSet)
		{
			if (Action.ActionID == CurrentActionID)
			{
				Plan.Add(Action);
				CurrentActionID = Action.ParentActionID;
				break;
			}
		}
	}

	return Plan;
}

FAction UPlannerSubsystem::FetchActionFromCurrentSetByID(FGuid ActionID)
{
	static FAction ResultAction;
	for (FAction& Action : CurrentActionSet)
	{
		if (Action.ActionID == ActionID)
		{
			ResultAction = Action;
		}
	}
	
	UE_LOG(LogPlanner, Error, TEXT("No Action Found for given ActionID! Likely a fatal error!"));
	
	return ResultAction;
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

bool UPlannerSubsystem::CheckSingleConditionAgainstWorldState(FName ConditionKey, bool ConditionValue, const TMap<FName, bool>& InWorldState)
{
	if (!InWorldState.Contains(ConditionKey))
	{
		return false;
	}
	else if (ConditionValue != InWorldState[ConditionKey])
	{
		return false;
	}
	
	return true;
}

bool UPlannerSubsystem::CheckAndUpdateBestCost(FAction Action)
{
	// If the cost set by GeneratePlan so far is not less than the current best known cost, return without reconstructing a plan.
	if (Action.CalculatedCost < BestCost)
	{
		BestCost = Action.CalculatedCost;
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




