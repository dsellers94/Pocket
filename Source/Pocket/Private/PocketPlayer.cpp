// Fill out your copyright notice in the Description page of Project Settings.


#include "PocketPlayer.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"

APocketPlayer::APocketPlayer()
{
	PrimaryActorTick.bCanEverTick = true;

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(RootComponent);

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

void APocketPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

