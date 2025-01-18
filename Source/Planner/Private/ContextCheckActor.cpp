// Copyright Daniel Sellers 2024.


#include "ContextCheckActor.h"

AContextCheckActor::AContextCheckActor()
{
	PrimaryActorTick.bCanEverTick = false;

}

void AContextCheckActor::BeginPlay()
{
	Super::BeginPlay();
	
}

bool AContextCheckActor::CheckValidity_Implementation(AActor* Agent)
{
	return true;
}

