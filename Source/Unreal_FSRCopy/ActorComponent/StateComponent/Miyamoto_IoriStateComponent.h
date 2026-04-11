// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ActorComponent/StateComponent/PlayableStateComponent.h"
#include "Miyamoto_IoriStateComponent.generated.h"

/**
 * 
 */

USTRUCT(BlueprintType)
struct FMiyamoto_IoriStat : public FPlayableStat
{
	GENERATED_BODY()
public:
	FMiyamoto_IoriStat() :
		MaxLinkSkill(0.0f),
		LinkSkill(0.0f),
		MaxLinkSkillBall(0),
		LinkSkillBall(0){
	}
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MiyamotoIoriStat")
	float MaxLinkSkill;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MiyamotoIoriStat")
	float LinkSkill;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MiyamotoIoriStat")
	int MaxLinkSkillBall;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MiyamotoIoriStat")
	int LinkSkillBall;
};

UCLASS()
class UNREAL_FSRCOPY_API UMiyamoto_IoriStateComponent : public UPlayableStateComponent
{
	GENERATED_BODY()
public:
	// Sets default values for this component's properties
	UMiyamoto_IoriStateComponent();
protected:
	UPROPERTY(EditAnywhere, Category = "Miyamoto_IoriStat")
	FMiyamoto_IoriStat Miyamoto_IoriStat;
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	virtual FBaseStat& GetStat() override { return Miyamoto_IoriStat; }
	virtual const FBaseStat& GetStat() const override { return Miyamoto_IoriStat; }
	virtual FPlayableStat& GetPlayableStat() override { return Miyamoto_IoriStat; }
	virtual const FPlayableStat& GetPlayableStat() const override { return Miyamoto_IoriStat; }
	virtual FMiyamoto_IoriStat& GetMiyamoto_IoriStat() { return Miyamoto_IoriStat; }
	virtual const FMiyamoto_IoriStat& GetMiyamoto_IoriStat() const { return Miyamoto_IoriStat; }

	virtual void InitState(const FBaseStat& InBaseStat) override;

	float GetLinkSkillPercent() const { return static_cast<float>(GetMiyamoto_IoriStat().LinkSkill) / static_cast<float>(GetMiyamoto_IoriStat().MaxLinkSkill); }
	int GetLinkSkillBall() const { return GetMiyamoto_IoriStat().LinkSkillBall; }
};
