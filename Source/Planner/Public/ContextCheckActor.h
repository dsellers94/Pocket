// Copyright Daniel Sellers 2024.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ContextCheckActor.generated.h"

UCLASS()
class PLANNER_API AContextCheckActor : public AActor
{
	GENERATED_BODY()
	
public:	
	AContextCheckActor();

	UFUNCTION(BlueprintNativeEvent)
	bool CheckValidity(AActor* Agent);

	virtual bool CheckValidity_Implementation(AActor* Agent);

protected:
	virtual void BeginPlay() override;


};
