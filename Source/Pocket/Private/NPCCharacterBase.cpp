// Copyright Daniel Sellers 2024.


#include "NPCCharacterBase.h"
#include "PlannerComponent.h"
#include "Components/SphereComponent.h"
#include "Inventory/Inventorycomponent.h"
#include "Item/ItemBase.h"
#include "Pocket/Pocket.h"

ANPCCharacterBase::ANPCCharacterBase()
{
	PrimaryActorTick.bCanEverTick = true;

	DetectionSphere = CreateDefaultSubobject<USphereComponent>("DetectionSphere");
	DetectionSphere->SetupAttachment(RootComponent);

	PlannerComponent = CreateDefaultSubobject<UPlannerComponent>("PlannerComponent");

	InventoryComponent = CreateDefaultSubobject<UInventoryComponent>("InventoryComponent");
}

void ANPCCharacterBase::BeginPlay()
{
	Super::BeginPlay();
	
}

void ANPCCharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

bool ANPCCharacterBase::AttemptItemPickup(TSubclassOf<AItemBase> ItemClass)
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

void ANPCCharacterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

