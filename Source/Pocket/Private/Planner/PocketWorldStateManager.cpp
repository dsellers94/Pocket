// Copyright Daniel Sellers 2024.

#include "Planner/PocketWorldStateManager.h"
#include "Item/ItemBase.h"
#include "Kismet/GameplayStatics.h"
#include "Item/FoodStashBase.h"
#include "Item/SoilBase.h"
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

TArray<FWorldStatePair> APocketWorldStateManager::RequestWorldState_Implementation(APlannerAIController* Agent)
{
	TArray<FWorldStatePair> WorldState = TArray<FWorldStatePair>();

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

	WorldState.Add(FWorldStatePair(FName("KIsHoldingFood"), AgentInventory->HasItemByClass(FoodClass)));
	WorldState.Add(FWorldStatePair(FName("kIsHoldingPlow"), AgentInventory->HasItemByClass(PlowClass)));
	WorldState.Add(FWorldStatePair(FName("kIsHoldingSeeds"), AgentInventory->HasItemByClass(SeedsClass)));
	WorldState.Add(FWorldStatePair(FName("kFoodOnMap"), bFoodOnMap));
	WorldState.Add(FWorldStatePair(FName("kPlowOnMap"), bPlowOnMap));
	WorldState.Add(FWorldStatePair(FName("kSeedsOnMap"), bSeedsOnMap));
	WorldState.Add(FWorldStatePair(FName("kSoilOnMap"), bSoilOnMap));
	WorldState.Add(FWorldStatePair(FName("kFoodIsGrowing"), bFoodIsGrowing));
	WorldState.Add(FWorldStatePair(FName("kSoilIsAvailable"), bSoilIsAvailable));
	WorldState.Add(FWorldStatePair(FName("kFoodIsDelivered"), bFoodIsDelivered));


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

	// Set bSoilIsAvailable & bSoilOnMap & bFoodIsGrowing
	UGameplayStatics::GetAllActorsOfClass(this, SoilClass, Actors);
	bSoilIsAvailable = false;
	bFoodIsGrowing = false;
	for (AActor* Actor : Actors)
	{
		ASoilBase* Soil = Cast<ASoilBase>(Actor);
		if (Soil->IsGrowingFood)
		{
			bFoodIsGrowing = true;
		}
		else
		{
			bSoilIsAvailable = true;
		}
	}
	if (Actors.Num() <= 0)
	{
		bSoilIsAvailable = false;
		bFoodIsGrowing = false;
	}
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
	Actors.Empty();

}