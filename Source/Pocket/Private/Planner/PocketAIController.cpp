// Copyright Daniel Sellers 2024.


#include "Planner/PocketAIController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "NPCCharacterBase.h"
#include "Pocket/Pocket.h"

void APocketAIController::BeginPlay()
{
	Super::BeginPlay();

	ControlledNPC = Cast<ANPCCharacterBase>(GetPawn());

}


