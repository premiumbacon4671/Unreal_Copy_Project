// Fill out your copyright notice in the Description page of Project Settings.
#include "ActorComponent/SkillActionComponent/SkillActionComponent.h"
#include "PlayableCharacter/PlayableBaseCharacter.h"
#include "Monster/BaseMonster.h"

#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"

// Sets default values for this component's properties
USkillActionComponent::USkillActionComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void USkillActionComponent::BeginPlay()
{
	Super::BeginPlay();
	
	// ...
	APlayableBaseCharacter* OwnerCharacter = Cast<APlayableBaseCharacter>(GetOwner());
	if (OwnerCharacter != nullptr && OwnerCharacter->GetSpringArm() != nullptr)
	{
		DefaultCameraArmLength = OwnerCharacter->GetSpringArm()->TargetArmLength;
		DefaultCameraRotation = OwnerCharacter->GetSpringArm()->GetRelativeRotation();
	}
	//타임라인 컴포넌트 생성 및 엔진에 등록
	CameraTimeline = NewObject<UTimelineComponent>(this, FName("CameraTimeline"));
	if (CameraTimeline)
	{
		CameraTimeline->RegisterComponent();
		
		FOnTimelineEvent TimelineFinished;
		TimelineFinished.BindUFunction(this, FName("OnCameraTimelineFinished"));
		CameraTimeline->SetTimelineFinishedFunc(TimelineFinished);

		CameraProgressDelegate.BindUFunction(this, FName("UpdateCameraTimeline"));
	}
}


// Called every frame
void USkillActionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void USkillActionComponent::GatherEnemies(const FSkillGatherSetting& GatherSetting)
{
	if (GatherSetting.bIsGathering == false)
		return;
	APlayableBaseCharacter* OwnerCharacter = Cast<APlayableBaseCharacter>(GetOwner());
	if(OwnerCharacter == nullptr || OwnerCharacter->GetCurrentCombatZone() == nullptr)
		return;
	const TArray<ABaseMonster*>& Monsters = OwnerCharacter->GetCurrentCombatZone()->GetLiveMonsters();
	//모을 중심점
	FVector GatherCenter = OwnerCharacter->GetActorLocation() + (OwnerCharacter->GetActorForwardVector() * GatherSetting.ForwardOffset);
	for(ABaseMonster* Monster : Monsters)
	{
		if(nullptr == Monster || Monster->IsDead())
			continue;
		FVector RandamCircle = FVector(FMath::RandPointInCircle(GatherSetting.GatherRadius), 0.f);
		//몬스터의 위치를 모을 중심점에서 랜덤한 원 안으로 이동
		FVector FinalLocation = GatherCenter + RandamCircle;
		FinalLocation.Z = Monster->GetActorLocation().Z;
		//강제이동을 통한 물리 방지
		Monster->SetActorLocation(FinalLocation, false, nullptr, ETeleportType::TeleportPhysics);

		if(auto* MoveComp = Monster->GetCharacterMovement())
		{
			MoveComp->StopMovementImmediately();
		}
		FVector LookDir = (OwnerCharacter->GetActorLocation() - FinalLocation).GetSafeNormal();
		Monster->SetActorRotation(FRotator(0.f, LookDir.Rotation().Yaw, 0.f));
	}
}

void USkillActionComponent::SetNormalSkill(int32 index, USkillDataAsset* NewSkill)
{
	if(NormalSkills.IsValidIndex(index))
	{
		NormalSkills[index] = NewSkill;
	}
}

void USkillActionComponent::SetHikenSkill(USkillDataAsset* NewSkill)
{
	HikenSkill = NewSkill;
}

void USkillActionComponent::ExecuteSkill(int32 index)
{
	USkillDataAsset* TargetSkill = nullptr;

	if(index == -1)
	{
		TargetSkill = HikenSkill.Get();
	}
	else if(NormalSkills.IsValidIndex(index))
	{
		TargetSkill = NormalSkills[index].Get();
	}

	if(TargetSkill == nullptr)
	{
		GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Green, TEXT("No Skill Found"));
		return;
	}

	CurrentActiveSkill = TargetSkill;
	//스킬 실행 로직
	//작성예정

	//GatherEnemies(TargetSkill->GatherSetting);
	//PlaySkillCinematic(TargetSkill);
	if(APlayableBaseCharacter* OwnerCharacter = Cast<APlayableBaseCharacter>(GetOwner()))
	{
		OwnerCharacter->PlayMontageFullBody(TargetSkill->SkillMontage);
		OwnerCharacter->SetIsActionLock(true);
	}
	GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Green, FString::Printf(TEXT("Executed Skill: %s"), *TargetSkill->GetName()));
}

void USkillActionComponent::BindMontageEndedDelegate(UAnimInstance* AnimInstance)
{
	if (AnimInstance == nullptr)
		return;
	AnimInstance->OnMontageEnded.AddDynamic(this, &USkillActionComponent::OnSkillMontageEnded);
}

void USkillActionComponent::OnSkillMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	if(Montage == nullptr || CurrentActiveSkill == nullptr || CurrentActiveSkill->SkillMontage != Montage)
		return;
	CurrentActiveSkill = nullptr;
}

void USkillActionComponent::PlaySkillCinematic(USkillDataAsset* TargetSkill)
{
	if(nullptr == TargetSkill)
		return;
	CurrentCinematicSkill = TargetSkill;
	APlayableBaseCharacter* OwnerCharacter = Cast<APlayableBaseCharacter>(GetOwner());
	if(nullptr == OwnerCharacter)
		return;

	//시간 조작
	if (TargetSkill->bUseTimeDilation)
	{
		OriginalTimeDilation = UGameplayStatics::GetGlobalTimeDilation(GetWorld());
		UGameplayStatics::SetGlobalTimeDilation(GetWorld(), TargetSkill->CustomTimeDilationValue);
		//조종중인 캐릭터만 정상 속도
		OwnerCharacter->CustomTimeDilation = 1.0f / TargetSkill->CustomTimeDilationValue;
	}
	//카메라 연출
	if (TargetSkill->CameraSetting.bIsCameraEffect)
	{
		if(TargetSkill->CameraSetting.CameraCurve)
		{
			if (OwnerCharacter->GetSpringArm())
			{
				OwnerCharacter->GetSpringArm()->bUsePawnControlRotation = false;
			}
			if (bIsTimelineTrackInitialized == false)
			{
				CameraTimeline->AddInterpFloat(TargetSkill->CameraSetting.CameraCurve, CameraProgressDelegate, NAME_None, FName("CameraTrack"));
				bIsTimelineTrackInitialized = true;
			}
			else
			{
				CameraTimeline->SetFloatCurve(TargetSkill->CameraSetting.CameraCurve, FName("CameraTrack"));
			}
			//Timeline 재생
			CameraTimeline->PlayFromStart();
		}
	}
}

void USkillActionComponent::EndSkillCinematic(USkillDataAsset* TargetSkill)
{
	if(nullptr == TargetSkill)
		return;
	//시간 조작 복구
	if (TargetSkill->bUseTimeDilation)
	{
		UGameplayStatics::SetGlobalTimeDilation(GetWorld(), OriginalTimeDilation);
		APlayableBaseCharacter* OwnerCharacter = Cast<APlayableBaseCharacter>(GetOwner());
		if (nullptr != OwnerCharacter)
		{
			OwnerCharacter->CustomTimeDilation = 1.0f;
		}
	}
}

void USkillActionComponent::UpdateCameraTimeline(float Value)
{
	APlayableBaseCharacter* OwnerCharacter = Cast<APlayableBaseCharacter>(GetOwner());
	if(nullptr == OwnerCharacter || nullptr == OwnerCharacter->GetSpringArm())
		return;

	float TargetZome = CurrentCinematicSkill->CameraSetting.TargetArmLength;
	FRotator TargetRotation = CurrentCinematicSkill->CameraSetting.SocketRotation;

	float CurrentArmLength = FMath::Lerp(TargetZome, DefaultCameraArmLength, Value);
	FRotator CurrentRotation = FMath::Lerp(TargetRotation, DefaultCameraRotation, Value);

	OwnerCharacter->GetSpringArm()->TargetArmLength = CurrentArmLength;
	OwnerCharacter->GetSpringArm()->SetRelativeRotation(CurrentRotation);
}

void USkillActionComponent::OnCameraTimelineFinished()
{
	APlayableBaseCharacter* OwnerCharacter = Cast<APlayableBaseCharacter>(GetOwner());
	if(nullptr == OwnerCharacter || nullptr == OwnerCharacter->GetSpringArm())
		return;
	USpringArmComponent* SpringArm = OwnerCharacter->GetSpringArm();
	SpringArm->bUsePawnControlRotation = true;
	SpringArm->TargetArmLength = DefaultCameraArmLength;
	SpringArm->SetRelativeRotation(DefaultCameraRotation);

	CurrentCinematicSkill = nullptr;
}

