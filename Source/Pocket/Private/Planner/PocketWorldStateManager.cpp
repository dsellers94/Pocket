// Copyright Daniel Sellers 2024.

#include "Planner/PocketWorldStateManager.h"
#include "Item/ItemBase.h"
#include "Kismet/GameplayStatics.h"
#include "Item/FoodStashBase.h"
#include "Pocket/Pocket.h"
#include "Inventory/InventoryComponent.h"

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

	if (!IsValid(Agent))
	{
		UE_LOG(LogPocket, Error, TEXT("Requesting Agent Invalid when requesting plan"));
		return WorldState;
	}
	if (!IsValid(Agent->GetPawn()))
	{
		UE_LOG(LogPocket, Error, TEXT("Agent Pawn Invalid when requesting plan"));
		return WorldState;
	}

	UInventoryComponent* AgentInventory = Agent->GetPawn()->GetComponentByClass<UInventoryComponent>();

	if (!IsValid(AgentInventory))
	{
		UE_LOG(LogPocket, Error, TEXT("Agent Pawn Invalid when requesting plan"));
		return WorldState;
	}

	WorldState.Add(FName("kIsHoldingFood"), AgentInventory->HasItemByClass(FoodClass));
	WorldState.Add(FName("kIsHoldingPlow"), AgentInventory->HasItemByClass(PlowClass));
	WorldState.Add(FName("kIsHoldingSeeds"), AgentInventory->HasItemByClass(SeedsClass));
	WorldState.Add(FName("kFoodOnMap"),bFoodOnMap);
	WorldState.Add(FName("kPlowOnMap"), bPlowOnMap);
	WorldState.Add(FName("kSeedsOnMap"), bSeedsOnMap);
	WorldState.Add(FName("kSoilOnMap"), bSoilOnMap);
	WorldState.Add(FName("kFoodIsDelivered"), bFoodIsDelivered);


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