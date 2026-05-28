// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "DataAsset/PrimaryDataAsset/ItemDataAsset/ItemDataAsset.h"
#include "InventoryComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UNREAL_FSRCOPY_API UInventoryComponent : public UActorComponent
{
	GENERATED_BODY()
private:
	//플레이어 소지품
	UPROPERTY(SaveGame)
	TMap<FName, FItemStack> MasterInventoryMap;

	//소지수 0개 빈 방 생성
	//게임 내 존재하는 모든 아이템 데이터베이스, 아이템 생성 시 참조
	//정렬 순서
	UPROPERTY(EditDefaultsOnly, Category = "Inventory | Setup")
	TArray<TObjectPtr<UItemDataAsset>> AllGameItemDataBase;
public:	
	// Sets default values for this component's properties
	UInventoryComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void InitializeInventory();
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	bool AddItem(const FName& ItemID, int32 Quantity);
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	bool ConsumeItem(const FName& ItemID, int32 Quantity);
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void GetFilteredItemList(EItemMainType MainType, TArray<FItemStack>& OutItemList) const;
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	int32 GetItemQuantity(const FName& ItemID) const;
};
