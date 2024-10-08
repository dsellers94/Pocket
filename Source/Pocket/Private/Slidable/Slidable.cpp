// Copyright Daniel Sellers 2024.


#include "Slidable/Slidable.h"

ASlidable::ASlidable()
{
	PrimaryActorTick.bCanEverTick = true;

	DefaultRoot = CreateDefaultSubobject<USceneComponent>(TEXT("DefaultRoot"));
	SetRootComponent(DefaultRoot);

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	StaticMesh->SetupAttachment(RootComponent);
	StaticMesh->OnComponentBeginOverlap.AddDynamic(this, &ASlidable::OnMeshBeginOverlap);
	//StaticMesh->OnComponentHit.AddDynamic(this, &ASlidable::OnHit);
}

void ASlidable::BeginPlay()
{
	Super::BeginPlay();
	
}

void ASlidable::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bIsSliding) Slide(DeltaTime);
}

void ASlidable::Slide(float DeltaTime)
{
	FHitResult BumpHit;
	SetActorLocation(GetActorLocation() + SlidingDirection * SlidingSpeed * DeltaTime);
}

void ASlidable::Interact_Implementation(FVector InteractionPoint)
{
	bIsSliding = true;

	FVector ImpactSeparationNormal = (GetActorLocation() - InteractionPoint).GetSafeNormal();

	FVector2D ImpactSeparationNormal2D = FVector2D(ImpactSeparationNormal.X, ImpactSeparationNormal.Y);
	FVector2D ForwardVector2D = FVector2D(GetActorForwardVector().X, GetActorForwardVector().Y);
	FVector2D RightVector2D = FVector2D(GetActorRightVector().X, GetActorRightVector().Y);

	float ForwardVectorDot2D = ImpactSeparationNormal2D.Dot(ForwardVector2D);
	float RightVectorDot2D = ImpactSeparationNormal2D.Dot(RightVector2D);

	if (ForwardVectorDot2D < SlideBackwardsCutoff)
	{
		UE_LOG(LogTemp, Warning, TEXT("Sliding Backward"));
		SlidingDirection = -GetActorForwardVector();
	}
	else if (ForwardVectorDot2D < SlideSideCutoff)
	{
		if (RightVectorDot2D > 0)
		{
			UE_LOG(LogTemp, Warning, TEXT("Sliding Right"));
			SlidingDirection = GetActorRightVector();
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Sliding Left"));
			SlidingDirection = -GetActorRightVector();
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Sliding Forward"));
		SlidingDirection = GetActorForwardVector();
	}

}

void ASlidable::StopInteracting_Implementation()
{
	bIsSliding = false;
	UE_LOG(LogTemp, Warning, TEXT("Slidable: I'll just stop right here"));
}

void ASlidable::OnMeshBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	UE_LOG(LogTemp, Warning, TEXT("Slidable: OverlapDetected"));
	bIsSliding = false;
}

void ASlidable::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit)
{
	UE_LOG(LogTemp, Warning, TEXT("Slidable: Hit Detected"));
	bIsSliding = false;
}
