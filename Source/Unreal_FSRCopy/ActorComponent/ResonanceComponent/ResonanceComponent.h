// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ResonanceComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCalculateLinkSkillGauge, float, Percent);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCalculateLinkBall, int32, Count);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FServantGaugeChanged, int, Index,float, CurrentPercent);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnServantGaugeDepleted);

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


	UPROPERTY(EditAnywhere, Category = "Resonance | Gauge")
	float MaxSaberGauge = { 100.0f };
	float CurrentSaberGauge = { 0.0f };
	//세이버 조종 여부
	//bool bIsSaberActive{ false };
	

protected:
	//보유하고 있는 서번트
	UPROPERTY(VisibleAnywhere, Category = "Resonance | Gauge")
	TMap<FName, float> ServantSwapGaugeMap;
	//현재 사용할 서번트
	UPROPERTY(VisibleAnywhere, Category = "Resonance | Party")
	TArray<FName> ActivePartyServants;
	UPROPERTY(VisibleAnywhere, Category = "Resonance | State")
	FName CurrentActiveServantName{ NAME_None };
	UPROPERTY(EditAnywhere, Category = "Resonance | Gauge")
	float MaxServantSwapGauge = { 60.0f };
	UPROPERTY(EditAnywhere, Category = "Resonance | Gauge")
	float SwapSageDecreaseRate = { 6.0f };

	UPROPERTY(EditAnywhere, Category = "GainMultiplier")
	float LinkSkillGaugeGainMultiplier{ 10.0f };
	UPROPERTY(EditAnywhere, Category = "GainMultiplier")
	float ServantGaugeGainMultiplier{ 100.0f };

public:	
	// Sets default values for this component's properties
	UResonanceComponent();
	FOnCalculateLinkSkillGauge OnCalculateLinkSkillGauge;
	FOnCalculateLinkBall OnCalculateLinkBall;
	FServantGaugeChanged OnServantGaugeChanged;
	FOnServantGaugeDepleted OnServantGaugeDepleted;
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

#pragma region ServantSwap
	void AddSaberGauge(float Amount);
	void ConsumeSaberGauge(float Amount);
	float GetSaberGaugePercent() const { return MaxSaberGauge > 0 ? CurrentSaberGauge / MaxSaberGauge : 0.0f; }
	//bool GetSaberActive() const { return bIsSaberActive; }
	//void SetSaberActive(bool bActive) { bIsSaberActive = bActive; }

	void RegisterServantToParty(FName ServantName);
	void SetSecondaryPartyServant(FName ServantName);
	void CalculateServnatGauge(int AmountDamage, float TargetMaxHP);
	void AddSwapGauge(float Amount);
	void SetActiveServant(FName ServantName);
	bool IsSwapAvailable(FName ServantName) const;
	float GetSwapGaugePercent(FName ServantName) const;
	int GetActivePartyServantIndex(FName ServantName) const;
	const TArray<FName>& GetActivePartyServants() const { return ActivePartyServants; }
	FName GetCurrentActiveServant() const { return CurrentActiveServantName; }
#pragma endregion
};
