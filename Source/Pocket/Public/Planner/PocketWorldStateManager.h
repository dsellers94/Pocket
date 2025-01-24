// Copyright Daniel Sellers 2024.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WorldStateManagerInterface.h"
#include "PlannerAIController.h"
#include "PocketWorldStateManager.generated.h"


UCLASS()
class POCKET_API APocketWorldStateManager : public AActor, public IWorldStateManagerInterface
{
	GENERATED_BODY()
	
public:	
	APocketWorldStateManager();

	virtual TMap<FName, bool> RequestWorldState_Implementation(APlannerAIController* Agent) override;

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

};
