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
	eNextAttackType = EATTACKTYPE::AT_MAX;
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

	if (IsInputPressed == true && IsPossibleNextAttack == false)
	{
		SetIsPossibleNextAttack(EATTACKTYPE::AT_NORMALATTACK);
		return;
	}

	//if (OwnerCharacter->GetBodyComponent()->GetAnimInstance()->Montage_IsPlaying(NormalAttackMontage) == true)
	if (IsPlayingNormalAttack == true)
		return;

	GEngine->AddOnScreenDebugMessage(2, 3.0f, FColor::Blue, TEXT("Play Normal Attack Montage"));
	if (OwnerCharacter->PlayMontageFullBody(NormalAttackMontage, GetAddCurNormalAttackSectionName()) == true)
	{
		//isUseableNormalAttack = false;
		IsPlayingNormalAttack = true;
	}
}

void UBaseSwordStanceActorComponent::PlayHeavyAttackMontage()
{
	APlayableBaseCharacter* OwnerCharacter = Cast<APlayableBaseCharacter>(GetOwner());
	//기본 적인 강공격 가능 여부 확인
	if (OwnerCharacter->GetMovementComponent()->IsFalling() == true ||
		OwnerCharacter->IsEvading() == true ||
		nullptr == HeavyAttackMontage || OwnerCharacter->GetIsCombatMode() == false)
		//OwnerCharacter->GetBodyComponent()->GetAnimInstance()->Montage_IsPlaying(NormalAttackMontage) == true)
		//isUseableNormalAttack == false ||
		//isUseableHeavyAttack == false)
		return;

	//연타 강공격 가능 여부 확인
	//수정 예정
	if (HeavyAttackCount[NormalAttackSectionIndex] >= HeavyAttackMaxCount[NormalAttackSectionIndex])
		return;

	//if (OwnerCharacter->GetBodyComponent()->GetAnimInstance()->Montage_IsPlaying(HeavyAttackMontage) == true &&
	if (IsInputPressed == true && IsPossibleNextAttack == false)
	{
		SetIsPossibleNextAttack(EATTACKTYPE::AT_HEAVYATTACK);
		return;
	}
	//Montage_IsPlaying()이 너무 빨리 false가 되어 중복 재생됨
	if (IsPlayingHeavyAttack == true)
		return;

	//GEngine->AddOnScreenDebugMessage(2, 3.0f, FColor::Blue, TEXT("Play Heavy Attack Montage"));
	
	if (OwnerCharacter->PlayMontageFullBody(HeavyAttackMontage, GetCurHeavyAttackSectionName()) == true)
	{
		//isUseableHeavyAttack = false;
	}
}

void UBaseSwordStanceActorComponent::PlayTriggeredHeavyAttackMontage()
{
	APlayableBaseCharacter* OwnerCharacter = Cast<APlayableBaseCharacter>(GetOwner());

	if (OwnerCharacter->GetMovementComponent()->IsFalling() == true ||
		OwnerCharacter->IsEvading() == true ||
		nullptr == NormalAttackMontage || OwnerCharacter->GetIsCombatMode() == false ||
		IsPlayingNormalAttack == true)
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
		nullptr == HeavyAttackMontage || OwnerCharacter->GetIsCombatMode() == false ||
		OwnerCharacter->GetBodyComponent()->GetAnimInstance()->Montage_IsPlaying(NormalAttackMontage) == true ||
		OwnerCharacter->GetBodyComponent()->GetAnimInstance()->Montage_IsPlaying(HeavyAttackMontage) == false)
		//isUseableHeavyAttack == true)
		return;

	GEngine->AddOnScreenDebugMessage(6, 3.0f, FColor::Magenta, FString::FromInt(HeavyAttackCount[NormalAttackSectionIndex]));
	if (OwnerCharacter->GetBodyComponent()->GetAnimInstance()->Montage_IsPlaying(HeavyAttackMontage) == true &&
		HeavyAttackCount[NormalAttackSectionIndex] > 0 &&
		HeavyAttackCount[NormalAttackSectionIndex] >= HeavyAttackMaxCount[NormalAttackSectionIndex])
		return;

	HeavyAttackCount[NormalAttackSectionIndex]++;
	GEngine->AddOnScreenDebugMessage(4, 3.0f, FColor::Magenta, TEXT("Play Completed Heavy Attack Montage"));
}

FName UBaseSwordStanceActorComponent::GetAddCurNormalAttackSectionName()
{
	GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Magenta, FString::FromInt(NormalAttackSectionIndex));
	FName SectionName = NormalAttackSectionNames[NormalAttackSectionIndex];
	++NormalAttackSectionIndex;
	GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Blue, FString::FromInt(NormalAttackSectionIndex));
	GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Green, FString::FromInt(NormalAttackSectionNames.Num()));
	NormalAttackSectionIndex %= NormalAttackSectionNames.Num();
	GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, FString::FromInt(NormalAttackSectionIndex));
	GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Black, FString::FromInt(NormalAttackSectionIndex %= NormalAttackSectionNames.Num()));
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
	APlayableBaseCharacter* OwnerCharacter = Cast<APlayableBaseCharacter>(GetOwner());
	if (eNextAttackType == EATTACKTYPE::AT_NORMALATTACK)
	{
		PlayNormalAttackMontage();
	}
	else if (eNextAttackType == EATTACKTYPE::AT_HEAVYATTACK)
	{
		PlayHeavyAttackMontage();
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(5, 3.0f, FColor::Red, TEXT("Next Attack Type Error"));
	}
	eNextAttackType = EATTACKTYPE::AT_MAX;
}

void UBaseSwordStanceActorComponent::ResetAttackInfo()
{
	NormalAttackSectionIndex = 0;
	//isUseableNormalAttack = true;
	//isUseableHeavyAttack = true;
	IsPossibleNextAttack = false;
	eNextAttackType = EATTACKTYPE::AT_MAX;
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

