// Copyright Daniel Sellers 2024.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ActionSetData.h"
#include "PlannerStructs.h"
#include "PlannerComponent.generated.h"

class APlannerAIController;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSelectedGoalChanged, FGoal, SelectedGoal);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PLANNER_API UPlannerComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UPlannerComponent();

	UPROPERTY()
	FOnSelectedGoalChanged OnSelectedGoalChanged;

	UPROPERTY()
	TObjectPtr<APlannerAIController> AIController = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<FDataTableRowHandle> ActionRows;

	UPROPERTY()
	TArray<FDataTableRowHandle> GoalRows;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<FGoal> Goals;

	UPROPERTY(EditDefaultsOnly)
	float GoalUpdateFrequency = 0.1;

	UPROPERTY()
	FGoal SelectedGoal = FGoal();

	UFUNCTION()
	void InitializeGoalSelection(APlannerAIController* NewController);

	UFUNCTION()
	void CheckPriorityGoal();

	UFUNCTION()
	bool CheckGoalAgainstWorldState(TArray<FWorldStatePair> InWorldState, FGoal InGoal);

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;


protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	void GenerateGoalSetFromRows();

private:

	UPROPERTY()
	FTimerHandle GoalUpdateTimerHandle;

};
