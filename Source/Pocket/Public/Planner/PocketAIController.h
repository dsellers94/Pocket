// Copyright Daniel Sellers 2024.

#pragma once

#include "CoreMinimal.h"
#include "PlannerAIController.h"
#include "PocketAIController.generated.h"

class UBehaviorTree;
class UBlackboardComponent;
class ANPCCharacterBase;

UCLASS()
class POCKET_API APocketAIController : public APlannerAIController
{
	GENERATED_BODY()

public:	

	UPROPERTY()
	TObjectPtr<ANPCCharacterBase> ControlledNPC = nullptr;

	UFUNCTION(BlueprintCallable)
	void SetGetNextGoalFlag();

	UFUNCTION(BlueprintCallable)
	void RemoveSetNextGoalFlag();

	UPROPERTY(EditAnywhere)
	float InfoUpdateTime = 0.1;

protected:

	UPROPERTY()
	FTimerHandle InfoUpdateTimerHandle;

	UFUNCTION()
	virtual void UpdateInfo();

private:

	UPROPERTY()
	TObjectPtr<UBehaviorTree> BehaviorTree = nullptr;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UBlackboardComponent> BlackboardComponent = nullptr;

	virtual void BeginPlay() override;
	
};
