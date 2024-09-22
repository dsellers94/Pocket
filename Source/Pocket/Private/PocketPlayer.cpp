// Fill out your copyright notice in the Description page of Project Settings.


#include "PocketPlayer.h"
#include "Camera/CameraComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "GameFramework/SpringArmComponent.h"

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

}

void APocketPlayer::Rotate(float InputValue)
{
	FRotator Rotation = GetActorRotation();
	FRotator NewRotation = FRotator(Rotation.Pitch, Rotation.Yaw + InputValue * RotationRate * GetWorld()->GetDeltaSeconds(), Rotation.Roll);
	SetActorRotation(NewRotation);
}

void APocketPlayer::Zoom(float InputValue)
{
	if (SpringArm)
	{
		SpringArm->TargetArmLength = FMath::Clamp(SpringArm->TargetArmLength + InputValue * ZoomRate, MinArmLength, MaxArmLength);
	}
}

void APocketPlayer::MoveUp(float InputValue)
{
	FVector Location = GetActorLocation();
	FVector NewLocation = FVector(Location.X, Location.Y, Location.Z + InputValue * MoveUpRate);
	SetActorLocation(NewLocation);
}


