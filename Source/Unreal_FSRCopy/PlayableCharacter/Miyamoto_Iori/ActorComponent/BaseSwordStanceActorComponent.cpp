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
	// ...
}


// Called when the game starts
void UBaseSwordStanceActorComponent::BeginPlay()
{
	Super::BeginPlay();

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
		//isUseableNormalAttack == false)
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
	if (OwnerCharacter->PlayMontageFullBody(NormalAttackMontage, GetAddCurNormalAttackSectionName()) == true)
	{

	}
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

	if (OwnerCharacter->PlayMontageFullBody(HeavyAttackMontage, GetCurHeavyAttackSectionName()) == true)
	{
		GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Blue, TEXT("TTTTTPlay Heavy Attack Montage"));
		HeavyAttackCount[NormalAttackSectionIndex]++;
		//isUseableHeavyAttack = false;
	}
}

void UBaseSwordStanceActorComponent::PlayTriggeredHeavyAttackMontage()
{
	APlayableBaseCharacter* OwnerCharacter = Cast<APlayableBaseCharacter>(GetOwner());

	if (OwnerCharacter->GetMovementComponent()->IsFalling() == true ||
		OwnerCharacter->IsEvading() == true ||
		nullptr == NormalAttackMontage || OwnerCharacter->GetIsCombatMode() == false)
		//IsPlayingNormalAttack == true)
		//isUseableNormalAttack == false)
		return;

	//GEngine->AddOnScreenDebugMessage(3, 3.0f, FColor::Green, TEXT("Play Triggered Heavy Attack Montage"));
	//OwnerCharacter->GetBodyComponent()->GetAnimInstance()->l
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
	OwnerCharacter->PlayMontageFullBody(NextAttackMontage, NextAttackName);
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

//void UBaseSwordStanceActorComponent::ResetNormalAttack()
//{
//	ResetisUseableNormalAttack();
//	ResetNormalAttackSectionIndex();
//}
//
//void UBaseSwordStanceActorComponent::ResetHeavyAttack()
//{
//	ResetisUseableHeavyAttack();
//	ResetNormalAttackSectionIndex();
//	for (int i = 0; i < HeavyAttackCount.Num(); ++i)
//	{
//		HeavyAttackCount[i] = 0;
//	}
//}

