// Fill out your copyright notice in the Description page of Project Settings.


#include "ActorComponent/InventoryComponent/InventoryComponent.h"

#include "PlayableCharacter/PlayableBaseCharacter.h"
#include "PlayerState/FatePlayerState.h"
#include "ActorComponent/StateComponent/PlayableStateComponent.h"
#include "ActorComponent/ResonanceComponent/ResonanceComponent.h"

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
	for (const auto& Pair : AllGameItemDataBase)
	{
		UItemDataAsset* ItemData = Pair.Value;
		if (ItemData)
		{
			MasterInventoryMap.Add(Pair.Key, FItemStack(ItemData, 20));
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
		if(OnItemUpdated.IsBound())
		{
			OnItemUpdated.Broadcast(ItemID, MasterInventoryMap[ItemID].Quantity);
		}
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
			if (OnItemUpdated.IsBound())
			{
				OnItemUpdated.Broadcast(ItemID, MasterInventoryMap[ItemID].Quantity);
			}
			return true;
		}
	}
	return false;
}

void UInventoryComponent::GetMainTypeFilteredItemList(EItemMainType MainType, TArray<FItemStack>& OutItemList) const
{
	//제작예정
}

void UInventoryComponent::GetSubTypeFilteredItemList(EItemSubType InSubType, TArray<FItemStack>& OutItemList) const
{
	OutItemList.Empty();

	for (const auto& [ItemID, ItemStack] : MasterInventoryMap)
	{
		if (ItemStack.ItemData == nullptr)
			continue;
		if (ItemStack.ItemData->SubType == InSubType)
			OutItemList.Add(ItemStack);
	}
}

int32 UInventoryComponent::GetItemQuantity(const FName& ItemID) const
{
	if(MasterInventoryMap.Contains(ItemID))
	{
		return MasterInventoryMap[ItemID].Quantity;
	}
	return 0;
}

bool UInventoryComponent::CanUseRecoverItem(const FName& InItemID) const
{
	if (MasterInventoryMap.Contains(InItemID) == false)
		return false;
	const FItemStack& TargetStack = MasterInventoryMap[InItemID];
	if (TargetStack.Quantity <= 0)
		return false;
	UItemDataAsset* DataAsset = TargetStack.ItemData.Get();
	if (nullptr == DataAsset)
		return false;
	AFatePlayerState* OwnerPS = Cast<AFatePlayerState>(GetOwner());
	if (nullptr == OwnerPS)
		return false;
	APlayableBaseCharacter* CurPlayableCharacter = Cast<APlayableBaseCharacter>(OwnerPS->GetPawn());
	if (nullptr == CurPlayableCharacter)
		return false;
	switch (DataAsset->FoodEffect.StatType)
	{
	case EFoodStatType::HP:
		if (CurPlayableCharacter->GetStatusComponent()->GetCurHP() >= CurPlayableCharacter->GetStatusComponent()->GetMaxHP())
			return false;
		break;
	case EFoodStatType::Hiken:
		if (CurPlayableCharacter->GetStatusComponent()->GetHiken() >= CurPlayableCharacter->GetStatusComponent()->GetMaxHiken())
			return false;
		break;
	case EFoodStatType::LinkSkill:
		if (OwnerPS->ResonanceComponent->GetLinkSkillGauge() >= OwnerPS->ResonanceComponent->GetMaxLinkSkillGauge())
			return false;
		break;
	}
	return true;
}

bool UInventoryComponent::UseRecoverItem(const FName& InItemID)
{
	if (CanUseRecoverItem(InItemID) == false)
		return false;
	if (ConsumeItem(InItemID, 1) == false)
		return false;
	UItemDataAsset* DataAsset = MasterInventoryMap[InItemID].ItemData.Get();
	AFatePlayerState* OwnerPS = Cast<AFatePlayerState>(GetOwner());
	APlayableBaseCharacter* CurPlayableCharacter = Cast<APlayableBaseCharacter>(OwnerPS->GetPawn());

	if (OwnerPS && CurPlayableCharacter && DataAsset)
	{
		switch (DataAsset->FoodEffect.StatType)
		{
		case EFoodStatType::HP:
			CurPlayableCharacter->GetStatusComponent()->RecoverHP(DataAsset->FoodEffect.EffectValue);
			break;
		case EFoodStatType::Hiken:
			CurPlayableCharacter->GetStatusComponent()->RecoverHiken(DataAsset->FoodEffect.EffectValue);
			break;
		case EFoodStatType::LinkSkill:
			OwnerPS->ResonanceComponent->RecoverLinkSkillGauge(DataAsset->FoodEffect.EffectValue);
		default:
			break;
		}
	}
	return true;
}
