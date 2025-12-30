// Copyright Daniel Sellers 2024.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "PlannerStructs.h"
#include "ActionSetData.h"
#include "WorldStateManagerInterface.h"
#include "PlannerAIController.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnGettingNextGoal);

class UPlannerComponent;
class AActionExecutionActor;
class UPlannerSubsystem;

// TODO: We want RequestWorldState to also pass the FullActionSet to the World State Manager, and get an (async returned) available action set which includes adjusted action costs.

// The key part (and most tedious to implement given the way I've set this up) is that the action costs need to be determined by async loaded ContextCheckActors per-action.
// So that doesn't really need to happen in the world state manager, but maybe that's a better place for it because it would already have world information available.

// SO! We should set this class up to async request a modified action set from the World State Manager, and migrate the code that exists here for async loading context check actors
// to the world state manager, and expand it so that it calulcates a numeric cost as well as determing availability. This class will wait for the returned AvailableActionSet
// (including cost determinations) before proceeding to request a plan from the Planner Subsystem (whih is already equipped to select the best plan based on the provided ActionCost
// of each action in the set passed to it.

// Because we will now be requesting the world state (and adjusted action set) from the world state manager asynchronously, we will need to implement and GUID event system like 
// we have in the planner subsystem itself, to make sure this Agent is able to recognize it's own World State and AvailableActionSet return values when they come back
// from the WorlsStateManager

UCLASS()
class PLANNER_API APlannerAIController : public AAIController
{
	GENERATED_BODY()

public: 

	UPROPERTY(BlueprintAssignable)
	FOnGettingNextGoal OnGettingNextGoal;

	UPROPERTY(VisibleAnywhere)
	TScriptInterface<IWorldStateManagerInterface> WorldStateManager;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<FWorldStatePair> WorldState = TArray<FWorldStatePair>();

	UFUNCTION(BlueprintCallable)
	void RequestPlan(FWorldStatePair GoalState);

	UPROPERTY()
	FGuid CurrentPlanID;

	UPROPERTY(EditAnywhere, BlueprintreadOnly)
	bool bSynchronousPlanningMode = false;

	UPROPERTY(EditDefaultsOnly)
	float ActionEvaluationRate = 3.f;

	UFUNCTION()
	void OnPlanningComplete(FGuid PlanID, TArray<FAction> Plan);

	UFUNCTION(BlueprintCallable)
	void RequestWorldState();

	UFUNCTION(BlueprintCallable)
	void ExecuteNextAction();

	UFUNCTION(BlueprintCallable)
	void GetNextGoal();

	UFUNCTION(BlueprintCallable)
	void CancelPlan();

	UFUNCTION(BlueprintCallable)
	void OnActionFailed();

	UFUNCTION(BlueprintCallable)
	void PrintActionSet();

	UFUNCTION(BlueprintCallable)
	void PrintCurrentPlan();

	UFUNCTION(BlueprintCallable)
	void PrintCurrentAction();

protected:

	UPROPERTY()
	TObjectPtr<UPlannerSubsystem> PlannerSubsystem = nullptr;

	UPROPERTY()
	TArray<FAction> FullActionSet = TArray<FAction>();

	UPROPERTY()
	TArray<FAction> AvailableActionSet = TArray<FAction>();

	UPROPERTY()
	TArray<FDataTableRowHandle> ActionRows;

	UPROPERTY()
	TObjectPtr<UPlannerComponent> ControlledPlannerComponent = nullptr;

	UPROPERTY()
	TArray<FAction> CurrentPlan = TArray<FAction>();

	UPROPERTY()
	int CurrentActionCount = 0;

	UPROPERTY()
	int CurrentActionIndex = 0;

	UPROPERTY()
	TSoftClassPtr<AActionExecutionActor> SoftExecutionActor = nullptr;

	UPROPERTY()
	TObjectPtr<AActionExecutionActor> CurrentExecutionActor = nullptr;

	UPROPERTY()
	TSoftClassPtr<AContextCheckActor> SoftContextCheckActor = nullptr;

	UPROPERTY()
	TObjectPtr<AContextCheckActor> CurrentContextCheckActor = nullptr;

	UPROPERTY()
	FGoal CurrentGoal = FGoal();

	UPROPERTY()
	FTimerHandle ActionEvaluationTimer;

	UPROPERTY()
	bool StillEvaluating = false;

	UPROPERTY()
	int ActionEvaluationIndex = 0;

	UFUNCTION()
	void OnExecutionActorLoaded();

	UFUNCTION()
	void OnSelectedGoalChanged(FGoal SelectedGoal);
	
	virtual void OnPossess(APawn* InPawn) override;

	UFUNCTION()
	void GenerateActionSetFromRows();

	UFUNCTION()
	void EvaluateActions();

	UFUNCTION()
	void EvaluateNextAction();

	UFUNCTION()
	void UpdateAvailableActionSet();

	UFUNCTION()
	void OnContextCheckActorLoaded();

	virtual void BeginPlay() override;
	
};
