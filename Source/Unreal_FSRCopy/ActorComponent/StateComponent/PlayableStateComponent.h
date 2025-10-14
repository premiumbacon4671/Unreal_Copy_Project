// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ActorComponent/StateComponent/BaseStateComponent.h"
#include "PlayableStateComponent.generated.h"

/**
 * 
 */
UCLASS()
class UNREAL_FSRCOPY_API UPlayableStateComponent : public UBaseStateComponent
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(EditAnywhere, Category = "PlayableState")
	int Level{ 1 };
	UPROPERTY(EditAnywhere, Category = "PlayableState")
	int Experience{ 0 };
	UPROPERTY(EditAnywhere, Category = "PlayableState")
	int MaxExperience{ 0 };
	UPROPERTY(EditAnywhere, Category = "PlayableState")
	int Mat;
	UPROPERTY(EditAnywhere, Category = "PlayableState")
	int Tec;
	UPROPERTY(EditAnywhere, Category = "PlayableState")
	int Hiken{ 40 };
	UPROPERTY(EditAnywhere, Category = "PlayableState")
	int MaxHiken{ 100 };

	//특성(버프, 디버프, 검의 형)을 통해 얻는 가변 가능한 능력치
	UPROPERTY(EditAnywhere, Category = "PlayableState")
	int ExtraAttackPower{ 0 };
	UPROPERTY(EditAnywhere, Category = "PlayableState")
	int ExtraDefencePower{ 0 };
	

public:
	// Sets default values for this component's properties
	UPlayableStateComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	float GetHikentPercent() const { return static_cast<float>(Hiken) / static_cast<float>(MaxHiken); }
	void AddExtraAttackPower(int iExtraAttackPower) { ExtraAttackPower += iExtraAttackPower; }
	void SubExtraAttackPower(int iExtraAttackPower) { ExtraAttackPower -= iExtraAttackPower; if (ExtraAttackPower <= 0) ExtraAttackPower = 0; }
	int GetExtraAttackPower() const { return ExtraAttackPower; }
	int GetTotalAttackPower() const { return AttackPower + ExtraAttackPower; }
	void AddExtraDefencePower(int iExtraDefencePower) { ExtraDefencePower += iExtraDefencePower; }
	void SubExtraDefencePower(int iExtraDefencePower) { ExtraDefencePower -= iExtraDefencePower; if (ExtraDefencePower <= 0) ExtraDefencePower = 0; }
	int GetExtraDefencePower() const { return ExtraDefencePower; }
};
