// Copyright Daniel Sellers 2024.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InventoryComponent.generated.h"

class AItemBase;

USTRUCT(BlueprintType)
struct FItem
{
	GENERATED_BODY()

	bool operator==(const FItem& Other) const
	{
		return ItemID == Other.ItemID;
	}

public:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FName ItemName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FGuid ItemID = FGuid::NewGuid();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<AItemBase> ItemClass = nullptr;

	FString ToString() const;
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class POCKET_API UInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UInventoryComponent();

	UPROPERTY()
	TArray<FItem> Items;

	UFUNCTION(BlueprintCallable)
	bool HasItem(TSubclassOf<AItemBase> ItemClass);


protected:
	virtual void BeginPlay() override;


		
};
