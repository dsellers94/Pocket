// Copyright Daniel Sellers 2024.

#pragma once

#include "CoreMinimal.h"
#include "Item/ItemBase.h"
#include "FoodStashBase.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnStatusChanged);

UCLASS()
class POCKET_API AFoodStashBase : public AItemBase
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintAssignable)
	FOnStatusChanged OnStatusChanged;

	UPROPERTY(BlueprintReadOnly)
	bool StashOccupied = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float ConsumeFoodTime = 15.f;

	UFUNCTION(BlueprintCallable)
	void DeliverFood();

	UFUNCTION(BlueprintCallable)
	void ConsumeFood();

protected:

	UPROPERTY()
	FTimerHandle ConsumeFoodTimerHandle;

private:

	virtual void BeginPlay() override;

	
};
