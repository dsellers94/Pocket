// Copyright Daniel Sellers 2024.

#pragma once

#include "CoreMinimal.h"
#include "ActionExecutionActor.h"
#include "ItemPickupAction.generated.h"

class AItemBase;
class ANPCCharacterBase;

UCLASS()
class POCKET_API AItemPickupAction : public AActionExecutionActor
{
	GENERATED_BODY()

public:

	virtual void Execute_Implementation() override;

	UPROPERTY()
	float ClosestItemDistance = FLT_MAX;

	UPROPERTY()
	TObjectPtr<AItemBase> ClosestItem = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<AItemBase> ItemClass;

	UPROPERTY()
	TObjectPtr<ANPCCharacterBase> ControlledCharacter = nullptr;

	UFUNCTION(BlueprintImplementableEvent)
	void StartMovement(AItemBase* TargetItem);

	UFUNCTION(BlueprintCallable)
	void FinishItemPickup();

protected:

	UFUNCTION()
	void OnTargetItemDestroyed(AActor* DestroyedItem);
	
};
