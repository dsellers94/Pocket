// Copyright Daniel Sellers 2024.


#include "ActionExecutionActor.h"
#include "PlannerAIController.h"
#include "Planner/Planner.h"

AActionExecutionActor::AActionExecutionActor()
{
	PrimaryActorTick.bCanEverTick = true;

}

void AActionExecutionActor::BeginPlay()
{
	Super::BeginPlay();
	
}

void AActionExecutionActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


void AActionExecutionActor::SetCallingAgent(APlannerAIController* InCallingAgent)
{
	if (IsValid(InCallingAgent))
	{
		CallingAgent = InCallingAgent;
	}
}

void AActionExecutionActor::Execute_Implementation()
{
	UE_LOG(LogPlanner, Warning, TEXT("Executing Action: %s"), *this->GetClass()->GetName());
}

void AActionExecutionActor::DeclareActionComplete()
{
	OnActionComplete.Broadcast();
}

void AActionExecutionActor::DeclareActionFailed()
{
	OnActionFailed.Broadcast();
}


