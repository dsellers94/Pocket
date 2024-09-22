// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InputAction.h"
#include "InputMappingContext.h"
#include "GameFramework/PlayerController.h"
#include "PocketPlayerController.generated.h"

class APocketPlayer;

UCLASS()
class POCKET_API APocketPlayerController : public APlayerController
{
	GENERATED_BODY()
	

public:

	virtual void SetupInputComponent() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enhanced Input")
	UInputMappingContext* inputMappingContext;

	void EnhancedRotate(const FInputActionValue& Value);
	void EnhancedZoom(const FInputActionValue& Value);
	void EnhancedMoveUp(const FInputActionValue& Value);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enhanced Input")
	UInputAction* inputRotate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enhanced Input")
	UInputAction* inputZoom;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enhanced Input")
	UInputAction* inputMoveUp;

	UPROPERTY()
	APocketPlayer* ControlledPlayer;

protected:

	virtual void BeginPlay() override;

};
