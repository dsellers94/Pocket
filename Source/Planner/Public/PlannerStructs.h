// Copyright Daniel Sellers 2024.

#pragma once

#include "CoreMinimal.h"
#include "PlannerStructs.generated.h"

class AContextCheckActor;
class AActionExecutionActor;

UENUM(BlueprintType)
enum EGoalPriority : uint8
{
	EGP_Low		UMETA(DisplayName = "Low"),
	EGP_Medmium UMETA(DisplayName = "Medium"),
	EGP_High	UMETA(DisplayName = "High"),
};

USTRUCT(BlueprintType)
struct FWorldStatePair
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName Key;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool Value;

	bool IsCompatible(const FWorldStatePair OtherPair)
	{
		return Key != OtherPair.Key || (Key == OtherPair.Key && Value == OtherPair.Value);
	}

	bool Satisfies(const FWorldStatePair OtherPair)
	{
		return Key == OtherPair.Key && Value == OtherPair.Value;
	}

	FWorldStatePair()
	{
		Key = FName("NONE");
		Value = false;
	}

	FWorldStatePair(FName NewKey, bool NewValue)
	{
		Key = NewKey;
		Value = NewValue;
	}

	bool operator==(const FWorldStatePair& OtherPair) const
	{
		return Key == OtherPair.Key && Value == OtherPair.Value;
	}

};

USTRUCT(BlueprintType)
struct FGoal
{
	GENERATED_BODY()

	bool operator==(const FGoal& Other) const
	{
		return GoalID == Other.GoalID;
	}

public:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FWorldStatePair GoalState;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FGuid GoalID = FGuid::NewGuid();

	UPROPERTY(EditDefaultsOnly)
	TEnumAsByte<EGoalPriority> Priority = EGP_Low;
};

USTRUCT(BlueprintType)
struct FAction
{
	GENERATED_BODY()

	bool operator==(const FAction& Other) const
	{
		return ActionID == Other.ActionID;
	}

public:

	bool EffectsSatisfyCondition(FWorldStatePair Condition)
	{
		for (FWorldStatePair Pair : Effects)
		{
			if (Pair.Satisfies(Condition)) 
			{
				return true;
			}
		}
		return false;
	}

	bool UnsatisfiedConditionsContain(FWorldStatePair Condition)
	{
		for (FWorldStatePair Pair : UnSatisfiedConditions)
		{
			if (Pair.Key == Condition.Key) return true;
		}
		return false;
	}

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FName ActionName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FGuid ActionID = FGuid::NewGuid();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<FWorldStatePair> Preconditions;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<FWorldStatePair> Effects;

	UPROPERTY()
	FGuid ParentActionID;

	UPROPERTY(VisibleAnywhere)
	int CalculatedCost = 0;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int ActionCost = 1;

	UPROPERTY(EditDefaultsOnly)
	bool bNeedsContextCheck = false;

	UPROPERTY()
	bool bActionAvailable = true;

	UPROPERTY()
	TArray<FWorldStatePair> UnSatisfiedConditions;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSoftClassPtr<AContextCheckActor> ContextCheckActor = nullptr;

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

USTRUCT(BlueprintType)
struct FGoalRow : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GoalData")
	FGoal Goal;
};


class PLANNER_API PlannerStructs
{
public:
	PlannerStructs();
	~PlannerStructs();
};
