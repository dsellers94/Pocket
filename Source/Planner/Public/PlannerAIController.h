// Copyright Daniel Sellers 2024.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "PlannerStructs.h"
#include "PlannerAIController.generated.h"

/**
 * 
 */
UCLASS()
class PLANNER_API APlannerAIController : public AAIController
{
	GENERATED_BODY()

protected:

	UPROPERTY()
	TMap<EWorldStateKey, bool> WorldState;


	
};
