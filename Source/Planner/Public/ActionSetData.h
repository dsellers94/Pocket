// Copyright Daniel Sellers 2024.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "PlannerStructs.h"
#include "ActionSetData.generated.h"


UCLASS()
class PLANNER_API UActionSetData : public UDataAsset
{
	GENERATED_BODY()

public: 

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<FAction> ActionSet;
	
};
