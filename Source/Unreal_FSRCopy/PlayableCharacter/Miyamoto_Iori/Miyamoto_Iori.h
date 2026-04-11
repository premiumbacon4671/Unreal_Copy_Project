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
	ESWORDSTANCE eCurSwordStance{ ESWORDSTANCE::EST_FIRE };
	ESWORDSTANCE eNextSwordStance;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sword Stance", meta = (AllowPrivateAccess = "true"))
	TArray<class UBaseSwordStanceActorComponent*> SwordStanceComponents;
	//NextMontage 실험 코드
	/*UPROPERTY(EditAnywhere, Category = "Sword Stance")
	TObjectPtr<UAnimMontage> NextMontage;*/


	bool bRetryEquip = { false };
	void PlayEquipWeaponStateMontage_New(bool bIsEquip) override;
public:
	AMiyamoto_Iori();
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	ESWORDSTANCE GeteCurSwordStance() const { return eCurSwordStance; }

	virtual void InitializeStatus() override;

	void PlayEquipWeaponMontage() override;
	void PlayUnEquipWeaponMontage();
	
	void WeaponEquip() override;
	void WeaponUnEquip() override;
	//TMap<ESWORDSTANCE, bool> GetIsUnlockedSwordStance() const { return IsUnlockedSwordStance; }

	bool GetIsUnlockSwordStance(ESWORDSTANCE SwordStance) const;
	void ChangeSwordStance(ESWORDSTANCE SwordStance);
	void PostInitializeComponents() override;
	UFUNCTION()
	void UnEquipMontageStarted(UAnimMontage* Montage);


	void UnEquipMontageEnded(UAnimMontage* Montage, bool bInterrupted) override;

	//모든 몽타주가 끝날 때 호출되는 함수
	//부모 클래스로 이동 완료
	void EquipMontageEnded(UAnimMontage* Montage, bool bInterrupted) override;


protected:
	void ProcessMontageEndedGeneral(UAnimMontage* Montage, bool bInterrupted) override;
	virtual void Landed(const FHitResult& Hit) override;
};
