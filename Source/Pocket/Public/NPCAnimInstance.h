// Copyright Daniel Sellers 2024.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "NPCAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class POCKET_API UNPCAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:

	virtual void NativeUpdateAnimation(float DeltaTime) override;

	UPROPERTY(BlueprintReadOnly)
	FVector Velocity;

	UPROPERTY(BlueprintReadOnly)
	float Speed;
	
};
