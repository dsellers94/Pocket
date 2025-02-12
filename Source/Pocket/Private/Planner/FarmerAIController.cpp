// Copyright Daniel Sellers 2024.


#include "Planner/FarmerAIController.h"
#include "Pocket/Pocket.h"
#include "Item/FoodStashBase.h"
#include "Kismet/GameplayStatics.h"
#include "BehaviorTree/BlackboardComponent.h"

void AFarmerAIController::UpdateInfo()
{
	if (!IsValid(BlackboardComponent))
	{
		UE_LOG(LogPocket, Error, TEXT("FarmerAIcontroller: BlackboardComponent not found"));
		return;
	}

	TArray<AActor*> FoodStashes;
	UGameplayStatics::GetAllActorsOfClass(this, AFoodStashBase::StaticClass(), FoodStashes);
	bool FoodStashAvailable = false;
	for (AActor* Actor : FoodStashes)
	{
		AFoodStashBase* FoodStash = Cast<AFoodStashBase>(Actor);
		if (IsValid(FoodStash) && !FoodStash->StashOccupied)
		{
			FoodStashAvailable = true;
			break;
		}
	}
	BlackboardComponent->SetValueAsBool("FoodStashAvailable", FoodStashAvailable);


}