// Copyright Daniel Sellers 2024.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "InteractInterface.h"
#include "Slidable.generated.h"

UCLASS()
class POCKET_API ASlidable : public AActor, public IInteractInterface
{
	GENERATED_BODY()
	
public:	
	ASlidable();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<USceneComponent> DefaultRoot;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UStaticMeshComponent> StaticMesh;

	UPROPERTY(EditAnywhere)
	float SlideBackwardsCutoff = -0.707;

	UPROPERTY(EditAnywhere)
	float SlideSideCutoff = 0.707;

	UPROPERTY(EditAnywhere)
	float SlidingSpeed = 100.f;

	virtual void Interact_Implementation(FVector InteractionPoint) override;

	virtual void StopInteracting_Implementation() override;

	virtual void ReactToCursorFocus_Implementation() override;

	virtual void StopReactingToCursorFocus_Implementation() override;


protected:
	virtual void BeginPlay() override;

	UPROPERTY()
	bool bIsSliding = false;

	UPROPERTY()
	FVector SlidingDirection;

	UPROPERTY()
	FVector LastLocation;

	UFUNCTION()
	void Slide(float DeltaTime);

public:	
	virtual void Tick(float DeltaTime) override;

};
