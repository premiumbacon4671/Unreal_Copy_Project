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
	ESWORDSTANCE NextSwordStance;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sword Stance", meta = (AllowPrivateAccess = "true"))
	TArray<class UBaseSwordStanceActorComponent*> SwordStanceComponents;
	UPROPERTY(EditAnywhere, Category = "Sword Stance")
	TObjectPtr<UAnimMontage> NextMontage;

public:
	AMiyamoto_Iori();
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	ESWORDSTANCE GetCurSwordStance() const { return CurSwordStance; }
	void PlayEquipWeaponMontage() override;
	void WeaponEquip() override;
	void WeaponUnEquip() override;
	//TMap<ESWORDSTANCE, bool> GetIsUnlockedSwordStance() const { return IsUnlockedSwordStance; }

	bool GetIsUnlockSwordStance(ESWORDSTANCE SwordStance) const;
	void ChangeSwordStance(ESWORDSTANCE SwordStance);
	void PostInitializeComponents() override;
	UFUNCTION()
	void UnEquipMontageStarted(UAnimMontage* Montage);
	UFUNCTION()
	void UnEquipMontageEnded(UAnimMontage* Montage, bool bInterrupted);
};
