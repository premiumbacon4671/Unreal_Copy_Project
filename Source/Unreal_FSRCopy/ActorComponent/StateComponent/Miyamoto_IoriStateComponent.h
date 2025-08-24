// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ActorComponent/StateComponent/PlayableStateComponent.h"
#include "Miyamoto_IoriStateComponent.generated.h"

/**
 * 
 */
UCLASS()
class UNREAL_FSRCOPY_API UMiyamoto_IoriStateComponent : public UPlayableStateComponent
{
	GENERATED_BODY()
private:
	UPROPERTY(EditAnywhere, Category = "PlayableState")
	int LinkSkill{ 70 };
	UPROPERTY(EditAnywhere, Category = "PlayableState")
	int LinkSkillMax{ 100 };
	UPROPERTY(EditAnywhere, Category = "PlayableState")
	int LinkSkillBall{ 1 };
	UPROPERTY(EditAnywhere, Category = "PlayableState")
	int LinkSkillBallMax{ 3 };

public:
	// Sets default values for this component's properties
	UMiyamoto_IoriStateComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	float GetLinkSkillPercent() const { return static_cast<float>(LinkSkill) / static_cast<float>(LinkSkillMax); }
	int GetLinkSkillBall() const { return LinkSkillBall; }
};
