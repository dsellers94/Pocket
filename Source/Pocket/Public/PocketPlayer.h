// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "InputAction.h"
#include "InputMappingContext.h"
#include "PocketPlayer.generated.h"

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

	UPROPERTY(EditAnywhere)
	float RotationRate = 100.f;

	UFUNCTION()
	void Rotate(float InputValue);

private:

	UPROPERTY()
	TObjectPtr<UCameraComponent> Camera;

	UPROPERTY()
	TObjectPtr<USpringArmComponent> SpringArm;

};
