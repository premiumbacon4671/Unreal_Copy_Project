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
	EST_FIRE UMETA(DisplayName = "FireStance"),
	EST_MAX
};

UCLASS()
class UNREAL_FSRCOPY_API AMiyamoto_Iori : public APlayableBaseCharacter
{
	GENERATED_BODY()
	
private:
	ESWORDSTANCE CurSwordStance;
	UPROPERTY(EditAnywhere, Instanced, Category = "Sword Stance")
	TArray<class UBaseSwordStanceActorComponent*> SwordStanceComponents;
public:
	AMiyamoto_Iori();
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	ESWORDSTANCE GetCurSwordStance() const { return CurSwordStance; }
	void PlayEquipWeaponMontage() override;
	void WeaponEquip() override;
	void WeaponUnEquip() override;
};
