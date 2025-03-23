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

	GenerateGoalSetFromRows();
}

void UPlannerComponent::GenerateGoalSetFromRows()
{
	Goals.Empty();
	for (const FDataTableRowHandle& Row : GoalRows)
	{
		const FGoalRow* FoundRow = Row.GetRow<FGoalRow>(__FUNCTION__);
		if (FoundRow == nullptr)
		{
			UE_LOG(LogPlanner, Error, TEXT("PlannerAIController: Failed to get goal data from Goal Row Handle"));
			return;
		}
		Goals.Add(FoundRow->Goal);
	}
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

	FGoal CandidateGoal = FGoal();
	for (const FGoal& Goal : Goals)
	{
		if (Goal.Priority >= HighestPriority)
		{
			if (!CheckGoalAgainstWorldState(WorldState, Goal))
			{
				HighestPriority = Goal.Priority;
				CandidateGoal = Goal;
			}
		}
	}
	if (SelectedGoal != CandidateGoal)
	{
		OnSelectedGoalChanged.Broadcast(CandidateGoal);
		UE_LOG(LogPlanner, Warning, TEXT("Selected Goal: %s"), *CandidateGoal.GoalState.Key.ToString());
	}
	SelectedGoal = CandidateGoal;

}

bool UPlannerComponent::CheckGoalAgainstWorldState(TArray<FWorldStatePair> InWorldState, FGoal InGoal)
{
	for (FWorldStatePair Pair : InWorldState)
	{
		if (Pair.Satisfies(InGoal.GoalState)) return true;
	}
	return false;
}




