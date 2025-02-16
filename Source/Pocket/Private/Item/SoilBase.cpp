// Copyright Daniel Sellers 2024.


#include "Item/SoilBase.h"
#include "Kismet/GameplayStatics.h"
#include "Pocket/Pocket.h"

void ASoilBase::AcceptSeeds()
{
	GetWorldTimerManager().SetTimer(
		FoodGrowTimerHandle,
		this,
		&ThisClass::GrowFood,
		FoodGrowTime,
		false);

	IsGrowingFood = true;
}

void ASoilBase::GrowFood()
{
	if (!IsValid(FoodClass))
	{
		UE_LOG(LogPocket, Error, TEXT("Food Class invalid, food growing failed, you are a bad farmer"));
		return;
	}
	
	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
	GetWorld()->SpawnActor<AItemBase>(FoodClass, GetActorLocation() + FVector(0, 0, 100), FRotator::ZeroRotator, SpawnParams);
	
	FoodGrown++;
	IsGrowingFood = false;
	OnFoodReady.Broadcast();

	if (FoodGrown >= MaxFoodToGrow)
	{
		Destroy();
	}
}
