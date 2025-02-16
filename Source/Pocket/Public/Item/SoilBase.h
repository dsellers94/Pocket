// Copyright Daniel Sellers 2024.

#pragma once

#include "CoreMinimal.h"
#include "Item/ItemBase.h"
#include "SoilBase.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnFoodReady);

UCLASS()
class POCKET_API ASoilBase : public AItemBase
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintAssignable)
	FOnFoodReady OnFoodReady;

	UPROPERTY()
	FTimerHandle FoodGrowTimerHandle;

	UPROPERTY(EditAnywhere)
	float FoodGrowTime = 5.0;

	UPROPERTY(EditAnywhere)
	int MaxFoodToGrow = 2;

	UPROPERTY(BlueprintReadOnly)
	bool IsGrowingFood = false;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AItemBase> FoodClass;

	UFUNCTION(BlueprintCallable)
	void AcceptSeeds();

protected:

	UPROPERTY()
	int FoodGrown = 0;

	UFUNCTION()
	void GrowFood();

};
