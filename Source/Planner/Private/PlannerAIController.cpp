// Copyright Daniel Sellers 2024.


#include "PlannerAIController.h"
#include "Engine/StreamableManager.h"
#include "Engine/AssetManager.h"
#include "Planner/Planner.h"
#include "PlannerComponent.h"


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

	ActionSet = ControlledPlannerComponent->ActionSet;
}


