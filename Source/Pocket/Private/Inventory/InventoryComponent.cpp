// Copyright Daniel Sellers 2024.


#include "Inventory/InventoryComponent.h"
#include "Item/ItemBase.h"
#include "Pocket/Pocket.h"

UInventoryComponent::UInventoryComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

}

void UInventoryComponent::BeginPlay()
{
	Super::BeginPlay();

}

bool UInventoryComponent::HasItemByClass(TSubclassOf<AItemBase> ItemClass)
{
	for (const FItem& Item : Items)
	{
		if (Item.ItemClass == ItemClass)
		{
			return true;
		}
	}
	return false;
}

bool UInventoryComponent::HasItemByID(FGuid ItemID)
{
	for (FItem Item : Items)
	{
		if (Item.ItemID == ItemID)
		{
			return true;
		}
	}
	return false;
}

void UInventoryComponent::AddItem(AItemBase* InItem)
{
	if (!IsValid(InItem))
	{
		UE_LOG(LogPocket, Error, TEXT("Invalid Item passed to AddItem"));
		return;
	}

	const FDataTableRowHandle& ItemRow = InItem->ItemRow;

	const FItemRow* FoundRow = ItemRow.GetRow<FItemRow>(__FUNCTION__);

	if (FoundRow == nullptr)
	{
		UE_LOG(LogPocket, Error, TEXT("InventoryComponent: Failed to get Item data from Item Row Handle"));
		return;
	}
	
	FItem ItemData = FoundRow->Item;

	for (FItem& Item : Items)
	{
		if (Item.ItemID == ItemData.ItemID)
		{
			Item.Quantity += 1;
			return;
		}
	}
	Items.Add(ItemData);
}


void UInventoryComponent::RemoveItemByClass(TSubclassOf<AItemBase> ItemClass)
{
	int IndexToRemove = 0;
	bool ItemDepleted = false;
	for (int i = 0; i < Items.Num(); i++)
	{
		if (Items[i].ItemClass == ItemClass)
		{
			Items[i].Quantity -= 1;
			if (Items[i].Quantity <= 0)
			{
				IndexToRemove = i;
				ItemDepleted = true;
			}
			break;
		}
	}
	if (ItemDepleted)
	{
		Items.RemoveAt(IndexToRemove);
	}
}

void UInventoryComponent::PrintHeldItems()
{
	TStringBuilderWithBuffer<TCHAR, 256> StringBuilder;
	for (FItem Item : Items)
	{
		StringBuilder.Append(Item.ToString());
	}

	FString Result = StringBuilder.ToString();

	UE_LOG(LogPocket, Warning, TEXT("%s"), *Result);
}

FString FItem::ToString() const
{
	TStringBuilderWithBuffer<TCHAR, 256> StringBuilder;
	StringBuilder.Append(ItemName.ToString());
	StringBuilder.Append(TEXT("\n"));
	StringBuilder.Append(ItemID.ToString());
	StringBuilder.Append(TEXT("\n"));
	StringBuilder.Append(FString::FromInt(Quantity));
	StringBuilder.Append(TEXT("\n"));

	FString Result = StringBuilder.ToString();
	return Result;
}
