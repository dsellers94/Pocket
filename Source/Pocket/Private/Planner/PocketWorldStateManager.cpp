// Copyright Daniel Sellers 2024.


#include "Planner/PocketWorldStateManager.h"

APocketWorldStateManager::APocketWorldStateManager()
{
	PrimaryActorTick.bCanEverTick = true;

}

void APocketWorldStateManager::BeginPlay()
{
	Super::BeginPlay();
	
}

void APocketWorldStateManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

TMap<FName, bool> APocketWorldStateManager::RequestWorldState_Implementation(APlannerAIController* Agent)
{
	return TMap<FName, bool>();
}