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

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int Quantity = 1;

	FString ToString() const;
};

USTRUCT(BlueprintType)
struct FItemRow : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "ItemData")
	FItem Item;
};

UCLASS(Blueprintable, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class POCKET_API UInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UInventoryComponent();

	UPROPERTY()
	TArray<FItem> Items;

	UFUNCTION(BlueprintCallable)
	bool HasItemByClass(TSubclassOf<AItemBase> ItemClass);

	UFUNCTION(BlueprintCallable)
	bool HasItemByID(FGuid ItemID);

	UFUNCTION(BlueprintCallable)
	void AddItem(AItemBase* InItem);

	UFUNCTION(BlueprintCallable)
	void RemoveItemByClass(TSubclassOf<AItemBase> ItemClass);

	UFUNCTION(BlueprintCallable)
	void PrintHeldItems();

protected:
	virtual void BeginPlay() override;


		
};
