// Fill out your copyright notice in the Description page of Project Settings.


#include "PocketPlayerController.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "PocketPlayer.h"

void APocketPlayerController::BeginPlay()
{
	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
	if (Subsystem)
	{
		Subsystem->AddMappingContext(inputMappingContext, 0);
	}
	ControlledPlayer = Cast<APocketPlayer>(GetPawn());
}

void APocketPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	UEnhancedInputComponent* EnhancedInput = Cast<UEnhancedInputComponent>(InputComponent);

	EnhancedInput->BindAction(inputRotate, ETriggerEvent::Triggered, this, &APocketPlayerController::EnhancedRotate);

}

void APocketPlayerController::EnhancedRotate(const FInputActionValue& Value)
{
	const float AxisValue = Value.Get<float>();

	if (!ControlledPlayer) return;

	ControlledPlayer->Rotate(AxisValue);

}


