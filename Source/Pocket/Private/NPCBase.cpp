// Copyright Daniel Sellers 2024.


#include "NPCBase.h"
#include "PlannerComponent.h"
#include "Inventory/Inventorycomponent.h"

ANPCBase::ANPCBase()
{
	PrimaryActorTick.bCanEverTick = true;

	PlannerComponent = CreateDefaultSubobject<UPlannerComponent>("PlannerComponent");

	InventoryComponent = CreateDefaultSubobject<UInventoryComponent>("InventoryComponent");
}

void ANPCBase::BeginPlay()
{
	Super::BeginPlay();
	
}

void ANPCBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ANPCBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

