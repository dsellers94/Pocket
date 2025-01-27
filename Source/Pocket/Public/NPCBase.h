// Copyright Daniel Sellers 2024.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "NPCBase.generated.h"

class UPlannerComponent;
class UInventoryComponent;
class UCapsuleComponent;
class USphereComponent;

UCLASS()
class POCKET_API ANPCBase : public APawn
{
	GENERATED_BODY()

public:
	ANPCBase();

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UCapsuleComponent> CapsuleComponent = nullptr;

	UPROPERTY(EditAnywhere)
	TObjectPtr<USphereComponent> DetectionSphere = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UPlannerComponent> PlannerComponent = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UInventoryComponent> InventoryComponent = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UFloatingPawnMovement> PawnMovementComponent = nullptr;

	UFUNCTION(BlueprintCallable)
	bool AttemptItemPickup(TSubclassOf<AItemBase> ItemClass);

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
