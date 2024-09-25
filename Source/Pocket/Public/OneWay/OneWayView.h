// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "OneWayView.generated.h"

class APocketPlayer;
class UCameraComponent;

UCLASS()
class POCKET_API AOneWayView : public AActor
{
	GENERATED_BODY()
	
public:	
	AOneWayView();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<USceneComponent> DefaultRoot;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UStaticMeshComponent> StaticMesh;

	UPROPERTY()
	TObjectPtr<APocketPlayer> Player;

	UPROPERTY(BlueprintReadWrite)
	TObjectPtr<UCameraComponent> GameCamera;


protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	void SetVisibility();


public:	
	virtual void Tick(float DeltaTime) override;

};
