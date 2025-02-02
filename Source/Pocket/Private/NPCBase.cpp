// Copyright Daniel Sellers 2024.


#include "NPCBase.h"
#include "PlannerComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SphereComponent.h"
#include "Inventory/Inventorycomponent.h"
#include "Item/ItemBase.h"
#include "Pocket/Pocket.h"

ANPCBase::ANPCBase()
{

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

