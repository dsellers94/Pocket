// Copyright Daniel Sellers 2024.

#pragma once

#include "CoreMinimal.h"
#include "PlannerStructs.generated.h"

class AContextCheckActor;

UENUM(BlueprintType)
enum class EWorldStateKey : uint8
{
	EWK_FoodIsDelivered,
	EWK_IsHoldingFood,
	EWK_IsHoldingSeeds,
	EWK_IsHoldingPlow,
	EWK_FoodOnMap,
	EWK_SeedsOnMap,
	EWK_SoilOnMap,
	EWK_PlowOnMap
};

USTRUCT(BlueprintType)
struct FAction
{
	GENERATED_BODY()

public:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TMap<EWorldStateKey, bool> Preconditions;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TMap<EWorldStateKey, bool> Effects;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSoftClassPtr<AContextCheckActor> ContextCheckActorClass = nullptr;
};

USTRUCT(BlueprintType)
struct FActionRow : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "ActionData")
	FAction Action;
};

class PLANNER_API PlannerStructs
{
public:
	PlannerStructs();
	~PlannerStructs();
};
