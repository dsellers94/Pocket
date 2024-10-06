// Copyright Daniel Sellers 2024.


#include "Slidable/Slidable.h"

ASlidable::ASlidable()
{
	PrimaryActorTick.bCanEverTick = true;

	DefaultRoot = CreateDefaultSubobject<USceneComponent>(TEXT("DefaultRoot"));
	SetRootComponent(DefaultRoot);

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	StaticMesh->SetupAttachment(DefaultRoot);
}

void ASlidable::BeginPlay()
{
	Super::BeginPlay();
	
}

void ASlidable::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ASlidable::Interact_Implementation()
{
	UE_LOG(LogTemp, Warning, TEXT("Slidable: I AM THE CHOSEN ONE!"));
}

void ASlidable::StopInteracting_Implementation()
{
	UE_LOG(LogTemp, Warning, TEXT("Slidable: I am the unchosen one..."));
}
