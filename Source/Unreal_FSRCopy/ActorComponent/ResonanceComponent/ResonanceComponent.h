// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ResonanceComponent.generated.h"

DECLARE_DELEGATE_OneParam(FOnCalculateLinkSkillGauge, float)
DECLARE_DELEGATE_OneParam(FOnCalculateLinkBall, int32)

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UNREAL_FSRCOPY_API UResonanceComponent : public UActorComponent
{
	GENERATED_BODY()

private:
	float LinkSkillGauge;
	float MaxLinkSkillGauge;
	int32 LinkBall;
	int32 MaxLinkBall;
	//수	정 예정
	//서번트 활성화 여부, 서번트가 활성화되어야 공명 시스템이 작동하도록 하기 위해 추가
	//현재 서번트 미구현 상태이므로, 일단 true로 초기화해둠
	bool bIsServantActive{ true };

protected:
	UPROPERTY(EditAnywhere, Category = "GainMultiplier")
	float LinkSkillGaugeGainMultiplier{ 10.0f };

public:	
	// Sets default values for this component's properties
	UResonanceComponent();
	FOnCalculateLinkSkillGauge OnCalculateLinkSkillGauge;
	FOnCalculateLinkBall OnCalculateLinkBall;
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void RecoverLinkSkillGauge(float Amount);
	void CalculateLinkSkillGauge(int AmountDamage, float TargetMaxHP);
	bool ConsumeLinkBall(int32 Count);

	void InitResonance(float fMaxLinkSkillGauge, float fLinkSkillGauge, int32 iMaxLinkBall, int32 iLinkBall);

	float GetLinkSkillGaugePercentage() const { return MaxLinkSkillGauge > 0 ? LinkSkillGauge / MaxLinkSkillGauge : 0.0f; }
	float GetLinkSkillGauge() const { return LinkSkillGauge; }
	float GetMaxLinkSkillGauge() const { return MaxLinkSkillGauge; }
	int32 GetLinkBall() const { return LinkBall; }

	void SetServantActive(bool bActive) { bIsServantActive = bActive; }
	bool GetServantActive() const { return bIsServantActive; }
};
