// Copyright Daniel Sellers 2024.


#include "PlannerComponent.h"
#include "Planner/Planner.h"
#include "PlannerAIController.h"

UPlannerComponent::UPlannerComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}


void UPlannerComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UPlannerComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

void UPlannerComponent::InitializeGoalSelection(APlannerAIController* NewController)
{
	AIController = NewController;

	UE_LOG(LogPlanner, Warning, TEXT("AIController: %s"), *AIController->GetName());

	GetWorld()->GetTimerManager().SetTimer(
		GoalUpdateTimerHandle,
		this,
		&ThisClass::SelectGoal,
		GoalUpdateFrequency,
		true);
}

void UPlannerComponent::SelectGoal()
{
	if (!IsValid(AIController))
	{
		UE_LOG(LogPlanner, Error, TEXT("PlannerComponent: AIController not valid when selecting goal"));
		return;
	}

	int HighestPriority = 0;
	AIController->RequestWorldState();
	TArray<FWorldStatePair> WorldState = AIController->WorldState;

	for (const FDataTableRowHandle RowHandle : GoalRows)
	{
		const FGoalRow* FoundRow = RowHandle.GetRow<FGoalRow>(__FUNCTION__);
		if (FoundRow == nullptr)
		{
			UE_LOG(LogPlanner, Error, TEXT("PlannerComponent: Failed to get GoalRow data from GoalRow Handle"));
			continue;
		}

		FGoal Goal = FoundRow->Goal;
		if (Goal.Priority > HighestPriority)
		{
			// Check if this goal is currently satisfied by the worldstate.
		}
	}
}



