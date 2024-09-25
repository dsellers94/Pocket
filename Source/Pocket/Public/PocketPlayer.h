// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "InputAction.h"
#include "InputMappingContext.h"
#include "PocketPlayer.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FRotated);

class UCameraComponent;
class USpringArmComponent;

UCLASS()
class POCKET_API APocketPlayer : public APawn
{
	GENERATED_BODY()

public:
	APocketPlayer();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	UPROPERTY()
	FRotated Rotated;

	UPROPERTY(EditAnywhere)
	float RotationRate = 100.f;

	UPROPERTY(EditAnywhere)
	float MinArmLength = 200.f;

	UPROPERTY(EditAnywhere)
	float MaxArmLength = 6000.f;

	UPROPERTY(EditAnywhere)
	float ZoomRate = 100.f;

	UPROPERTY(EditAnywhere)
	float MoveUpRate = 100.f;

	UFUNCTION()
	void Rotate(float InputValue);

	UFUNCTION()
	void Zoom(float InputValue);

	UFUNCTION()
	void MoveUp(float InputValue);

	UPROPERTY(EditAnywhere)
	TObjectPtr<UCameraComponent> Camera;

	UPROPERTY(EditAnywhere)
	TObjectPtr<USpringArmComponent> SpringArm;

	UPROPERTY(EditAnywhere)
	TObjectPtr<USceneComponent> DefaultRoot;

private:



};
