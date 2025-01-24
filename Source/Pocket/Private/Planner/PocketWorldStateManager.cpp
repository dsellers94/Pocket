// Copyright Daniel Sellers 2024.

#include "Planner/PocketWorldStateManager.h"
#include "Item/ItemBase.h"
#include "Kismet/GameplayStatics.h"
#include "Item/FoodStashBase.h"

APocketWorldStateManager::APocketWorldStateManager()
{
	PrimaryActorTick.bCanEverTick = false;

}

void APocketWorldStateManager::BeginPlay()
{
	Super::BeginPlay();
	
}

TMap<FName, bool> APocketWorldStateManager::RequestWorldState_Implementation(APlannerAIController* Agent)
{
	TMap<FName, bool> WorldState = TMap<FName, bool>();

	UpdateLevelWorldStateValues();

	// Access the Agent's controlled pawn and inventory component to set the individual worldstate value

	// Build the map out of all the worldstate values

	return WorldState;
}

void APocketWorldStateManager::UpdateLevelWorldStateValues()
{
	TArray<AActor*> Actors = TArray<AActor*>();
	
	// Set bFoodOnMap
	UGameplayStatics::GetAllActorsOfClass(this, FoodClass, Actors);
	bFoodOnMap = Actors.Num() > 0;
	Actors.Empty();
	// Set bPlowOnMap
	UGameplayStatics::GetAllActorsOfClass(this, PlowClass, Actors);
	bPlowOnMap = Actors.Num() > 0;
	Actors.Empty();
	// Set bSoilOnMap
	UGameplayStatics::GetAllActorsOfClass(this, SoilClass, Actors);
	bSoilOnMap = Actors.Num() > 0;
	Actors.Empty();
	// Set bSeedsOnMap
	UGameplayStatics::GetAllActorsOfClass(this, SeedsClass, Actors);
	bSeedsOnMap = Actors.Num() > 0;
	Actors.Empty();
	// Set bFoodIsDelivered
	UGameplayStatics::GetAllActorsOfClass(this, FoodStashClass, Actors);
	bFoodIsDelivered = true;
	for (AActor* Actor : Actors)
	{
		AFoodStashBase* FoodStash = Cast<AFoodStashBase>(Actor);
		if (!FoodStash->StashOccupied)
		{
			bFoodIsDelivered = false;
			break;
		}
	}

}