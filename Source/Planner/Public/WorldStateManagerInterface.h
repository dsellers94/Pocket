// Copyright Daniel Sellers 2024.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "PlannerStructs.h"
#include "WorldStateManagerInterface.generated.h"

class APlannerAIController;

UINTERFACE(MinimalAPI)
class UWorldStateManagerInterface : public UInterface
{
	GENERATED_BODY()
};


class PLANNER_API IWorldStateManagerInterface
{
	GENERATED_BODY()

public: 

	UFUNCTION(BlueprintNativeEvent)
	TArray<FWorldStatePair> RequestWorldState(APlannerAIController* Agent);

};

