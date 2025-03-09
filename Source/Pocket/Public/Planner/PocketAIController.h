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

protected:

private:

	virtual void BeginPlay() override;
	
};
