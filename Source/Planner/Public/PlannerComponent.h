// Copyright Daniel Sellers 2024.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ActionSetData.h"
#include "PlannerStructs.h"
#include "PlannerComponent.generated.h"

class APlannerAIController;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PLANNER_API UPlannerComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UPlannerComponent();

	UPROPERTY()
	TObjectPtr<APlannerAIController> AIController = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<FDataTableRowHandle> ActionRows;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<FDataTableRowHandle> GoalRows;

	UPROPERTY(EditDefaultsOnly)
	float GoalUpdateFrequency = 0.1;

	UPROPERTY()
	FGoal SelectedGoal = FGoal();

	UFUNCTION()
	void InitializeGoalSelection(APlannerAIController* NewController);

	UFUNCTION()
	void SelectGoal();

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;


protected:
	virtual void BeginPlay() override;

private:

	UPROPERTY()
	FTimerHandle GoalUpdateTimerHandle;

};
