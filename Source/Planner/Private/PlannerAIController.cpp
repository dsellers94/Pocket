// Copyright Daniel Sellers 2024.


#include "PlannerAIController.h"
#include "Engine/StreamableManager.h"
#include "Engine/AssetManager.h"
#include "Planner/Planner.h"
#include "PlannerComponent.h"
#include "Kismet/GameplayStatics.h"
#include "PlannerSubsystem.h"
#include "WorldStateManagerInterface.h"
#include "ActionExecutionActor.h"
#include "ContextCheckActor.h"
UE_DISABLE_OPTIMIZATION

void APlannerAIController::BeginPlay()
{
	Super::BeginPlay();

	TArray<AActor*> Actors = TArray<AActor*>();

	UGameplayStatics::GetAllActorsWithInterface(this, UWorldStateManagerInterface::StaticClass(), Actors);

	if (Actors.Num() == 0)
	{
		UE_LOG(LogPlanner, Error, TEXT("No WorldStateManager found in level, likely a fatal error"));
	}

	if (Actors.IsValidIndex(0))
	{
		if (Actors[0]->GetClass()->ImplementsInterface(UWorldStateManagerInterface::StaticClass()))
		{
			WorldStateManager.SetObject(Actors[0]);
		}
	}

	if (!IsValid(WorldStateManager.GetObject()))
	{
		UE_LOG(LogPlanner, Error, TEXT("Invalid WorldStateManager, likely a fatal error"));
	}

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

	UGameInstance* GameInstance = UGameplayStatics::GetGameInstance(GetWorld());
	if (!IsValid(GameInstance))
	{
		UE_LOG(LogPlanner, Error, TEXT("Failed to get GameInstance while requesting plan"));
		return;
	}

	PlannerSubsystem = GameInstance->GetSubsystem<UPlannerSubsystem>();

	if (!IsValid(PlannerSubsystem))
	{
		UE_LOG(LogPlanner, Error, TEXT("Failed to get PlannerSubsystem on Possess"));
		return;
	}

	PlannerSubsystem->OnPlanningComplete.AddDynamic(this, &ThisClass::OnPlanningComplete);

	ActionRows = ControlledPlannerComponent->ActionRows;

	ControlledPlannerComponent->OnSelectedGoalChanged.AddDynamic(this, &ThisClass::OnSelectedGoalChanged);

	ControlledPlannerComponent->InitializeGoalSelection(this);

	GenerateActionSetFromRows();

	GetWorld()->GetTimerManager().SetTimer(
		ActionEvaluationTimer,
		this,
		&ThisClass::EvaluateActions,
		ActionEvaluationRate,
		true);
}

void APlannerAIController::GenerateActionSetFromRows()
{
	FullActionSet.Empty();
	for (const FDataTableRowHandle& Row : ActionRows)
	{
		const FActionRow* FoundRow = Row.GetRow<FActionRow>(__FUNCTION__);
		if (FoundRow == nullptr)
		{
			UE_LOG(LogPlanner, Error, TEXT("PlannerAIController: Failed to get action data from Action Row Handle"));
			return;
		}
		FullActionSet.Add(FoundRow->Action);
	}
}

void APlannerAIController::RequestPlan(FWorldStatePair GoalState)
{	
	if (!IsValid(PlannerSubsystem))
	{
		UE_LOG(LogPlanner, Error, TEXT("Failed to get PlannerSubsystem while requesting plan"));
		return;
	}

	CurrentPlanID = FGuid::NewGuid();

	if (bSynchronousPlanningMode)
	{
		TArray<FAction> NewPlan = PlannerSubsystem->GeneratePlan(
			this,
			AvailableActionSet,
			WorldState,
			GoalState);

		OnPlanningComplete(CurrentPlanID, NewPlan);
	}
	else
	{
		PlannerSubsystem->RequestPlan(this, AvailableActionSet, WorldState, GoalState, CurrentPlanID);
	}
}

void APlannerAIController::OnPlanningComplete(FGuid PlanID, TArray<FAction> Plan)
{
	if (PlanID != CurrentPlanID) return;

	CurrentPlan.Empty();
	CurrentPlan = Plan;

	if (CurrentPlan.Num() == 0)
	{
		UE_LOG(LogPlanner, Error, TEXT("No Plan Returned"));
		GetNextGoal();
		return;
	}

	CurrentActionCount = CurrentPlan.Num();

	CurrentActionIndex = 0;

	ExecuteNextAction();
}

void APlannerAIController::RequestWorldState()
{
	if (IsValid(WorldStateManager.GetObject()))
	{
		WorldState = IWorldStateManagerInterface::Execute_RequestWorldState(WorldStateManager.GetObject(), this);
	}
}

void APlannerAIController::ExecuteNextAction()
{
	// Cleanup the last ActionExecutionActor if one exists
	if (IsValid(CurrentExecutionActor))
	{
		CurrentExecutionActor->Destroy();
	}
	CurrentExecutionActor = nullptr;
	
	if (CurrentActionIndex >= CurrentPlan.Num() || CurrentPlan.Num() == 0)
	{
		UE_LOG(LogPlanner, Warning, TEXT("Plan Completed"));
		GetNextGoal();
		return;
	}
	FAction CurrentAction = CurrentPlan[CurrentActionIndex];

	SoftExecutionActor = CurrentAction.ActionExecutionActor;

	FStreamableDelegate Delegate;
	Delegate.BindUObject(this, &APlannerAIController::OnExecutionActorLoaded);
	UAssetManager::GetStreamableManager()
		.RequestAsyncLoad(SoftExecutionActor.ToSoftObjectPath(), Delegate);

	CurrentActionIndex++; 
}

void APlannerAIController::GetNextGoal()
{
	UE_LOG(LogPlanner, Warning, TEXT("Getting Next Goal"));
	OnGettingNextGoal.Broadcast();
	RequestWorldState();
	RequestPlan(CurrentGoal.GoalState);
}

void APlannerAIController::CancelPlan()
{
	if (IsValid(CurrentExecutionActor))
	{
		CurrentExecutionActor->DeclareActionFailed();
	}
}


void APlannerAIController::OnExecutionActorLoaded()
{
	UClass* ExecutionActorClass = SoftExecutionActor.Get();
	AActor* Actor = GetWorld()->SpawnActor(ExecutionActorClass);
	
	if (!IsValid(Actor))
	{
		UE_LOG(LogPlanner, Error, TEXT("Failed to spawn ActionExecutionActor"));
		return;
	}

	CurrentExecutionActor = Cast<AActionExecutionActor>(Actor);

	CurrentExecutionActor->CallingAgent = this;
	CurrentExecutionActor->OnActionComplete.AddDynamic(this, &APlannerAIController::ExecuteNextAction);
	CurrentExecutionActor->OnActionFailed.AddDynamic(this, &APlannerAIController::OnActionFailed);
	CurrentExecutionActor->Execute();

	UAssetManager::GetStreamableManager().Unload(SoftExecutionActor.ToSoftObjectPath());
}

void APlannerAIController::OnSelectedGoalChanged(FGoal SelectedGoal)
{
	CancelPlan();
	RequestWorldState();
	RequestPlan(SelectedGoal.GoalState);
	CurrentGoal = SelectedGoal;
}

void APlannerAIController::OnActionFailed()
{
	if (IsValid(CurrentExecutionActor))
	{
		CurrentExecutionActor->Destroy();
	}
	CurrentExecutionActor = nullptr;
	GetNextGoal();
}

void APlannerAIController::EvaluateActions()
{
	if (StillEvaluating) return;

	StillEvaluating = true;

	CurrentActionIndex = 0;

	EvaluateNextAction();
}

void APlannerAIController::EvaluateNextAction()
{
	if (IsValid(CurrentContextCheckActor))
	{
		CurrentContextCheckActor->Destroy();
	}
	CurrentContextCheckActor = nullptr;

	if (!FullActionSet.IsValidIndex(ActionEvaluationIndex))
	{
		StillEvaluating = false;
		UpdateAvailableActionSet();
		return;
	}

	FAction CurrentAction = FullActionSet[ActionEvaluationIndex];

	if (!CurrentAction.bNeedsContextCheck)
	{
		ActionEvaluationIndex++;
		UpdateAvailableActionSet();
		EvaluateNextAction();
		return;
	}

	SoftContextCheckActor = CurrentAction.ContextCheckActor;

	FStreamableDelegate Delegate;
	Delegate.BindUObject(this, &APlannerAIController::OnContextCheckActorLoaded);
	UAssetManager::GetStreamableManager()
		.RequestAsyncLoad(SoftContextCheckActor.ToSoftObjectPath(), Delegate);

	ActionEvaluationIndex++;
}

void APlannerAIController::UpdateAvailableActionSet()
{
	AvailableActionSet.Empty();
	for (FAction& Action : FullActionSet)
	{
		if (Action.bActionAvailable)
		{
			AvailableActionSet.Add(Action);
		}
	}
}

void APlannerAIController::OnContextCheckActorLoaded()
{
	UClass* ContextCheckActorClass = SoftContextCheckActor.Get();
	AActor* Actor = GetWorld()->SpawnActor(ContextCheckActorClass);

	if (!IsValid(Actor))
	{
		UE_LOG(LogPlanner, Error, TEXT("Failed to spawn ContextcheckActor"));
		return;
	}

	CurrentContextCheckActor = Cast<AContextCheckActor>(Actor);

	FullActionSet[ActionEvaluationIndex].bActionAvailable = CurrentContextCheckActor->CheckValidity(this);

	UAssetManager::GetStreamableManager().Unload(SoftExecutionActor.ToSoftObjectPath());

	EvaluateNextAction();
}

void APlannerAIController::PrintActionSet()
{
	for (FAction Action : FullActionSet)
	{
		FString ActionString = Action.ToString();
		UE_LOG(LogPlanner, Warning, TEXT("%s"), *ActionString);
	}
}

void APlannerAIController::PrintCurrentPlan()
{
	UE_LOG(LogPlanner, Warning, TEXT("----------"));
	for (FAction Action : CurrentPlan)
	{
		UE_LOG(LogPlanner, Warning, TEXT("%s"), *Action.ActionName.ToString());
	}
}

void APlannerAIController::PrintCurrentAction()
{
	if (!IsValid(CurrentExecutionActor))
	{
		UE_LOG(LogPlanner, Warning, TEXT("No Valid Exceution Actor"));
		return;
	}
	FString CurrentActionName = CurrentExecutionActor->GetActorNameOrLabel();
	UE_LOG(LogPlanner, Warning, TEXT("Current Action: %s"), *CurrentActionName);
}

UE_ENABLE_OPTIMIZATION