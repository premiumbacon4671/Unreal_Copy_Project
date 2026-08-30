// Fill out your copyright notice in the Description page of Project Settings.


#include "ActorComponent/StateComponent/BaseStateComponent.h"

#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

// Sets default values for this component's properties
UBaseStateComponent::UBaseStateComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UBaseStateComponent::BeginPlay()
{
	Super::BeginPlay();
	// ...
	if (ACharacter* OwnerChar = Cast<ACharacter>(GetOwner()))
	{
		float test = OwnerChar->GetCharacterMovement()->MaxWalkSpeed;
		GetStat().CurrentBaseSpeed = test;
	}
	
}

void UBaseStateComponent::RefreshMovementSpeed()
{
	if (ACharacter* OwnerChar = Cast<ACharacter>(GetOwner()))
	{
		OwnerChar->GetCharacterMovement()->MaxWalkSpeed = GetStat().CurrentBaseSpeed * GetStat().MoveSpeedMultiplier;
	}
}


// Called every frame
void UBaseStateComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UBaseStateComponent::RecoverHP(float RecoverAmount)
{
	if (RecoverAmount <= 0.0)
		return;
	GetStat().CurHP = FMath::Clamp(GetStat().CurHP + RecoverAmount, 0.0f, GetStat().MaxHP);

	if (OnUpdateHpSignature.IsBound())
		OnUpdateHpSignature.Broadcast(this, GetHPPercent());
}

void UBaseStateComponent::TakeDamage(int DamageAmount)
{
	if(DamageAmount <= 0 || GetStat().CurHP <= 0)
		return;

	GetStat().CurHP = FMath::Max(0, (GetStat().CurHP - DamageAmount));

	if(OnUpdateHpSignature.IsBound())
		OnUpdateHpSignature.Broadcast(this, GetHPPercent());
}

void UBaseStateComponent::InitState(const FBaseStat& InBaseStat)
{
	GetStat() = InBaseStat; 
	if (ACharacter* OwnerChar = Cast<ACharacter>(GetOwner()))
	{
		float test = OwnerChar->GetCharacterMovement()->MaxWalkSpeed;
		GetStat().CurrentBaseSpeed = test;
	}
}

void UBaseStateComponent::SetBaseWalkSpeed(float NewBaseSpeed)
{
	GetStat().CurrentBaseSpeed = NewBaseSpeed;
	RefreshMovementSpeed();
}

void UBaseStateComponent::ApplyDebuff(const FDebuffData& Debuff)
{
	if (Debuff.DebuffType == EDebuffType::Slow)
	{
		float NewMultiplier = 1.0f - Debuff.Value;
		if (GetStat().MoveSpeedMultiplier < NewMultiplier && GetStat().MoveSpeedMultiplier != 1.0f)
		{
			return;
		}

		GetStat().MoveSpeedMultiplier = NewMultiplier;
		RefreshMovementSpeed();
		GetWorld()->GetTimerManager().SetTimer(
			DebuffTimerHandles.FindOrAdd(EDebuffType::Slow),
			FTimerDelegate::CreateWeakLambda(this, [this, Type = Debuff.DebuffType]()
				{
					GetStat().MoveSpeedMultiplier = 1.0f;
					RefreshMovementSpeed();
					DebuffTimerHandles.Remove(Type);
				}),
			Debuff.Duration,
			false
		);
	}
}

void UBaseStateComponent::AddOnHitDebuffBuff(const FDebuffData& DebuffToApply, float BuffDuration)
{
	int32 ExistingIndex = ActiveOnHitDebuffs.IndexOfByPredicate([&DebuffToApply](const FDebuffData& Item) {
		return Item.DebuffType == DebuffToApply.DebuffType;
		});
	if (ExistingIndex != INDEX_NONE)
	{
		ActiveOnHitDebuffs[ExistingIndex] = DebuffToApply;
	}
	else
	{
		ActiveOnHitDebuffs.Add(DebuffToApply);
	}
	FTimerHandle& TargetHandle = OnHitBuffTimerHandles.FindOrAdd(DebuffToApply.DebuffType);
	//보유 중인 주는 디버프 목록 제거
	GetWorld()->GetTimerManager().SetTimer(
		TargetHandle,
		FTimerDelegate::CreateWeakLambda(this, [this, DebuffType = DebuffToApply.DebuffType]()
			{
				ActiveOnHitDebuffs.RemoveAll([DebuffType](const FDebuffData& Item) {
					return Item.DebuffType == DebuffType;
					});

				OnHitBuffTimerHandles.Remove(DebuffType);
			}),
		BuffDuration,
		false
	);
}

