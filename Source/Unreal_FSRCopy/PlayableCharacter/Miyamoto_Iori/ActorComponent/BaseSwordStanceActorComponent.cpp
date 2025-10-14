// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayableCharacter/Miyamoto_Iori/ActorComponent/BaseSwordStanceActorComponent.h"
#include "PlayableCharacter/PlayableBaseCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"

// Sets default values for this component's properties
UBaseSwordStanceActorComponent::UBaseSwordStanceActorComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	NextAttackMontage = nullptr;
	NextAttackName = "";
	IsUseStance = false;
	// ...
}


// Called when the game starts
void UBaseSwordStanceActorComponent::BeginPlay()
{
	Super::BeginPlay();
	AmountAttackSpeed = BasicAttackSpeed;
	// ...
	HeavyAttackCount.Init(0, HeavyAttackMaxCount.Num());
}


// Called every frame
void UBaseSwordStanceActorComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UBaseSwordStanceActorComponent::PlayNormalAttackMontage()
{
	APlayableBaseCharacter* OwnerCharacter = Cast<APlayableBaseCharacter>(GetOwner());
	if(OwnerCharacter->GetMovementComponent()->IsFalling() == true ||
		OwnerCharacter->IsEvading() == true ||
		nullptr == NormalAttackMontage || OwnerCharacter->GetIsCombatMode() == false ||
		OwnerCharacter->GetBodyComponent()->GetAnimInstance()->Montage_IsPlaying(HeavyAttackMontage) == true)
		return;
	
	if (IsPossibleNextAttack == false && NormalAttackSectionIndex > 0)
	{
		IsPossibleNextAttack = true;
		NextAttackMontage = NormalAttackMontage;
		NextAttackName = NormalAttackSectionNames[NormalAttackSectionIndex];
		return;
	}

	if (OwnerCharacter->GetBodyComponent()->GetAnimInstance()->Montage_IsPlaying(NormalAttackMontage) == true)
		return;

	GEngine->AddOnScreenDebugMessage(2, 3.0f, FColor::Blue, TEXT("Play Normal Attack Montage"));
	OwnerCharacter->PlayMontageFullBody(NormalAttackMontage, GetAddCurNormalAttackSectionName(), AmountAttackSpeed);
}

void UBaseSwordStanceActorComponent::PlayHeavyAttackMontage()
{
	APlayableBaseCharacter* OwnerCharacter = Cast<APlayableBaseCharacter>(GetOwner());
	//기본 적인 강공격 가능 여부 확인
	if (OwnerCharacter->GetMovementComponent()->IsFalling() == true ||
		OwnerCharacter->IsEvading() == true ||
		nullptr == HeavyAttackMontage || OwnerCharacter->GetIsCombatMode() == false)
		return;

	//연타 강공격 가능 여부 확인
	//수정 예정
	if (HeavyAttackCount[NormalAttackSectionIndex] >= HeavyAttackMaxCount[NormalAttackSectionIndex])
		return;
	
	if (IsPossibleNextAttack == false && NormalAttackSectionIndex > 0)
	{
		IsPossibleNextAttack = true;
		NextAttackMontage = HeavyAttackMontage;
		NextAttackName = HeavyAttackSectionNames[NormalAttackSectionIndex];
		return;
	}
	if (OwnerCharacter->GetBodyComponent()->GetAnimInstance()->Montage_IsPlaying(HeavyAttackMontage) == true ||
		OwnerCharacter->GetBodyComponent()->GetAnimInstance()->Montage_IsPlaying(NormalAttackMontage) == true)
		return;
	//GEngine->AddOnScreenDebugMessage(2, 3.0f, FColor::Blue, TEXT("Play Heavy Attack Montage"));
	
	//몽타주에서 다음 몽타주로 갈때 소량의 프레임에서 
	if (IsPossibleNextAttack == true)
		return;

	//일회성 특수 공격력 초기화
	SpeicalAttackPower = 0;
	if (OwnerCharacter->PlayMontageFullBody(HeavyAttackMontage, GetCurHeavyAttackSectionName(), AmountAttackSpeed) == true)
	{
		GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Blue, TEXT("TTTTTPlay Heavy Attack Montage"));
		HeavyAttackCount[NormalAttackSectionIndex]++;
	}
}

void UBaseSwordStanceActorComponent::PlayTriggeredHeavyAttackMontage()
{
	APlayableBaseCharacter* OwnerCharacter = Cast<APlayableBaseCharacter>(GetOwner());

	if (OwnerCharacter->GetMovementComponent()->IsFalling() == true ||
		OwnerCharacter->IsEvading() == true ||
		nullptr == NormalAttackMontage || OwnerCharacter->GetIsCombatMode() == false)
		return;

	if (OwnerCharacter->GetBodyComponent()->GetAnimInstance()->Montage_IsPlaying(HeavyAttackMontage) == false)
		return;
	GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Blue, TEXT("SSSSSPlay Heavy Attack Montage"));
	if(IsCharging == false)
	{
		ChargeStartTime = GetWorld()->GetTimeSeconds();
		IsCharging = true;
	}
}

void UBaseSwordStanceActorComponent::PlayCompletedHeavyAttackMontage()
{
	APlayableBaseCharacter* OwnerCharacter = Cast<APlayableBaseCharacter>(GetOwner());
	if (OwnerCharacter->GetMovementComponent()->IsFalling() == true ||
		OwnerCharacter->IsEvading() == true ||
		nullptr == HeavyAttackMontage || OwnerCharacter->GetIsCombatMode() == false)
		return;

	//차징 몽타주가 안니면 리턴
	if (OwnerCharacter->GetBodyComponent()->GetAnimInstance()->Montage_IsPlaying(HeavyAttackMontage) == false)
		return;

	//차징 몽타주가 플레이 중이고, 연타 횟수가 최대 횟수를 초과하면 리턴
	if (OwnerCharacter->GetBodyComponent()->GetAnimInstance()->Montage_IsPlaying(HeavyAttackMontage) == true &&
		HeavyAttackCount[NormalAttackSectionIndex] >= HeavyAttackMaxCount[NormalAttackSectionIndex])
		return;

	IsCharging = false;
	CurrentChargeTime = GetWorld()->GetTimeSeconds() - ChargeStartTime;
	CurrentChargeTime = FMath::Clamp(CurrentChargeTime, 0.0f, MaxChargeTime);
}

FName UBaseSwordStanceActorComponent::GetAddCurNormalAttackSectionName()
{
	FName SectionName = NormalAttackSectionNames[NormalAttackSectionIndex];
	++NormalAttackSectionIndex;
	NormalAttackSectionIndex %= NormalAttackSectionNames.Num();
	return SectionName;
}

bool UBaseSwordStanceActorComponent::IsAttacking()
{
	APlayableBaseCharacter* OwnerCharacter = Cast<APlayableBaseCharacter>(GetOwner());
	if (OwnerCharacter->GetBodyComponent()->GetAnimInstance()->Montage_IsPlaying(NormalAttackMontage) == true ||
		OwnerCharacter->GetBodyComponent()->GetAnimInstance()->Montage_IsPlaying(HeavyAttackMontage) == true)
		return true;
	return false;
}

void UBaseSwordStanceActorComponent::PlayNextAttackMontage()
{
	if (IsPossibleNextAttack == false)
	{
		ResetAttackInfo();
		return;
	}
	GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Red, TEXT("Play Next Attack Montage"));
	APlayableBaseCharacter* OwnerCharacter = Cast<APlayableBaseCharacter>(GetOwner());
	OwnerCharacter->PlayMontageFullBody(NextAttackMontage, NextAttackName, AmountAttackSpeed);
	if(NextAttackMontage == HeavyAttackMontage)
	{
		GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Blue, TEXT("NNNNNPlay Heavy Attack Montage"));
		HeavyAttackCount[NormalAttackSectionIndex]++;
	}
	else if (NextAttackMontage == NormalAttackMontage)
	{
		GetAddCurNormalAttackSectionName();
	}
}

void UBaseSwordStanceActorComponent::ResetAttackInfo()
{
	NormalAttackSectionIndex = 0;
	for (int i = 0; i < HeavyAttackCount.Num(); ++i)
	{
		HeavyAttackCount[i] = 0;
	}
}

void UBaseSwordStanceActorComponent::ReleaseSwordStance()
{
	IsUseStance = false;
}

void UBaseSwordStanceActorComponent::InitSwordStance()
{
	IsUseStance = true;
	AmountAttackSpeed = BasicAttackSpeed;
}

int UBaseSwordStanceActorComponent::SwordStanceBeforeUpdateHp(int Damage)
{
	return Damage;
}

void UBaseSwordStanceActorComponent::SwordStanceAfterUpdateHp(int Damage)
{
}

int UBaseSwordStanceActorComponent::SwordStanceUpdateAttack()
{
	return 0;
}

bool UBaseSwordStanceActorComponent::GetIsPlayHeavyAttackMontage()
{
	return Cast<APlayableBaseCharacter>(GetOwner())->GetBodyComponent()->GetAnimInstance()->Montage_IsPlaying(HeavyAttackMontage);
}

