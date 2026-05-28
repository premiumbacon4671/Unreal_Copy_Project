// Fill out your copyright notice in the Description page of Project Settings.


#include "ActorComponent/InventoryComponent/InventoryComponent.h"

// Sets default values for this component's properties
UInventoryComponent::UInventoryComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}


// Called when the game starts
void UInventoryComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	InitializeInventory();
}


// Called every frame
void UInventoryComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UInventoryComponent::InitializeInventory()
{
	MasterInventoryMap.Empty();
	for (const auto& ItemData : AllGameItemDataBase)
	{
		if (ItemData)
		{
			//초기 아이템 수량 20개로 설정, 필요에 따라 조정 가능
			MasterInventoryMap.Add(ItemData->ItemID, FItemStack(ItemData, 20));
		}
	}
}

bool UInventoryComponent::AddItem(const FName& ItemID, int32 Quantity)
{
	if(Quantity <= 0)
		return false;
	if(MasterInventoryMap.Contains(ItemID))
	{
		MasterInventoryMap[ItemID].Quantity += Quantity;
		return true;
	}
	return false;
}

bool UInventoryComponent::ConsumeItem(const FName& ItemID, int32 Quantity)
{
	if(Quantity <= 0)
		return false;
	if (MasterInventoryMap.Contains(ItemID))
	{
		if(MasterInventoryMap[ItemID].Quantity >= Quantity)
		{
			MasterInventoryMap[ItemID].Quantity -= Quantity;
			return true;
		}
	}
	return false;
}

void UInventoryComponent::GetFilteredItemList(EItemMainType MainType, TArray<FItemStack>& OutItemList) const
{
	//제작예정
}

int32 UInventoryComponent::GetItemQuantity(const FName& ItemID) const
{
	if(MasterInventoryMap.Contains(ItemID))
	{
		return MasterInventoryMap[ItemID].Quantity;
	}
	return 0;
}

