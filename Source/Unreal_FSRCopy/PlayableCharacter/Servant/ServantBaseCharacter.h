// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PlayableCharacter/PlayableBaseCharacter.h"
#include "ServantBaseCharacter.generated.h"

/**
 * 
 */
UCLASS()
class UNREAL_FSRCOPY_API AServantBaseCharacter : public APlayableBaseCharacter
{
	GENERATED_BODY()
protected:

	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	bool bStartWithWeaponHidden = false;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Data")
	FName ServantRowName;
public:
	// 단독 행동 중인지 여부
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Servant|State")
	bool bIsIndependentAction = false;

protected:
	virtual void BeginPlay() override;

public:
	AServantBaseCharacter();
	virtual void Tick(float DeltaTime) override;
	virtual void InitializeStatus() override;

	UFUNCTION(BlueprintCallable, Category = "Weapon")
	void SetWeaponVisibility(bool bVisible);

	virtual void WeaponEquip() override;
	virtual void WeaponUnEquip() override;
};
