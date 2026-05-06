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
	if(!IsValid(OwnerCharacter) || !IsValid(NormalAttackMontage))
		return;

	//응격(퍼펙트 닷지) 몽타주 재생
	if (OwnerCharacter->GetIsWaitingForCounterInput())
	{
		ACharacter* TargetMonster = OwnerCharacter->GetLastAttacker();
		OwnerCharacter->EndCounterInputWindow();
		OwnerCharacter->StopMontage(nullptr);
		//반격 몽타주 재생
		//응격을 위한 시간 정지 상태가 되었을 때 다른 입력을 받지 않도록 하는 코드 필요
		if(IsValid(TargetMonster) == true)
		{
			FVector PlayerLocation = OwnerCharacter->GetActorLocation();
			FVector MonsterLocation = TargetMonster->GetActorLocation();
			FVector DirectionToMonster = (MonsterLocation - PlayerLocation).GetSafeNormal2D();
			FRotator TargetRotation = DirectionToMonster.Rotation();

			float CurrentDistance = FVector::Dist2D(PlayerLocation, MonsterLocation);
			float AttackRange = 30.0f;
			float SafeDistance = AttackRange;

			FVector Destination = PlayerLocation;
			if (CurrentDistance > SafeDistance)
			{
				Destination = MonsterLocation - (DirectionToMonster * SafeDistance);
			}

			Destination.Z = PlayerLocation.Z;
			OwnerCharacter->SetActorLocationAndRotation(Destination, TargetRotation, true);

			OwnerCharacter->PlayMontageFullBody(CounterAttackMontage, NAME_None, AmountAttackSpeed);
			GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Cyan, FString::Printf(TEXT("CounterAttack"), NormalAttackSectionIndex));
		}
		return;
	}


	if (OwnerCharacter->GetMovementComponent()->IsFalling() == true ||
		OwnerCharacter->IsEvading() == true || OwnerCharacter->GetIsCombatMode() == false)
		return;

	UAnimInstance* AnimInstance = OwnerCharacter->GetBodyComponent()->GetAnimInstance();

	if (!IsValid(AnimInstance))
		return;

	if(AnimInstance->Montage_IsPlaying(HeavyAttackMontage) == true)
		return;
	
	//다른 행동 중인지 판단
	if (OwnerCharacter->GetIsActionLock())
	{
		if (AnimInstance->Montage_IsPlaying(NormalAttackMontage) && bCanReceiveInput == true)
		{

			//다음 공격 선입력
			//if (IsPossibleNextAttack == false && NormalAttackSectionIndex > 0)
			if (bIsAttackQueued == false && NormalAttackSectionIndex > 0)
			{
				//IsPossibleNextAttack = true;
				bIsAttackQueued = true;
				bCanEverAffectNavigation = false;
				NextAttackMontage = NormalAttackMontage;
				if (NormalAttackData[NormalAttackSectionIndex].MontageSectionName != NAME_None)
				{
					GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Blue, FString::Printf(TEXT("NA %d"), NormalAttackSectionIndex));
					NextAttackName = NormalAttackData[NormalAttackSectionIndex].MontageSectionName;
				}
			}
		}
		return;
	}
	//행동 잠금
	OwnerCharacter->SetIsActionLock(true);

	//GEngine->AddOnScreenDebugMessage(2, 3.0f, FColor::Blue, FString::Printf(TEXT("Play Normal Attack Montage AttackSpeed %f"), AmountAttackSpeed));

	GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Blue, FString::Printf(TEXT("AmountAttackSpeed %d"), AmountAttackSpeed));
	OwnerCharacter->PlayMontageFullBody(NormalAttackMontage, GetAddCurNormalAttackSectionName(), AmountAttackSpeed);
	GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Magenta, FString::Printf(TEXT("AmountAttackSpeed %d"), AmountAttackSpeed));
}

void UBaseSwordStanceActorComponent::PlayHeavyAttackMontage()
{
	APlayableBaseCharacter* OwnerCharacter = Cast<APlayableBaseCharacter>(GetOwner());
	if(!IsValid(OwnerCharacter) || !IsValid(HeavyAttackMontage))
		return;

	//기본 적인 강공격 가능 여부 확인
	if(OwnerCharacter->GetMovementComponent()->IsFalling() ||
		OwnerCharacter->IsEvading() ||
		OwnerCharacter->GetIsCombatMode() == false)
		return;
	
	//강공격을 통한 응격은 없음
	//현재 TestCode로 인해 사용 중 수정 예정
	if (OwnerCharacter->GetIsWaitingForCounterInput())
		return;

	UAnimInstance* AnimInstance = OwnerCharacter->GetBodyComponent()->GetAnimInstance();
	if(!IsValid(AnimInstance))
		return;

	//현재 행동 여부 확인
	if(OwnerCharacter->GetIsActionLock())
	{
		if(AnimInstance->Montage_IsPlaying(HeavyAttackMontage) || AnimInstance->Montage_IsPlaying(NormalAttackMontage))
		{
			//연타 강공격 가능 여부 확인
			if (HeavyAttackData[NormalAttackSectionIndex].AttackCount >= HeavyAttackData[NormalAttackSectionIndex].AttackMaxCount)
				return;

			//if(IsPossibleNextAttack == false && NormalAttackSectionIndex > 0)
			if (bIsAttackQueued == false && bCanReceiveInput == true && NormalAttackSectionIndex > 0)
			{
				//다음 공격 선입력
				//IsPossibleNextAttack = true;
				bIsAttackQueued = true;
				bCanReceiveInput = false;
				NextAttackMontage = HeavyAttackMontage;
				GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Blue, FString::Printf(TEXT("HA %d"), NormalAttackSectionIndex));
				if (HeavyAttackData[NormalAttackSectionIndex].MontageSectionName != NAME_None)
					NextAttackName = HeavyAttackData[NormalAttackSectionIndex].MontageSectionName;
			}
		}
		return;
	}

	OwnerCharacter->SetIsActionLock(true);

	//일회성 특수 공격력 초기화
	//코드 수정 예정
	//뭔가 위험함

	if (OwnerCharacter->PlayMontageFullBody(HeavyAttackMontage, GetCurHeavyAttackSectionName(), AmountAttackSpeed) == true)
	{
		HeavyAttackData[NormalAttackSectionIndex].AttackCount++;
		//GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Blue, TEXT("TTTTTPlay Heavy Attack Montage"));
	}
	else
	{
		OwnerCharacter->SetIsActionLock(false);
	}
}

void UBaseSwordStanceActorComponent::PlayTriggeredHeavyAttackMontage()
{
	APlayableBaseCharacter* OwnerCharacter = Cast<APlayableBaseCharacter>(GetOwner());

	if(!IsValid(OwnerCharacter) || OwnerCharacter->GetIsActionLock() == false)
		return;
	UAnimInstance* AnimInstance = OwnerCharacter->GetBodyComponent()->GetAnimInstance();
	if(!IsValid(AnimInstance))
		return;

	if(AnimInstance->Montage_IsPlaying(HeavyAttackMontage) == false)
		return;
	//GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Blue, TEXT("SSSSSPlay Heavy Attack Montage"));
	
	if(IsCharging == false)
	{
		IsCharging = true;
		ChargeStartTime = GetWorld()->GetTimeSeconds();
		//GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Yellow, TEXT("Charging Started..."));
	}
}

void UBaseSwordStanceActorComponent::PlayCompletedHeavyAttackMontage()
{
	APlayableBaseCharacter* OwnerCharacter = Cast<APlayableBaseCharacter>(GetOwner());
	if(!IsValid(OwnerCharacter))
		return;

	if(IsCharging == false)
		return;
	
	UAnimInstance* AnimInstance = OwnerCharacter->GetBodyComponent()->GetAnimInstance();
	if (!IsValid(AnimInstance) || AnimInstance->Montage_IsPlaying(HeavyAttackMontage) == false)
	{
		IsCharging = false;
		return;
	}


	float ReleaseTime = GetWorld()->GetTimeSeconds();
	IsCharging = false;
	CurrentChargeTime = ReleaseTime - ChargeStartTime;
	CurrentChargeTime = FMath::Clamp(CurrentChargeTime, 0.0f, MaxChargeTime);
}

FName UBaseSwordStanceActorComponent::GetAddCurNormalAttackSectionName()
{
	FName SectionName = NormalAttackData[NormalAttackSectionIndex].MontageSectionName;
	++NormalAttackSectionIndex;
	//마지막 공격 섹션 이후에는 다시 처음 공격 섹션으로 돌아감
	NormalAttackSectionIndex %= NormalAttackData.Num();
	GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Blue, FString::Printf(TEXT("AddNA %d"), NormalAttackSectionIndex));
	return SectionName;
}

FName UBaseSwordStanceActorComponent::GetCurHeavyAttackSectionName()
{
	return HeavyAttackData[NormalAttackSectionIndex].MontageSectionName;
}

bool UBaseSwordStanceActorComponent::IsAttacking()
{
	APlayableBaseCharacter* OwnerCharacter = Cast<APlayableBaseCharacter>(GetOwner());
	if (OwnerCharacter->GetBodyComponent()->GetAnimInstance()->Montage_IsPlaying(NormalAttackMontage) == true ||
		OwnerCharacter->GetBodyComponent()->GetAnimInstance()->Montage_IsPlaying(HeavyAttackMontage) == true)
		return true;
	return false;
}

void UBaseSwordStanceActorComponent::ResetNextAttack()
{
	//입력 받을 수 있는 상태로 변경
	bCanReceiveInput = true;

	//IsPossibleNextAttack = false;
	bIsAttackQueued = false;
	bIsNextAttackTransitioning = false;
	NextAttackMontage = nullptr; 
	NextAttackName = NAME_None;
}

void UBaseSwordStanceActorComponent::PlayNextAttackMontage()
{
	//if (IsPossibleNextAttack == false)
	if (bIsAttackQueued == false)
	{
		ResetAttackInfo();
		return;
	}
	GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Magenta, FString::Printf(TEXT("Test %s"), *NextAttackName.ToString()));

	//다음 공격으로 넘어가는 중을 제어하는 플래그 활성화
	bIsNextAttackTransitioning = true;

	APlayableBaseCharacter* OwnerCharacter = Cast<APlayableBaseCharacter>(GetOwner());
	OwnerCharacter->PlayMontageFullBody(NextAttackMontage, NextAttackName, AmountAttackSpeed);
	if(NextAttackMontage == HeavyAttackMontage)
	{
		//GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Blue, TEXT("NNNNNPlay Heavy Attack Montage"));
		HeavyAttackData[NormalAttackSectionIndex].AttackCount++;
	}
	else if (NextAttackMontage == NormalAttackMontage)
	{
		GetAddCurNormalAttackSectionName();
	}
	bIsAttackQueued = false;
}

void UBaseSwordStanceActorComponent::ResetAttackInfo()
{
	NormalAttackSectionIndex = 0;
	for (int i = 0; i < HeavyAttackData.Num(); ++i)
	{
		HeavyAttackData[i].AttackCount = 0;
	}

	IsCharging = false;
	SpeicalAttackPower = 0;
	CurrentChargeTime = 0.0f;
	SpeicalAttackPower = 0;

	bIsAttackQueued = false;
	bCanReceiveInput = false;
	bIsNextAttackTransitioning = false;
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

float UBaseSwordStanceActorComponent::GetDamageMultiplier(EAttackVariety AttackVariety)
{
	switch (AttackVariety)
	{
	case EAttackVariety::Normal:
		return NormalAttackData[NormalAttackSectionIndex].DamageMultiplier;
	case EAttackVariety::Heavy:
		return HeavyAttackData[NormalAttackSectionIndex].DamageMultiplier;
		break;
	case EAttackVariety::Special:
		break;
	default:
		break;
	}
	return 1.0f;
}

