// Fill out your copyright notice in the Description page of Project Settings.
#include "ActorComponent/SkillActionComponent/SkillActionComponent.h"
#include "PlayableCharacter/PlayableBaseCharacter.h"
#include "Monster/BaseMonster.h"
#include "PlayerState/FatePlayerState.h"
#include "ActorComponent/InventoryComponent/InventoryComponent.h"
#include "ActorComponent/ResonanceComponent/ResonanceComponent.h"
#include "Controller/MiyamotoIoriController/MiyamotoIoriController.h"
#include "DataAsset/PrimaryDataAsset/SkillDataAsset/ProjectileSkillDataAsset.h"
#include "Projectile/FateProjectile.h"
#include "ActorComponent/StateComponent/PlayableStateComponent.h"
#include "DataAsset/PrimaryDataAsset/SkillDataAsset/BuffSkillDataAsset.h"
#include "DataAsset/PrimaryDataAsset/SkillDataAsset/HikenSkillDataAsset.h"
#include "PlayableCharacter/Miyamoto_Iori/ActorComponent/BaseSwordStanceActorComponent.h"

#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/PlayerController.h"
#include "NiagaraSystem.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"
#include "LevelSequence.h"
#include "LevelSequencePlayer.h"
#include "LevelSequenceActor.h"


// Sets default values for this component's properties
USkillActionComponent::USkillActionComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	NormalSkills.SetNum(4);
	// ...
}


// Called when the game starts
void USkillActionComponent::BeginPlay()
{
	Super::BeginPlay();
	
	// ...
	OwnerCharacter = Cast<APlayableBaseCharacter>(GetOwner());
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

	if(NormalSkills.Num() != 4)
	{
		NormalSkills.SetNum(4);
	}
}


// Called every frame
void USkillActionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void USkillActionComponent::GatherEnemies()
{
	if (nullptr == CurrentActiveSkill)
		return;
	const FSkillGatherSetting GatherSetting = CurrentActiveSkill->GatherSetting;
	if (GatherSetting.bIsGathering == false)
		return;
	if(OwnerCharacter == nullptr || OwnerCharacter->GetCurrentCombatZone() == nullptr)
		return;
	EndGathering();
	const TArray<ABaseMonster*>& Monsters = OwnerCharacter->GetCurrentCombatZone()->GetLiveMonsters();
	//모을 중심점
	FVector GatherCenter = OwnerCharacter->GetActorLocation() + (OwnerCharacter->GetActorForwardVector() * GatherSetting.ForwardOffset);
	for(ABaseMonster* Monster : Monsters)
	{
		if(nullptr == Monster || Monster->IsDead())
			continue;
		FGatherTargetInfo Info;
		Info.TargetMonster = Monster;
		Info.RelativeOffset = FVector(FMath::RandPointInCircle(GatherSetting.GatherRadius), 0.f);

		ActiveGatherTargets.Add(Info);
	}

	if (ActiveGatherTargets.Num() == 0)
		return;
	UpdateGathering();
	GetWorld()->GetTimerManager().SetTimer(
		GatherTimerHandle,
		this,
		&USkillActionComponent::UpdateGathering,
		0.016f,
		true
	);
}

void USkillActionComponent::UpdateGathering()
{
	if (nullptr == CurrentActiveSkill || nullptr == OwnerCharacter)
	{
		EndGathering();
		return;
	}
	const FSkillGatherSetting& GatherSetting = CurrentActiveSkill->GatherSetting;
	FVector GatherCenter = OwnerCharacter->GetActorLocation() + (OwnerCharacter->GetActorForwardVector() * GatherSetting.ForwardOffset);

	for (int32 i = ActiveGatherTargets.Num() - 1; i >= 0; --i)
	{
		ABaseMonster* Monster = ActiveGatherTargets[i].TargetMonster.Get();

		if (nullptr == Monster || Monster->IsDead())
		{
			ActiveGatherTargets.RemoveAt(i);
			continue;
		}

		// 최종 도달 목표 위치 (중심점 + 고정된 오프셋)
		FVector FinalLocation = GatherCenter + ActiveGatherTargets[i].RelativeOffset;
		FinalLocation.Z = Monster->GetActorLocation().Z;

		// VInterpTo를 사용해 매끄럽게 끌어당김
		FVector SmoothLocation = FMath::VInterpTo(
			Monster->GetActorLocation(),
			FinalLocation,
			0.016f,
			12.0f
		);

		Monster->SetActorLocation(SmoothLocation, false, nullptr, ETeleportType::TeleportPhysics);

		if (auto* MoveComp = Monster->GetCharacterMovement())
		{
			MoveComp->StopMovementImmediately();
		}

		// 플레이어 바라보도록 회전
		FVector LookDir = (OwnerCharacter->GetActorLocation() - SmoothLocation).GetSafeNormal();
		Monster->SetActorRotation(FRotator(0.f, LookDir.Rotation().Yaw, 0.f));
	}
}

void USkillActionComponent::EndGathering()
{
	if (GetWorld())
	{
		GetWorld()->GetTimerManager().ClearTimer(GatherTimerHandle);
	}
	ActiveGatherTargets.Empty();
}

void USkillActionComponent::SetNormalSkill(int32 index, USkillDataAsset* NewSkill)
{
	if(OwnerCharacter != nullptr)
	{
		if (NormalSkills.IsValidIndex(index))
		{
			NormalSkills[index] = NewSkill;
		}
	}
}

void USkillActionComponent::SetHikenSkill(USkillDataAsset* NewSkill)
{
	HikenSkill = NewSkill;
}

bool USkillActionComponent::ExecuteSkill(int32 index)
{
	USkillDataAsset* TargetSkill = nullptr;

	if(index == -1)
	{
		TargetSkill = HikenSkill;
	}
	else if(NormalSkills.IsValidIndex(index))
	{
		TargetSkill = NormalSkills[index];
	}

	if(TargetSkill == nullptr)
	{
		GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Green, TEXT("No Skill Found"));
		return false;
	}
	return TryExecuteSkill(TargetSkill);
}

bool USkillActionComponent::TryExecuteSkill(USkillDataAsset* TargetSkill)
{
	if(TargetSkill == nullptr)
		return false;
	
	if(!HasEnoughResource(TargetSkill))
	{
		GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Red, TEXT("Not Enough Resources"));
		return false;
	}
	ConsumeResource(TargetSkill);
	CurrentActiveSkill = TargetSkill;
	if (OwnerCharacter != nullptr)
	{
		//무기 위치의 강제 이동이므로 애니메이션 실행 안함
		switch (TargetSkill->WeaponRequirement)
		{
		case ESkillWeaponRequirement::RequireEquipment:
			OwnerCharacter->WeaponEquip();
			break;
		case ESkillWeaponRequirement::RequireNoEquipment:
			OwnerCharacter->WeaponUnEquip();
			break;
		default:
			break;
		}
		UHikenSkillDataAsset* HikenData = Cast<UHikenSkillDataAsset>(TargetSkill);	

		OwnerCharacter->SetIsActionLock(true);
		//비검, 보구일 경우 시네마틱 재생
		if (HikenData)
		{
			FString MontageName = GetNameSafe(OwnerCharacter->GetMesh()->GetAnimInstance()->GetCurrentActiveMontage());

			UE_LOG(LogTemp, Warning, TEXT("Current Montage : %s"), *MontageName);
			PlaySkillCinematic(HikenData);
		}
		//일반 스킬일 경우 몽타주 실행
		else
		{
			OwnerCharacter->SnapToTargetEnemy();
			OwnerCharacter->PlayMontageFullBody(TargetSkill->SkillMontage);
		}
	}
	GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Green, FString::Printf(TEXT("Executed Skill: %s"), *TargetSkill->GetName()));
	return true;
}

bool USkillActionComponent::HasEnoughResource(USkillDataAsset* TargetSkill) const
{
	AFatePlayerState* PlayerState = Cast<AFatePlayerState>(OwnerCharacter->GetPlayerState());
	int32 CurrentCost = 0;
	switch (TargetSkill->CostType)
	{
	case ESkillCostType::None:
		return true;
	case ESkillCostType::Gem:
	{
		if (PlayerState != nullptr)
		{
			UInventoryComponent* InventoryComp = PlayerState->FindComponentByClass<UInventoryComponent>();
			if (InventoryComp != nullptr)
			{
				CurrentCost = InventoryComp->GetItemQuantity(FName(TEXT("Gem")));
			}
		}
	}
	break;
	case ESkillCostType::LinkSkillBall:
	{
		if (PlayerState != nullptr)
		{
			UResonanceComponent* ResonanceComp = PlayerState->FindComponentByClass<UResonanceComponent>();
			if (ResonanceComp != nullptr)
			{
				CurrentCost = ResonanceComp->GetLinkBall();
			}
		}
	}
	case ESkillCostType::Hiken:
	{
		UPlayableStateComponent* OwnerStat = OwnerCharacter->GetStatusComponent();
		if (OwnerStat != nullptr)
		{
			CurrentCost = OwnerStat->GetHiken();
		}
	}
	break;
	default:
		break;
	}
	return CurrentCost >= TargetSkill->CostAmount;
}

void USkillActionComponent::ConsumeResource(USkillDataAsset* TargetSkill)
{
	if(TargetSkill->CostType == ESkillCostType::None)
		return;
	AFatePlayerState* PlayerState = Cast<AFatePlayerState>(OwnerCharacter->GetPlayerState());
	switch (TargetSkill->CostType)
	{
	case ESkillCostType::Gem:
		{
			if (PlayerState != nullptr)
			{
				UInventoryComponent* InventoryComp = PlayerState->FindComponentByClass<UInventoryComponent>();
				if (InventoryComp != nullptr)
				{
					InventoryComp->ConsumeItem(FName(TEXT("Gem")), TargetSkill->CostAmount);
				}
			}
		}
		break;
	case ESkillCostType::LinkSkillBall:
		{
			if (PlayerState != nullptr)
			{
				UResonanceComponent* ResonanceComp = PlayerState->FindComponentByClass<UResonanceComponent>();
				if (ResonanceComp != nullptr)
				{
					ResonanceComp->ConsumeLinkBall(TargetSkill->CostAmount);
				}
			}
		}
		break;
	case ESkillCostType::Hiken:
	{
		UPlayableStateComponent* OwnerStat = OwnerCharacter->GetStatusComponent();
		if (OwnerStat != nullptr)
		{
			OwnerStat->ConsumeHike(TargetSkill->CostAmount);
		}
	}
		break;
	}
}

void USkillActionComponent::FireProjectileSkill(USkillDataAsset* TargetSkill)
{
	APawn* MyPawn = Cast<APawn>(GetOwner());
	UProjectileSkillDataAsset* ProjecttileSkillData = Cast<UProjectileSkillDataAsset>(TargetSkill);
	if (!MyPawn || !ProjecttileSkillData || !ProjecttileSkillData->ProjectileClass)
		return;
	FVector SpawnLocation = MyPawn->GetActorLocation() + (MyPawn->GetActorForwardVector() * 100.0f);
	FRotator SpawnRotation = MyPawn->GetActorRotation();

	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = MyPawn;
	SpawnParams.Instigator = MyPawn;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	AFateProjectile* SpawnedProjectile = GetWorld()->SpawnActor<AFateProjectile>(
		ProjecttileSkillData->ProjectileClass,
		SpawnLocation,
		SpawnRotation,
		SpawnParams
	);

	if (SpawnedProjectile)
	{
		ECollisionChannel TargetChannel = ECC_Pawn;
		TSubclassOf<ACharacter> AllyClassFilter = nullptr;

		if (MyPawn->IsA(APlayableBaseCharacter::StaticClass()))
		{
			TargetChannel = ECC_GameTraceChannel3;
			AllyClassFilter = APlayableBaseCharacter::StaticClass();
		}
		else
		{
			//몬스터 용 제작 예정
			TargetChannel = ECC_GameTraceChannel1;
		}
		SpawnedProjectile->LaunchProjectile(
			ProjecttileSkillData->ProjectileSpeed,
			OwnerCharacter->GetStatusComponent()->GetMat(),
			ProjecttileSkillData->AttackData.DamageMultiplier,
			ProjecttileSkillData->ExplosionRadius,
			ProjecttileSkillData->MaxTravelDistance,
			TargetChannel,
			AllyClassFilter
		);
	}
}

void USkillActionComponent::FireBuffSkill(USkillDataAsset* TargetSkill)
{
	UBuffSkillDataAsset* BuffSkillData = Cast<UBuffSkillDataAsset>(TargetSkill);
	if (!OwnerCharacter || !BuffSkillData)
		return;
	TArray<UNiagaraComponent*> SpawnedVFXs;
	if (BuffSkillData->LoopVFX)
	{
		EWeaponVFXTarget ActiveTarget = OwnerCharacter->GetCurrentWeaponVFXTarget();
		USceneComponent* FirstWeapon = OwnerCharacter->GetFirstWeaponMesh();
		USceneComponent* SecondWeapon = OwnerCharacter->GetSecondWeaponMesh();

		auto SpawnVFXOnWeapon = [&](USceneComponent* TargetMesh) -> UNiagaraComponent*
			{
				if (TargetMesh)
				{
					return UNiagaraFunctionLibrary::SpawnSystemAttached(
						BuffSkillData->LoopVFX,
						TargetMesh,
						NAME_None,
						FVector::ZeroVector,
						FRotator::ZeroRotator,
						EAttachLocation::SnapToTarget,
						true
					);
				}
				return nullptr;
			};

		switch (ActiveTarget)
		{
		case EWeaponVFXTarget::RightWeapon:
			SpawnedVFXs.Add(SpawnVFXOnWeapon(FirstWeapon));
			break;
		case EWeaponVFXTarget::LeftWeapon:
			SpawnedVFXs.Add(SpawnVFXOnWeapon(SecondWeapon));
			break;
		case EWeaponVFXTarget::BothWeapon:
			SpawnedVFXs.Add(SpawnVFXOnWeapon(FirstWeapon));
			SpawnedVFXs.Add(SpawnVFXOnWeapon(SecondWeapon));
			break;
		default:
			break;
		}
	}

	if (OwnerCharacter->GetStatusComponent())
	{
		OwnerCharacter->GetStatusComponent()->ApplyAttackBuff(BuffSkillData->BuffMultiplier, BuffSkillData->BuffDuration, SpawnedVFXs);
	}
}

void USkillActionComponent::FireRecoverSkill(USkillDataAsset* TargetSkill)
{
	UBuffSkillDataAsset* BuffSkillData = Cast<UBuffSkillDataAsset>(TargetSkill);
	if (!OwnerCharacter || !BuffSkillData)
	{
		return;
	}

	if (BuffSkillData->StartVFX && OwnerCharacter->GetMesh())
	{
		UNiagaraFunctionLibrary::SpawnSystemAttached(
			BuffSkillData->StartVFX,
			OwnerCharacter->GetMesh(),
			FName("hand_r"),
			FVector::ZeroVector,
			FRotator::ZeroRotator,
			EAttachLocation::SnapToTarget,
			true
		);
	}

	UBaseStateComponent* StatusComp = OwnerCharacter->GetStatusComponent();
	if (StatusComp)
	{
		float RecoverAmount = BuffSkillData->BuffMultiplier * StatusComp->GetMaxHP();
		OwnerCharacter->ExecuteHeal(RecoverAmount);
	}
}

void USkillActionComponent::PlayTriggerSkillMontage()
{
	//ExecuteSkill보다 먼저 실행이 될 경우 return;
	if (CurrentActiveSkill == nullptr)
		return;
	if (CurrentActiveSkill->CastType != ESkillCastType::Charge && CurrentActiveSkill->CastType != ESkillCastType::Continuous)
		return;
	if (IsCharging == false)
	{
		IsCharging = true;
		ChargeStartTime = GetWorld()->GetTimeSeconds();
	}
	if (IsCharging == true)
	{
		CurrentChargeTime = GetWorld()->GetTimeSeconds();
		float ElapsedTime = CurrentChargeTime - ChargeStartTime;
		//시간 초과시 강제 실행
		if (ElapsedTime >= CurrentActiveSkill->ChargeTime)
		{
			PlayEndSkillMontage(1.0);
		}
	}
}

void USkillActionComponent::PlayCompletedSkillMontage()
{
	if (CurrentActiveSkill == nullptr)
		return;
	if (IsCharging == false)
		return;
	if (CurrentActiveSkill->CastType != ESkillCastType::Charge && CurrentActiveSkill->CastType != ESkillCastType::Continuous)
		return;
	CurrentChargeTime = GetWorld()->GetTimeSeconds();
	float ElapsedTime = CurrentChargeTime - ChargeStartTime;
	float ChargeRatio = FMath::Clamp(ElapsedTime / CurrentActiveSkill->ChargeTime, 0.0f, 1.0f);
	PlayEndSkillMontage(ChargeRatio);

}

void USkillActionComponent::PlayEndSkillMontage(float ChargeRatio)
{
	FName SectionName = FName(CurrentActiveSkill->SkillID.ToString() + TEXT("End"));
	OwnerCharacter->PlayMontageFullBody(CurrentActiveSkill->SkillMontage, SectionName);
}

void USkillActionComponent::BindSkillMontageDelegate(UAnimInstance* AnimInstance)
{
	if (AnimInstance == nullptr)
		return;
	AnimInstance->OnMontageEnded.AddDynamic(this, &USkillActionComponent::OnSkillMontageEnded);
}

void USkillActionComponent::OnSkillMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	if(Montage == nullptr || CurrentActiveSkill == nullptr || CurrentActiveSkill->SkillMontage != Montage)
		return;
	if (OwnerCharacter != nullptr)
	{
		APlayerController* PC = Cast<APlayerController>(OwnerCharacter->GetController());
		if (PC != nullptr)
		{
			PC->SetIgnoreLookInput(false);
			switch (CurrentActiveSkill->CastType)
			{
			case ESkillCastType::Instant:
				break;
			case ESkillCastType::Charge:
			case ESkillCastType::Continuous:
				{
					AMiyamotoIoriController* MiyamotoPC = Cast<AMiyamotoIoriController>(PC);
					if(MiyamotoPC && MiyamotoPC->IsSkillHoldActionPressed())
						OwnerCharacter->RequestBlockContinuousInput();
				}
				break;
			default:
				break;
			}
		}
		if (CurrentActiveSkill->WeaponRequirement == ESkillWeaponRequirement::RequireNoEquipment)
			OwnerCharacter->PlayEquipWeaponMontage_New();
	}
	CurrentActiveSkill = nullptr;
	IsCharging = false;
	ChargeStartTime = 0.0f;
	CurrentChargeTime = 0.0f;
}

bool USkillActionComponent::IsPlayingCinematic() const
{
	return ActiveSequencePlayer &&
		ActiveSequencePlayer->IsPlaying();
}

void USkillActionComponent::PlaySkillCinematic(USkillDataAsset* TargetSkill)
{
	if(nullptr == TargetSkill)
		return;
	if(nullptr == OwnerCharacter)
		return;
	CurrentCinematicSkill = TargetSkill;

	//시간 조작
	if (TargetSkill->bUseTimeDilation)
	{
		OriginalTimeDilation = UGameplayStatics::GetGlobalTimeDilation(GetWorld());
		UGameplayStatics::SetGlobalTimeDilation(GetWorld(), TargetSkill->CustomTimeDilationValue);
		//조종중인 캐릭터만 정상 속도
		OwnerCharacter->CustomTimeDilation = 1.0f / TargetSkill->CustomTimeDilationValue;
	}

	//1. 시퀀서를 이용한 스킬 연출 시스템
	//비검, 보구만 시퀀서를 사용
	UHikenSkillDataAsset* TargetHiken = Cast<UHikenSkillDataAsset>(TargetSkill);
	if (TargetHiken && TargetHiken->CinematicSequence)
	{
		ALevelSequenceActor* SequenceActor = nullptr;
		FMovieSceneSequencePlaybackSettings PlaybackSettings;
		ULevelSequencePlayer* SequencePlayer = ULevelSequencePlayer::CreateLevelSequencePlayer(
			GetWorld(),
			TargetHiken->CinematicSequence,
			PlaybackSettings,
			SequenceActor
		);
		if (SequenceActor && SequencePlayer)
		{
			TArray<AActor*> BindActors;
			BindActors.Add(OwnerCharacter);
			SequenceActor->SetBindingByTag(TargetHiken->CharacterBindingTag, BindActors, true);
			if (TargetSkill->bUseTimeDilation && TargetSkill->CustomTimeDilationValue > 0.0f)
			{
				SequencePlayer->SetPlayRate(1.0f / TargetSkill->CustomTimeDilationValue);
			}
			APlayerController* PC = Cast<APlayerController>(OwnerCharacter->GetController());
			if (PC)
			{
				PC->SetIgnoreLookInput(true);
			}
			ActiveSequencePlayer = SequencePlayer;
			ActiveSequencePlayer->OnFinished.AddUniqueDynamic(
				this,
				&USkillActionComponent::OnSkillCinematicFinished);
			SequencePlayer->Play();
			return;
		}
	}

	//2. 타임라인을 통한 스킬연출 시스템
	//카메라 연출
	//몇몇 일반 스킬은 타임라인을 통한 간단한 카메라 무빙을 이용
	if (TargetSkill->CameraSetting.bIsCameraEffect == false)
	{
		OwnerCharacter->ResetCameraPosition();
	}
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

void USkillActionComponent::EndSkillCinematic(USkillDataAsset* TargetSkill)
{
	if (nullptr == TargetSkill)
	{
		TargetSkill = CurrentCinematicSkill;
	}
	if(nullptr == TargetSkill)
		return;
	//시간 조작 복구
	ULevelSequencePlayer* SequencePlayer = nullptr;

	if (TargetSkill->bUseTimeDilation)
	{
		if(OriginalTimeDilation > 0.0f)
		{
			UGameplayStatics::SetGlobalTimeDilation(GetWorld(), OriginalTimeDilation);
			if (nullptr != OwnerCharacter)
			{
				OwnerCharacter->CustomTimeDilation = 1.0f;
			}
			OriginalTimeDilation = 0.0f;
		}
	}
	if(ActiveSequencePlayer)
	{
		ActiveSequencePlayer->SetPlayRate(1.0f);
		ActiveSequencePlayer = nullptr;
	}
}

void USkillActionComponent::UpdateCameraTimeline(float Value)
{
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
	if(nullptr == OwnerCharacter || nullptr == OwnerCharacter->GetSpringArm())
		return;
	USpringArmComponent* SpringArm = OwnerCharacter->GetSpringArm();
	SpringArm->bUsePawnControlRotation = true;
	SpringArm->TargetArmLength = DefaultCameraArmLength;
	SpringArm->SetRelativeRotation(DefaultCameraRotation);

	CurrentCinematicSkill = nullptr;
}

void USkillActionComponent::OnSkillCinematicFinished()
{
	OwnerCharacter->SetIsActionLock(false);
	APlayerController* PC = Cast<APlayerController>(OwnerCharacter->GetController());
	if (PC)
	{
		PC->SetIgnoreLookInput(false);
	}
	CurrentCinematicSkill = nullptr;
	ActiveSequencePlayer = nullptr;
}
