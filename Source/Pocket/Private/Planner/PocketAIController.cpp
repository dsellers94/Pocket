// Copyright Daniel Sellers 2024.


#include "Planner/PocketAIController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "NPCCharacterBase.h"
#include "Pocket/Pocket.h"

void APocketAIController::BeginPlay()
{
	Super::BeginPlay();

	ControlledNPC = Cast<ANPCCharacterBase>(GetPawn());
	if (!IsValid(ControlledNPC))
	{
		UE_LOG(LogPocket, Error, TEXT("Failed to get Controlled NPC"));
		return;
	}
	BehaviorTree = ControlledNPC->BehaviorTree;

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

	GetWorldTimerManager().SetTimer(
		InfoUpdateTimerHandle,
		this,
		&ThisClass::UpdateInfo,
		InfoUpdateTime,
		true);

	SetGetNextGoalFlag();
}

void APocketAIController::SetGetNextGoalFlag()
{
	if (IsValid(BlackboardComponent))
	{
		BlackboardComponent->SetValueAsBool("GettingNextGoal", true);
	}
}

void APocketAIController::RemoveGetNextGoalFlag()
{
	if (IsValid(BlackboardComponent))
	{
		BlackboardComponent->SetValueAsBool("GettingNextGoal", false);
	}
}

void APocketAIController::UpdateInfo()
{
}

