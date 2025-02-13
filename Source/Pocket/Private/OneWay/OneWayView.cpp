// Fill out your copyright notice in the Description page of Project Settings.


#include "OneWay/OneWayView.h"
#include "Camera/CameraComponent.h"
#include "PocketPlayer.h"

AOneWayView::AOneWayView()
{
	PrimaryActorTick.bCanEverTick = true;

	DefaultRoot = CreateDefaultSubobject<USceneComponent>(TEXT("DefaultRoot"));
	SetRootComponent(DefaultRoot);

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	StaticMesh->SetupAttachment(DefaultRoot);
	StaticMesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);
	StaticMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldDynamic, ECollisionResponse::ECR_Overlap);

}

void AOneWayView::BeginPlay()
{
	Super::BeginPlay();
	
	APlayerController* Controller = GetWorld()->GetFirstPlayerController();

	if (Controller) Player = Cast<APocketPlayer>(Controller->GetPawn());

	if (Player)
	{
		GameCamera = Player->Camera;
		Player->ViewChanged.AddDynamic(this, &AOneWayView::SetVisibility);
	}

	if (!GameCamera || !Player) UE_LOG(LogTemp, Warning, TEXT("OneWayView: Failed to get Game Camera or Player Pawn!!!"));

	SetVisibility();
}

void AOneWayView::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AOneWayView::SetVisibility()
{
	if (!StaticMesh || !GameCamera) return;

	FVector SeparationVector = GetActorLocation() - (GameCamera->GetComponentLocation() - GameCamera->GetForwardVector() * 100.f);

	float VectorDot = GetActorForwardVector().Dot(SeparationVector);

	//StaticMesh->SetVisibility(VectorDot < 0.f);
	DefaultRoot->SetVisibility(VectorDot < 0.f, true);
}
