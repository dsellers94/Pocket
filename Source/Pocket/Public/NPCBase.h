// Copyright Daniel Sellers 2024.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "NPCBase.generated.h"

class UPlannerComponent;

UCLASS()
class POCKET_API ANPCBase : public APawn
{
	GENERATED_BODY()

public:
	ANPCBase();

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UPlannerComponent> PlannerComponent = nullptr;

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
