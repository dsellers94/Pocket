// Fill out your copyright notice in the Description page of Project Settings.


#include "PocketPlayer.h"
#include "Camera/CameraComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/GameplayStatics.h"
#include "InteractInterface.h"

APocketPlayer::APocketPlayer()
{
	PrimaryActorTick.bCanEverTick = true;

	DefaultRoot = CreateDefaultSubobject<USceneComponent>(TEXT("SceneRoot"));
	SetRootComponent(DefaultRoot);

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(RootComponent);
	SpringArm->bDoCollisionTest = false;

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera")); 
	Camera->SetupAttachment(SpringArm);
}

void APocketPlayer::BeginPlay()
{
	Super::BeginPlay();
}

void APocketPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (CursorActive) CursorTrace();
}

void APocketPlayer::CursorTrace()
{
	GetWorld()->GetFirstPlayerController()->GetHitResultUnderCursor(ECollisionChannel::ECC_Visibility, false, HitUnderCursor);

	ActorUnderCursor = HitUnderCursor.GetActor();

	if (ActorUnderCursor && ActorUnderCursor->GetClass()->ImplementsInterface(UInteractInterface::StaticClass()))
	{
		CurrentInteractable.SetObject(ActorUnderCursor);
	}
	else
	{
		if (CurrentInteractable.GetObject()) LastInteractable = CurrentInteractable;
		CurrentInteractable.SetObject(nullptr);
	}
}

void APocketPlayer::Rotate(float InputValue)
{
	FRotator Rotation = GetActorRotation();
	FRotator NewRotation = FRotator(Rotation.Pitch, Rotation.Yaw + InputValue * RotationRate * GetWorld()->GetDeltaSeconds(), Rotation.Roll);
	SetActorRotation(NewRotation);
	ViewChanged.Broadcast();
}

void APocketPlayer::Zoom(float InputValue)
{
	if (SpringArm)
	{
		SpringArm->TargetArmLength = FMath::Clamp(SpringArm->TargetArmLength + InputValue * ZoomRate, MinArmLength, MaxArmLength);
	}
	ViewChanged.Broadcast();
}

void APocketPlayer::MoveUp(float InputValue)
{
	FVector Location = GetActorLocation();
	FVector NewLocation = FVector(Location.X, Location.Y, Location.Z + InputValue * MoveUpRate);
	SetActorLocation(NewLocation);
}

void APocketPlayer::Interact(bool InputValue)
{
	if (InputValue)
	{
		if (CurrentInteractable.GetObject())
		{
			UObject* InteractionObject = CurrentInteractable.GetObject();
			IInteractInterface::Execute_Interact(InteractionObject, HitUnderCursor.ImpactPoint);
		}
	}
	else
	{
		if (CurrentInteractable.GetObject())
		{
			UObject* InteractionObject = CurrentInteractable.GetObject();
			IInteractInterface::Execute_StopInteracting(InteractionObject);
		}
		else if (LastInteractable.GetObject())
		{
			UObject* InteractionObject = LastInteractable.GetObject();
			IInteractInterface::Execute_StopInteracting(InteractionObject);
		}
	}
}


