// Copyright Daniel Sellers 2024.


#include "Inventory/InventoryComponent.h"
#include "Item/ItemBase.h"

UInventoryComponent::UInventoryComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

}

bool UInventoryComponent::HasItem(TSubclassOf<AItemBase> ItemClass)
{
	for (FItem Item : Items)
	{
		if (Item.ItemClass == ItemClass)
		{
			return true;
		}
	}
	return false;
}

void UInventoryComponent::BeginPlay()
{
	Super::BeginPlay();
	
}

FString FItem::ToString() const
{
	TStringBuilderWithBuffer<TCHAR, 256> StringBuilder;
	StringBuilder.Append(TEXT("This will prevent item information eventually"));

	FString Result = StringBuilder.ToString();
	return Result;
}
