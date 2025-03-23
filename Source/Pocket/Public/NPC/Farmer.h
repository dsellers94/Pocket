// Copyright Daniel Sellers 2024.

#pragma once

#include "CoreMinimal.h"
#include "NPCCharacterBase.h"
#include "Farmer.generated.h"


UCLASS()
class POCKET_API AFarmer : public ANPCCharacterBase
{
	GENERATED_BODY()

public:

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float MonsterDistanceTolerance = 500.f;
	
};
