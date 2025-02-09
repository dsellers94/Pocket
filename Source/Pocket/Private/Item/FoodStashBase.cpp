// Copyright Daniel Sellers 2024.


#include "Item/FoodStashBase.h"

void AFoodStashBase::BeginPlay()
{
	Super::BeginPlay();
}


void AFoodStashBase::DeliverFood()
{
	StashOccupied = true;
	
	GetWorldTimerManager().SetTimer(
		ConsumeFoodTimerHandle,
		this,
		&ThisClass::ConsumeFood,
		ConsumeFoodTime,
		true);

	OnStatusChanged.Broadcast();
}

void AFoodStashBase::ConsumeFood()
{
	StashOccupied = false;
	OnStatusChanged.Broadcast();
}

