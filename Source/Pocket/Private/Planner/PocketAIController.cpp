// Copyright Daniel Sellers 2024.


#include "Planner/PocketAIController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Pocket/Pocket.h"

void APocketAIController::BeginPlay()
{
	if (!IsValid(BehaviorTree))
	{
		UE_LOG(LogPocket, Error, TEXT("No valid behavior tree found"));
		return;
	}
	RunBehaviorTree(BehaviorTree);
	BlackboardComponent = GetBlackboardComponent();
	if (!IsValid(BlackboardComponent))
	{
		UE_LOG(LogPocket, Error, TEXT("Failed to get blackboard component"));
		return;
	}
	OnGettingNextGoal.AddDynamic(this, &ThisClass::SetGetNextGoalFlag);
}

void APocketAIController::SetGetNextGoalFlag()
{
	if (IsValid(BlackboardComponent))
	{
		BlackboardComponent->SetValueAsBool("GettingNextGoal", true);
	}
}

