// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PlayableCharacter/PlayableBaseCharacter.h"
#include "Miyamoto_Iori.generated.h"

/**
 * 
 */

enum SWORDSTANCE
{
	EARTH,
	FIRE,
	SWORDSTANCE_MAX
};

UCLASS()
class UNREAL_FSRCOPY_API AMiyamoto_Iori : public APlayableBaseCharacter
{
	GENERATED_BODY()
	
private:
	SWORDSTANCE CurSwordStance;
	TArray<class UBaseSwordStanceActorComponent*> SwordStanceComponents;
	TObjectPtr<class UBaseSwordStanceActorComponent> CurSwordStanceComponent;
#pragma region Component
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USkeletalMeshComponent> FirstWeaponCoverComponent;
public:
	AMiyamoto_Iori();
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	int GetCurSwordStance() const { return static_cast<int>(CurSwordStance); }
	void PlayEquipWeaponMontage() override;
	UBaseSwordStanceActorComponent* GetCurSwordStanceComponent() const { return CurSwordStanceComponent; }
};
