// Copyright Daniel Sellers 2024.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ActionExecutionActor.generated.h"

class APlannerAIController;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnActionComplete);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnActionFailed);

UCLASS()
class PLANNER_API AActionExecutionActor : public AActor
{
	GENERATED_BODY()
	
public:	
	AActionExecutionActor();

	UPROPERTY(BlueprintAssignable)
	FOnActionComplete OnActionComplete;

	UPROPERTY(BlueprintAssignable)
	FOnActionFailed OnActionFailed;

	UPROPERTY(BlueprintReadWrite)
	TObjectPtr<APlannerAIController> CallingAgent = nullptr;

	UFUNCTION()
	void SetCallingAgent(APlannerAIController* InCallingAgent);

	UFUNCTION(BlueprintNativeEvent)
	void Execute();

	virtual void Execute_Implementation();

	UFUNCTION(BlueprintCallable)
	void DeclareActionComplete();

	UFUNCTION(BlueprintCallable)
	void DeclareActionFailed();


protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

};
