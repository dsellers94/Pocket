// Copyright Daniel Sellers 2024.


#include "NPCBase.h"
#include "PlannerComponent.h"
#include "Components/CapsuleComponent.h"
#include "Inventory/Inventorycomponent.h"

ANPCBase::ANPCBase()
{
	PrimaryActorTick.bCanEverTick = true;

	CapsuleComponent = CreateDefaultSubobject<UCapsuleComponent>("CapsuleComponent");
	SetRootComponent(CapsuleComponent);

	PlannerComponent = CreateDefaultSubobject<UPlannerComponent>("PlannerComponent");

	InventoryComponent = CreateDefaultSubobject<UInventoryComponent>("InventoryComponent");

	PawnMovementComponent = CreateDefaultSubobject<UFloatingPawnMovement>("PawnMovementComponent");
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

