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
		&ThisClass::CheckPriorityGoal,
		GoalUpdateFrequency,
		true);
}

void UPlannerComponent::CheckPriorityGoal()
{
	if (!IsValid(AIController))
	{
		UE_LOG(LogPlanner, Error, TEXT("PlannerComponent: AIController not valid when selecting goal"));
		return;
	}

	int HighestPriority = 0;
	AIController->RequestWorldState();
	TArray<FWorldStatePair> WorldState = AIController->WorldState;

	for (const FGoal Goal : Goals)
	{
		if (!CheckGoalAgainstWorldState(WorldState, Goal)) // If goal is not satisfied by WorldState
		{
			if (Goal.Priority > HighestPriority)
			{
				HighestPriority = Goal.Priority;
				if (SelectedGoal != Goal)
				{
					OnSelectedGoalChanged.Broadcast();
				}
				SelectedGoal = Goal;
			}
		}
	}


}

bool UPlannerComponent::CheckGoalAgainstWorldState(TArray<FWorldStatePair> InWorldState, FGoal InGoal)
{
	for (FWorldStatePair Pair : InWorldState)
	{
		if (Pair.Satisfies(InGoal.GoalState)) return true;
	}
	return false;
}




