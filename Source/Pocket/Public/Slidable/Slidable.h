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

	UFUNCTION()
	void OnMeshBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, 
		int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit);


protected:
	virtual void BeginPlay() override;

	UPROPERTY()
	bool bIsSliding = false;

	UPROPERTY()
	FVector SlidingDirection;

	UFUNCTION()
	void Slide(float DeltaTime);

public:	
	virtual void Tick(float DeltaTime) override;

};
