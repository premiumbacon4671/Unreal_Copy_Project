// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PlayableCharacter/PlayableBaseCharacter.h"
#include "Miyamoto_Iori.generated.h"

/**
 * 
 */

UENUM(BlueprintType)
enum class ESWORDSTANCE : uint8
{
	EST_EARTH UMETA(DisplayName = "EarthStance"),
	EST_WATER UMETA(DisplayName = "WaterStance"),
	EST_FIRE UMETA(DisplayName = "FireStance"),
	EST_WIND UMETA(DisplayName = "WindStance"),
	EST_VOID UMETA(DisplayName = "VoidStance"),
	EST_MAX
};

UCLASS()
class UNREAL_FSRCOPY_API AMiyamoto_Iori : public APlayableBaseCharacter
{
	GENERATED_BODY()
	
private:
	ESWORDSTANCE CurSwordStance{ ESWORDSTANCE::EST_FIRE };
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sword Stance", meta = (AllowPrivateAccess = "true"))
	TArray<class UBaseSwordStanceActorComponent*> SwordStanceComponents;

	UPROPERTY(VisibleAnywhere, Category = "Sword Stance", meta = (AllowPrivateAccess = "true"))
	TMap<ESWORDSTANCE, bool> IsUnlockedSwordStance;
public:
	AMiyamoto_Iori();
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	ESWORDSTANCE GetCurSwordStance() const { return CurSwordStance; }
	void PlayEquipWeaponMontage() override;
	void WeaponEquip() override;
	void WeaponUnEquip() override;
	TMap<ESWORDSTANCE, bool> GetIsUnlockedSwordStance() const { return IsUnlockedSwordStance; }
};
