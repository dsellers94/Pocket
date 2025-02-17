// Copyright Daniel Sellers 2024.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
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
	TMap<FName, bool> RequestWorldState(APlannerAIController* Agent);

};

