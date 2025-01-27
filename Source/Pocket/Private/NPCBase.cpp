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
	PrimaryActorTick.bCanEverTick = true;

	CapsuleComponent = CreateDefaultSubobject<UCapsuleComponent>("CapsuleComponent");
	SetRootComponent(CapsuleComponent);

	DetectionSphere = CreateDefaultSubobject<USphereComponent>("DetectionSphere");
	DetectionSphere->SetupAttachment(CapsuleComponent);

	PlannerComponent = CreateDefaultSubobject<UPlannerComponent>("PlannerComponent");

	InventoryComponent = CreateDefaultSubobject<UInventoryComponent>("InventoryComponent");

	PawnMovementComponent = CreateDefaultSubobject<UFloatingPawnMovement>("PawnMovementComponent");
}

bool ANPCBase::AttemptItemPickup(TSubclassOf<AItemBase> ItemClass)
{
	if (!IsValid(InventoryComponent))
	{
		UE_LOG(LogPocket, Error, TEXT("InventoryComponent not valid"));
		return false;
	}

	TArray<AActor*> Actors;
	DetectionSphere->GetOverlappingActors(Actors, ItemClass.Get());

	for (AActor* Actor : Actors)
	{
		if (Actor->GetClass() == ItemClass)
		{
			AItemBase* Item = Cast<AItemBase>(Actor);
			InventoryComponent->AddItem(Item->ItemData);
			return true;
		}
	}
	return false;
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

