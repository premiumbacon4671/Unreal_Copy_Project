// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ActorComponent/StateComponent/BaseStateComponent.h"
#include "MonsterStateComponent.generated.h"

/**
 * 
 */
UENUM(BlueprintType)
enum class EMonsterTier : uint8
{
	Normal,
	Elite,
	Boss
};

USTRUCT(BlueprintType)
struct FMonsterStat : public FBaseStat
{
	GENERATED_BODY()
public:
	FMonsterStat() :
		MonsterTier(EMonsterTier::Normal)
	{}
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MonsterStat")
	EMonsterTier MonsterTier;
};

UCLASS()
class UNREAL_FSRCOPY_API UMonsterStateComponent : public UBaseStateComponent
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(EditAnywhere, Category = "MonsterStat")
	FMonsterStat MonsterStat;

public:
	UMonsterStateComponent();

protected:
	virtual void BeginPlay() override;

public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	virtual FBaseStat& GetStat() override { return MonsterStat; }
	virtual const FBaseStat& GetStat() const override { return MonsterStat; }
	virtual FMonsterStat& GetMonsterStat() { return MonsterStat; }
	virtual const FMonsterStat& GetMonsterStat() const { return MonsterStat; }

	EMonsterTier GetMonsterTier() { return GetMonsterStat().MonsterTier; }
};
