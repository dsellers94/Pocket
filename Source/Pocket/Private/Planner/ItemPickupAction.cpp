// Copyright Daniel Sellers 2024.


#include "Planner/ItemPickupAction.h"
#include "Kismet/gameplayStatics.h"
#include "PlannerAIController.h"
#include "Item/ItemBase.h"
#include "Pocket/Pocket.h"
#include "NPCCharacterBase.h"

void AItemPickupAction::Execute_Implementation()
{
	if (!IsValid(CallingAgent))
	{
		UE_LOG(LogPocket, Error, TEXT("CallingAgent invalid when executing action"));
		return;
	}
	APawn* ControlledPawn = CallingAgent->GetPawn();
	if (!IsValid(ControlledPawn))
	{
		UE_LOG(LogPocket, Error, TEXT("ControlledPawn invalid when executing action"));
		return;
	}
	ControlledCharacter = Cast<ANPCCharacterBase>(ControlledPawn);
	if (!IsValid(ControlledCharacter))
	{
		UE_LOG(LogPocket, Error, TEXT("NPCCharacterBase invalid when executing action"));
		return;
	}

	TArray<AActor*> Actors;
	UGameplayStatics::GetAllActorsOfClass(this, ItemClass, Actors);

	for (AActor* Actor : Actors)
	{
		
		float ItemDistance = FVector::Distance(
			ControlledCharacter->GetActorLocation(), Actor->GetActorLocation());

		if (ItemDistance < ClosestItemDistance)
		{
			ClosestItemDistance = ItemDistance;
			ClosestItem = Cast<AItemBase>(Actor);
		}
	}

	if (IsValid(ClosestItem))
	{
		ClosestItem->OnDestroyed.AddDynamic(this, &ThisClass::OnTargetItemDestroyed);
		StartMovement(ClosestItem);
	}
	else
	{
		DeclareActionFailed();
	}
}

void AItemPickupAction::FinishItemPickup()
{
	if (ControlledCharacter->AttemptItemPickup(ItemClass))
	{
		DeclareActionComplete();
		ClosestItem->Destroy();
	}
	else
	{
		DeclareActionFailed();
	}
}

void AItemPickupAction::OnTargetItemDestroyed(AActor* DestroyedItem)
{
	DeclareActionFailed();
}
