// Copyright Daniel Sellers 2024.


#include "Static/LevelStatic.h"

ALevelStatic::ALevelStatic()
{
	PrimaryActorTick.bCanEverTick = true;

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	SetRootComponent(StaticMesh);
	StaticMesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);
	StaticMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldDynamic, ECollisionResponse::ECR_Overlap);
}

void ALevelStatic::BeginPlay()
{
	Super::BeginPlay();
	
}

void ALevelStatic::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

