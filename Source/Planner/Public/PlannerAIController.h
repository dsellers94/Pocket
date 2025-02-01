// Copyright Daniel Sellers 2024.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "PlannerStructs.h"
#include "ActionSetData.h"
#include "WorldStateManagerInterface.h"
#include "PlannerAIController.generated.h"

class UPlannerComponent;
class AActionExecutionActor;

UCLASS()
class PLANNER_API APlannerAIController : public AAIController
{
	GENERATED_BODY()

public: 

	UPROPERTY(VisibleAnywhere)
	TScriptInterface<IWorldStateManagerInterface> WorldStateManager;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TMap<FName, bool> WorldState = TMap<FName, bool>();

	UFUNCTION(BlueprintCallable)
	void RequestPlan(FName GoalKey, bool GoalValue);

	UPROPERTY()
	FGuid CurrentPlanID;

	UPROPERTY(EditAnywhere, BlueprintreadOnly)
	bool bSynchronousPlanningMode = false;

	UFUNCTION()
	void OnPlanningComplete(FGuid PlanID, TArray<FAction> Plan);

	UFUNCTION(BlueprintCallable)
	void RequestWorldState();

	UFUNCTION(BlueprintCallable)
	void ExecuteNextAction();

	UFUNCTION(BlueprintCallable)
	void GetNextGoal();

	UFUNCTION(BlueprintCallable)
	void PrintActionSet();

	UFUNCTION(BlueprintCallable)
	void PrintCurrentPlan();


protected:

	UPROPERTY()
	TArray<FAction> ActionSet = TArray<FAction>();

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

	UFUNCTION()
	void OnExecutionActorLoaded();


protected:
	
	virtual void OnPossess(APawn* InPawn) override;

	UFUNCTION()
	void GenerateActionSetFromRows();

private:

	virtual void BeginPlay() override;
	
};
