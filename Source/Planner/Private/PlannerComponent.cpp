// Copyright Daniel Sellers 2024.


#include "PlannerComponent.h"

UPlannerComponent::UPlannerComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

}


void UPlannerComponent::BeginPlay()
{
	Super::BeginPlay();

	
}


void UPlannerComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

