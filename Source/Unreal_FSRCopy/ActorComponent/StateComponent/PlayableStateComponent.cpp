// Fill out your copyright notice in the Description page of Project Settings.


#include "ActorComponent/StateComponent/PlayableStateComponent.h"
#include "PlayableCharacter/PlayableBaseCharacter.h"

#include "NiagaraComponent.h"

UPlayableStateComponent::UPlayableStateComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UPlayableStateComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UPlayableStateComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UPlayableStateComponent::ConsumeHike(float AmountHiken)
{
	GetPlayableStat().Hiken -= AmountHiken;
	if (OnCalculateHikenGauge.IsBound())
		OnCalculateHikenGauge.Broadcast(GetHikenPercent());
}

void UPlayableStateComponent::RecoverHiken(float AmountHiken)
{
	GetPlayableStat().Hiken = FMath::Clamp(GetPlayableStat().Hiken + AmountHiken, 0.0f, GetPlayableStat().MaxHiken);
	if (OnCalculateHikenGauge.IsBound())
		OnCalculateHikenGauge.Broadcast(GetHikenPercent());
}

void UPlayableStateComponent::InitState(const FBaseStat& InBaseStat)
{
	PlayableStat = static_cast<const FPlayableStat&>(InBaseStat);
	OnInitializedStat.Broadcast(Cast<APlayableBaseCharacter>(GetOwner()));
}

void UPlayableStateComponent::CalculateHikenGauge(float AmountDamage, float TargetMaxHP)
{
	if (TargetMaxHP <= 0.0f)
		return;

	float GaugeIncrease = (AmountDamage / TargetMaxHP) * HikenGainMultiplier;
	GetPlayableStat().Hiken += GaugeIncrease;

	//공명 구슬 획득 제작 예정

	if (GetPlayableStat().Hiken >= GetPlayableStat().MaxHiken)
		GetPlayableStat().Hiken = FMath::Clamp(GetPlayableStat().Hiken, 0.0f, GetPlayableStat().MaxHiken);

	if (OnCalculateHikenGauge.IsBound())
		OnCalculateHikenGauge.Broadcast(GetHikenPercent());
}

void UPlayableStateComponent::ApplyAttackBuff(float BonusMultiplier, float Duration, const TArray<UNiagaraComponent*>& InBuffVFXs)
{
	if (!GetWorld())
		return;
	int32 BaseAtk = GetPlayableStat().AttackPower;
	int32 BuffAmount = FMath::RoundToInt32(BaseAtk * BonusMultiplier);
	if (BuffAmount <= 0)
		return;
	AddExtraAttackPower(BuffAmount);
	for (UNiagaraComponent* VFX : ActiveBuffVFXs)
	{
		if (VFX)
			VFX->Deactivate();
	}
	ActiveBuffVFXs.Empty();
	for (UNiagaraComponent* NewVFX : InBuffVFXs)
	{
		if (NewVFX)
			ActiveBuffVFXs.Add(NewVFX);
	}
	GEngine->AddOnScreenDebugMessage(-1, 2, FColor::Magenta, FString::Printf(TEXT("버프 발동! 증가량: %d | 현재 총 공격력: %d"), BuffAmount, GetTotalAttackPower()));
	
	FTimerDelegate BuffDelegate;

	BuffDelegate.BindUFunction(this, FName("RemoveAttackBuff"), BuffAmount);
	GetWorld()->GetTimerManager().SetTimer(BuffTimerHandle, BuffDelegate, Duration, false);
}

void UPlayableStateComponent::RemoveAttackBuff(int32 BuffAmount)
{
	SubExtraAttackPower(BuffAmount);
	GEngine->AddOnScreenDebugMessage(-1, 2, FColor::Magenta, FString::Printf(TEXT("버프 종료! 감소량: %d | 현재 총 공격력: %d"), BuffAmount, GetTotalAttackPower()));
	for (UNiagaraComponent* VFX : ActiveBuffVFXs)
	{
		if (VFX) VFX->Deactivate();
	}
	ActiveBuffVFXs.Empty();
}
