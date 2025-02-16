// Copyright Daniel Sellers 2024.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WorldStateManagerInterface.h"
#include "PlannerAIController.h"
#include "PocketWorldStateManager.generated.h"

class AItemBase;
class AFoodStashBase;

UCLASS()
class POCKET_API APocketWorldStateManager : public AActor, public IWorldStateManagerInterface
{
	GENERATED_BODY()
	
public:	
	APocketWorldStateManager();

	virtual TMap<FName, bool> RequestWorldState_Implementation(APlannerAIController* Agent) override;

	UPROPERTY(EditAnywhere)
	TSubclassOf<AItemBase> FoodClass;

	UPROPERTY(EditAnywhere)
	TSubclassOf<AItemBase> PlowClass;

	UPROPERTY(EditAnywhere)
	TSubclassOf<AItemBase> SeedsClass;

	UPROPERTY(EditAnywhere)
	TSubclassOf<AItemBase> SoilClass;

	UPROPERTY(EditAnywhere)
	TSubclassOf<AFoodStashBase> FoodStashClass;

protected:

	virtual void BeginPlay() override;

	// Level WorldState Values

	UPROPERTY()
	bool bFoodOnMap = false;

	UPROPERTY()
	bool bFoodIsDelivered = false;

	UPROPERTY()
	bool bPlowOnMap = false;

	UPROPERTY()
	bool bSoilOnMap = false;

	UPROPERTY()
	bool bSoilIsAvailable = false;

	UPROPERTY()
	bool bFoodIsGrowing = false;

	UPROPERTY()
	bool bSeedsOnMap = false;

	UFUNCTION()
	void UpdateLevelWorldStateValues();


};
