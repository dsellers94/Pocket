// Copyright Daniel Sellers 2024.

#pragma once

#include "CoreMinimal.h"
#include "PlannerStructs.generated.h"

class AContextCheckActor;
class AActionExecutionActor;

USTRUCT(BlueprintType)
struct FAction
{
	GENERATED_BODY()

	bool operator==(const FAction& Other) const
	{
		return ActionID == Other.ActionID;
	}

public:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FName ActionName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FGuid ActionID = FGuid::NewGuid();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TMap<FName, bool> Preconditions;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TMap<FName, bool> Effects;

	UPROPERTY()
	FGuid ParentActionID;

	UPROPERTY(VisibleAnywhere)
	int CalculatedCost = 0;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int ActionCost = 1;

	UPROPERTY()
	TMap<FName, bool> UnSatisfiedConditions;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSoftClassPtr<AContextCheckActor> ContextCheckActorClass = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSoftClassPtr<AActionExecutionActor> ActionExecutionActor = nullptr;

	FString ToString() const;
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
