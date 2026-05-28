// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "ItemDataAsset.generated.h"

/**
 * 
 */

UENUM(BlueprintType)
enum class EItemMainType : uint8
{
	Tool UMETA(DisplayName = "Tool"),
	Material UMETA(DisplayName = "Material"),
	Gem UMETA(DisplayName = "Gem")
};

UENUM(BlueprintType)
enum class EItemSubType : uint8
{
	None UMETA(DisplayName = "None"),

	Tool_Food UMETA(DisplayName = "Food"),
	Tool_Valuable UMETA(DisplayName = "Valuable"),

	Material_Magic UMETA(DisplayName = "Magic Material"),
	Material_Monster UMETA(DisplayName = "Monster Material")
};

UENUM(BlueprintType)
enum class EItemRarity : uint8
{
	Star_1 UMETA(DisplayName = "1 Star"),
	Star_2 UMETA(DisplayName = "2 Star"),
	Star_3 UMETA(DisplayName = "3 Star"),
	Star_4 UMETA(DisplayName = "4 Star")
};

UENUM(BlueprintType)
enum class EFoodStatType : uint8
{
	HP UMETA(DisplayName = "Health"),
	LinkSkill UMETA(DisplayName = "Resonance Gauge"),
	Hiken UMETA(DisplayName = "Hiken Gauge")
};

USTRUCT(BlueprintType)
struct FFoodTypeEffect
{
	GENERATED_BODY()
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Food | Effect")
	EFoodStatType StatType = EFoodStatType::HP;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Food | Effect")
	float EffectValue = 0.f;
};

USTRUCT(BlueprintType)
struct FItemStack
{
	GENERATED_BODY()
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Item Stack")
	TObjectPtr<class UItemDataAsset> ItemData;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Item Stack")
	int32 Quantity = 1;

	bool IsEmpty() const { return ItemData == nullptr || Quantity <= 0; }
	FItemStack() {}
	FItemStack(UItemDataAsset* InItemData, int32 InQuantity)
		: ItemData(InItemData), Quantity(InQuantity) {}
};

UCLASS()
class UNREAL_FSRCOPY_API UItemDataAsset : public UPrimaryDataAsset
{
	GENERATED_BODY()
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Item | Identity")
	FName ItemID;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Item | Identity")
	FText ItemName;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Item | Identity", meta = (MultiLine = true));
	FText ItemDescription;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Item | Category")
	EItemMainType MainType;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Item | Category")
	EItemSubType SubType;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Item | Policy")
	EItemRarity Rarity;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Item | Policy")
	int32 MaxStackCount;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Item | Visual")
	TObjectPtr<UTexture2D> ItemIcon;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Item | Effect", meta = (EditCondition = "MainType == EItemMainType::Tool", EditConditionHides))
	FFoodTypeEffect FoodEffect;

};
