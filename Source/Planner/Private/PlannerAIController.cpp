// Copyright Daniel Sellers 2024.


#include "PlannerAIController.h"
#include "Engine/StreamableManager.h"
#include "Engine/AssetManager.h"
#include "Planner/Planner.h"
#include "PlannerComponent.h"
#include "Kismet/GameplayStatics.h"
#include "PlannerSubsystem.h"


void APlannerAIController::BeginPlay()
{
	Super::BeginPlay();


}

void APlannerAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	if (!IsValid(GetPawn()))
	{
		UE_LOG(LogPlanner, Error, TEXT("Failed to get pawn in OnPossess"));
		return;
	}

	ControlledPlannerComponent = GetPawn()->GetComponentByClass<UPlannerComponent>();

	if (!IsValid(ControlledPlannerComponent))
	{
		UE_LOG(LogPlanner, Error, TEXT("Failed to get planner component in OnPossess"));
		return;
	}

	ActionRows = ControlledPlannerComponent->ActionRows;

	GenerateActionSetFromRows();
}

void APlannerAIController::GenerateActionSetFromRows()
{
	for (const FDataTableRowHandle& Row : ActionRows)
	{
		const FActionRow* FoundRow = Row.GetRow<FActionRow>(__FUNCTION__);
		if (FoundRow == nullptr)
		{
			UE_LOG(LogPlanner, Error, TEXT("PlannerAIController: Failed to get action data from Action Row Handle"));
			return;
		}
		ActionSet.Add(FoundRow->Action);
	}
}

void APlannerAIController::RequestPlan(FName GoalKey, bool GoalValue)
{
	UGameInstance* GameInstance = UGameplayStatics::GetGameInstance(GetWorld());
	if (!IsValid(GameInstance))
	{
		UE_LOG(LogPlanner, Error, TEXT("Failed to get GameInstance while requesting plan"));
		return;
	}
	
	UPlannerSubsystem* PlannerSubsystem = GameInstance->GetSubsystem<UPlannerSubsystem>();
	
	if (!IsValid(PlannerSubsystem))
	{
		UE_LOG(LogPlanner, Error, TEXT("Failed to get PlannerSubsystem while requesting plan"));
		return;
	}

	CurrentPlan = PlannerSubsystem->GeneratePlan(this, ActionSet, WorldState, GoalKey, GoalValue);
}


void APlannerAIController::PrintActionSet()
{
	for (FAction Action : ActionSet)
	{
		FString ActionString = Action.ToString();
		UE_LOG(LogPlanner, Warning, TEXT("%s"), *ActionString);
	}
}

void APlannerAIController::PrintCurrentPlan()
{
	for (FAction Action : ActionSet)
	{
		UE_LOG(LogPlanner, Warning, TEXT("%s"), *Action.ActionName.ToString());
	}
}

