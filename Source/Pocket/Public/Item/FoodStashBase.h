// Copyright Daniel Sellers 2024.

#pragma once

#include "CoreMinimal.h"
#include "Item/ItemBase.h"
#include "FoodStashBase.generated.h"

/**
 * 
 */
UCLASS()
class POCKET_API AFoodStashBase : public AItemBase
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintReadWrite)
	bool StashOccupied = false;

	
};
