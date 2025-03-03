// Copyright Daniel Sellers 2024.

#include "PlannerSubsystem.h"
#include "Async/Async.h"

void UPlannerSubsystem::RequestPlan(
	APlannerAIController* Agent, 
	const TArray<FAction>& ActionSet, 
	TArray<FWorldStatePair> WorldState,
	FWorldStatePair GoalState, 
	FGuid PlanID)
{
	AsyncTask(ENamedThreads::AnyBackgroundThreadNormalTask, 
		[this,
		Agent, 
		ActionSet, 
		WorldState, 
		GoalState, 
		PlanID]()
	{
		TArray<FAction> GeneratedPlan = GeneratePlan(Agent, ActionSet, WorldState, GoalState);

		AsyncTask(ENamedThreads::GameThread, [this, PlanID, GeneratedPlan]()
		{
				OnPlanningComplete.Broadcast(PlanID, GeneratedPlan);
		});
	});
}

TArray<FAction> UPlannerSubsystem::GeneratePlan(
	APlannerAIController* Agent, 
	const TArray<FAction>& ActionSet,
	TArray<FWorldStatePair> WorldState,
	FWorldStatePair GoalState)
{
	TArray<FAction> Plan = TArray<FAction>();
	TArray<FAction> OpenSet = TArray<FAction>();
	TArray<FAction> ClosedSet = TArray<FAction>();
	TArray<FAction> CurrentActionSet = TArray<FAction>();
	FGuid RootID = FGuid::NewGuid();
	int BestCost = INT_MAX;

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
		if (CheckAction.EffectsSatisfyCondition(GoalState))
		{
			CheckAction.CalculatedCost = CheckAction.ActionCost;
			CheckAction.UnSatisfiedConditions = CheckAction.Preconditions;
			CheckAction.ParentActionID = RootID;
			OpenSet.Add(CheckAction);

			// Handle the case of a one step plan. 
			// We don't want to return early in this case: Our heauristic functions (each step costs 1 + a satisfied condition is closer to the target) are currently very simple,
			// but we may want to implement a more sophisticated cost/closeness function in the future, so it's worth checking the remaining open nodes for now.
			if (CheckConditionsAgainstWorldState(CheckAction.UnSatisfiedConditions, WorldState))
			{
				if (CheckAndUpdateBestCost(CheckAction, BestCost))
				{
					Plan = ReconstructPlan(CheckActionID, OpenSet, GoalState, RootID);
				}
			}
		}
	}

	// While we have unexplored nodes which may lead back to the current worldstate
	while (!OpenSet.IsEmpty())
	{
		// Get the next action to explore from the open set
		FAction CurrentAction = OpenSet.Pop();


		// It's possible that a node is added to the open set and inherits unsatisfied conditions from a parent node whose inheritance, and therefore
		// inherited unsatisfied conditions, is later changed. This block checks the current action's unsatisfied conditions against the parent's unsatisfied conditions,
		// (which are updated in the case that the parent's inheritance changes) and adds any conditions that are missing, unless this action's effects specifically 
		// satisfy that condition. Obviously.
		// This might still leave open the possibility of an action being added to the open set which is no longer needed, but I don't think this is a fatal problem.
		// This is also where we'll calculate a node's cost before closing it, to ensure it's calculated from the parent correctly
		if (CurrentAction.ParentActionID != RootID)
		{
			FAction ParentAction = FetchActionByID(CurrentAction.ParentActionID, ClosedSet);
			
			CurrentAction.CalculatedCost = ParentAction.CalculatedCost + CurrentAction.ActionCost; // Set the new cost for the current action

			// Iterate over the parent action's unsat. conds
			for (FWorldStatePair InheritedCondition : ParentAction.UnSatisfiedConditions)
			{
				// if this action does not already contain a certain condition
				if (!CurrentAction.UnsatisfiedConditionsContain(InheritedCondition))
				{
					// add the missing condition to this actions unsat. conds, as long as this action's effects don't satisfy it 
					// (we know this action satisfies at least one of it's parents unsat. conds, or it wouldn't have been added to the open set by that parent)
					if (!CurrentAction.EffectsSatisfyCondition(InheritedCondition))
					{
						CurrentAction.UnSatisfiedConditions.Add(InheritedCondition);
					}
				}
			}
			AppendMapNonDestructive(CurrentAction.UnSatisfiedConditions, CurrentAction.Preconditions); //Append this action's own preconditions to the unsat. conds. list
		}

		ClosedSet.Add(CurrentAction);

		// Iterate over unsatisfied conditions stored on the current action (accumulated from previous actions explored on this path)
		for (auto UnSatCondition : CurrentAction.UnSatisfiedConditions)
		{
			// If this particular precondition is satisfied by the worldstate, we don't need to explore actions which also satisfy it, continue instead
			if (CheckSingleConditionAgainstWorldState(UnSatCondition, WorldState))
			{
				continue;
			}
			// Iterate over available actions
			for (FAction CheckAction : CurrentActionSet)
			{
				// Check if the action satisfies the condition we're currently considering.
				if (CheckAction.EffectsSatisfyCondition(UnSatCondition))
				{
					//ToDo: This is where we might spawn a ContextCheckActor, or now that I think of it, call a more general Context Checker. Either way, ignoring that step for now
					// The context check actors could pull double duty: check if an action is possible and check if it is effective

					// Handle the case of an action we've already closed (update cost and conditions if it would be less than the cost already stored on that action)
					if (ClosedSet.Contains(CheckAction))
					{
						if (CurrentAction.CalculatedCost + CheckAction.ActionCost < CheckAction.CalculatedCost)
						{
							CheckAction.CalculatedCost = CurrentAction.CalculatedCost + CheckAction.ActionCost; // Set the new cost for the check action
							CheckAction.ParentActionID = CurrentAction.ActionID;
							CheckAction.UnSatisfiedConditions = CurrentAction.UnSatisfiedConditions; // Set the new set of unsat. conds. for the check action (inherited from current)
							AppendMapNonDestructive(CheckAction.UnSatisfiedConditions, CheckAction.Preconditions); // Add the action's own preconditions back to the unsat. list
							CheckAction.UnSatisfiedConditions.Remove(UnSatCondition);
						}
					}
					else
					{					
						CheckAction.ParentActionID = CurrentAction.ActionID;
						OpenSet.Add(CheckAction);
					}
				}
			}
		}

		if (CheckConditionsAgainstWorldState(CurrentAction.UnSatisfiedConditions, WorldState))
		{
			if (CheckAndUpdateBestCost(CurrentAction, BestCost))
			{
				Plan = ReconstructPlan(CurrentAction.ActionID, ClosedSet, GoalState, RootID);
			}
		}
	}

	return Plan;
}

TArray<FAction> UPlannerSubsystem::ReconstructPlan(
	FGuid FirstActionID, 
	TArray<FAction> InActionSet, 
	FWorldStatePair GoalState,
	FGuid RootID)
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

FAction UPlannerSubsystem::FetchActionByID(FGuid ActionID, TArray<FAction>& CurrentActionSet)
{
	FAction EmptyAction = FAction();
	for (FAction Action : CurrentActionSet)
	{
		if (Action.ActionID == ActionID)
		{
			return Action;
		}
	}
	
	UE_LOG(LogPlanner, Error, TEXT("No Action Found for given ActionID! Likely a fatal error!"));

	return EmptyAction;
}

bool UPlannerSubsystem::CheckConditionsAgainstWorldState(const TArray<FWorldStatePair>& InUnsatisfiedConditions, const TArray<FWorldStatePair>& InWorldState)
{
	for (auto UnSatCondition : InUnsatisfiedConditions)
	{
		for (auto WorldStateCondition : InWorldState)
		{
			if (!UnSatCondition.IsCompatible(WorldStateCondition)) return false;
		}
	}

	return true;
}

bool UPlannerSubsystem::CheckSingleConditionAgainstWorldState(FWorldStatePair Condition, const TArray<FWorldStatePair>& InWorldState)
{
	for (auto WorldStateCondition : InWorldState)
	{
		if (!WorldStateCondition.IsCompatible(Condition)) return false;
	}
	
	return true;
}

bool UPlannerSubsystem::CheckAndUpdateBestCost(FAction Action, int& OutBestCost)
{
	// If the cost set by GeneratePlan so far is not less than the current best known cost, return without reconstructing a plan.
	if (Action.CalculatedCost < OutBestCost)
	{
		OutBestCost = Action.CalculatedCost;
		return true;
	}
	else
	{
		return false;
	}
}

void UPlannerSubsystem::AppendMapNonDestructive(TArray<FWorldStatePair>& TargetMap, TArray<FWorldStatePair>& SourceMap)
{
	for (const auto& Pair : SourceMap)
	{
		TargetMap.Add(Pair);
	}
}

void UPlannerSubsystem::AppendConditionsNoDuplicates(TArray<FWorldStatePair>& TargetArray, TArray<FWorldStatePair>& SourceArray)
{
	for (const FWorldStatePair& SourcePair : SourceArray)
	{
		for (const FWorldStatePair& TargetPair : TargetArray)
		{
			if (SourcePair.Key != TargetPair.Key)
			{
				TargetArray.Add(SourcePair);
			}
		}
	}
}




