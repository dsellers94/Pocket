// Copyright Daniel Sellers 2024.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "NPCCharacterBase.generated.h"

class UPlannerComponent;
class UInventoryComponent;
class USphereComponent;
class UBehaviorTree;

UCLASS()
class POCKET_API ANPCCharacterBase : public ACharacter
{
	GENERATED_BODY()

public:
	ANPCCharacterBase();

	UPROPERTY(EditAnywhere)
	TObjectPtr<USphereComponent> DetectionSphere = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UPlannerComponent> PlannerComponent = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UInventoryComponent> InventoryComponent = nullptr;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UBehaviorTree> BehaviorTree = nullptr;

	UFUNCTION(BlueprintCallable)
	bool AttemptItemPickup(TSubclassOf<AItemBase> ItemClass);

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
