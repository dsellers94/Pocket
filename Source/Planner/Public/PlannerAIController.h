// Copyright Daniel Sellers 2024.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "PlannerStructs.h"
#include "ActionSetData.h"
#include "PlannerAIController.generated.h"

class UPlannerComponent;

UCLASS()
class PLANNER_API APlannerAIController : public AAIController
{
	GENERATED_BODY()

public: 

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TMap<FName, bool> WorldState = TMap<FName, bool>();

	UFUNCTION(BlueprintCallable)
	void RequestPlan(FName GoalKey, bool GoalValue);

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
	TArray<FAction> CurrentPlan;


protected:
	
	virtual void OnPossess(APawn* InPawn) override;

	UFUNCTION()
	void GenerateActionSetFromRows();

private:

	virtual void BeginPlay() override;
	
};
