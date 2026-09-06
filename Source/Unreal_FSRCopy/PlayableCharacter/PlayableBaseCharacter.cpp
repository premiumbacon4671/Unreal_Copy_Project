// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayableCharacter/PlayableBaseCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Components/WidgetComponent.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputSubsystemInterface.h"
#include "EnhancedInputComponent.h"
#include "InputMappingContext.h"
#include "InputAction.h"
#include "Blueprint/UserWidget.h"   
#include "Engine/DamageEvents.h"
#include "Algo/Sort.h"
#include "Kismet/KismetMathLibrary.h"
#include "AIController.h"
#include "BrainComponent.h"
#include "BehaviorTree/BlackboardComponent.h"


#include "Controller/MiyamotoIoriController/MiyamotoIoriController.h"
#include "UI/CounterAttackUI.h"
#include "PlayableCharacter/Miyamoto_Iori/ActorComponent/BaseSwordStanceActorComponent.h"
#include "ActorComponent/StateComponent/PlayableStateComponent.h"
#include "ActorComponent/StateComponent/MonsterStateComponent.h"
#include "Monster/BaseMonster.h"
#include "PlayerState/FatePlayerState.h"
#include "ActorComponent/ResonanceComponent/ResonanceComponent.h"
#include "ActorComponent/SkillActionComponent/SkillActionComponent.h"

// Sets default values
APlayableBaseCharacter::APlayableBaseCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
#pragma region CreateComponent
	BodyComponent = GetMesh();
	HeadComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("HeadComponent"));
	HeadComponent->SetupAttachment(BodyComponent);
	ArmComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("ArmComponent"));
	ArmComponent->SetupAttachment(BodyComponent);
	LegComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("LegComponent"));
	LegComponent->SetupAttachment(BodyComponent);
	FootComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("FootComponent"));
	FootComponent->SetupAttachment(BodyComponent);
	HairComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("HairComponent"));
	HairComponent->SetupAttachment(HeadComponent);

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(GetRootComponent());
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm);

	FirstWeaponComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("FirstWeapon"));
	FirstWeaponComponent->SetupAttachment(BodyComponent, FName(TEXT("FirstWeapon")));
	FirstWeaponCoverComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("FirstWeaponCover"));
	FirstWeaponCoverComponent->SetupAttachment(BodyComponent, FName(TEXT("FirstWeapon")));

	SecondWeaponComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SecondWeapon"));
	SecondWeaponComponent->SetupAttachment(BodyComponent, FName(TEXT("SecondWeapon")));
	SecondWeaponCoverComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SecondWeaponCover"));
	SecondWeaponCoverComponent->SetupAttachment(BodyComponent, FName(TEXT("SecondWeapon")));

	SkillActionComponent = CreateDefaultSubobject<USkillActionComponent>(TEXT("SkillActionComponent"));
#pragma endregion

	SpringArm->bUsePawnControlRotation = true;
	bUseControllerRotationYaw = false;
	GetCharacterMovement()->bOrientRotationToMovement = true;

	//응격UI 기본 세팅

	CounterAttackWidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("CounterAttackWidget"));
	CounterAttackWidgetComponent->SetupAttachment(GetRootComponent());
	static ConstructorHelpers::FClassFinder<UUserWidget> CounterAttackWidgetClassFinder(
		TEXT("/Game/Blueprint/PlayableCharacter/UI/BP_CounterAttack"));
		//TEXT("/Game/Blueprint/PlayableCharacter/UI/BP_CounterAttack.BP_CounterAttack_C"));
	if (CounterAttackWidgetClassFinder.Succeeded())
	{
		CounterAttackWidgetComponent->SetWidgetClass(CounterAttackWidgetClassFinder.Class);
	}
	CounterAttackWidgetComponent->SetWidgetSpace(EWidgetSpace::Screen);
	
}

// Called when the game starts or when spawned
void APlayableBaseCharacter::BeginPlay()
{
	Super::BeginPlay();
	CounterAttackWidget = Cast<UCounterAttackUI>(CounterAttackWidgetComponent->GetUserWidgetObject());
	InitializeIconUI();
	CounterAttackWidgetComponent->SetVisibility(false);
}

// Called every frame
void APlayableBaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if(isSprint && GetVelocity().Size2D() < WalkSpeed)
	{
		SetWalk();
	}
	AController* CurrentController = GetController();
	if(APlayerController* PC = Cast<APlayerController>(CurrentController))
	{
		if (CurrentTarget && !CurrentTarget->IsDead())
		{
			if (PC && PC->PlayerCameraManager)
			{
				FVector CameraLocation = PC->PlayerCameraManager->GetCameraLocation();
				FVector TargetLocation = CurrentTarget->GetActorLocation() + FVector(0.f, 0.f, 80.0f);
				FRotator TargetRotation = UKismetMathLibrary::FindLookAtRotation(CameraLocation, TargetLocation);

				FRotator CurrentControlRotation = PC->GetControlRotation();
				FRotator NewControlRotation = UKismetMathLibrary::RInterpTo(
					CurrentControlRotation,
					TargetRotation,
					DeltaTime,
					CameraTargetingInterpSpeed
				);
				NewControlRotation.Roll = 0.f;
				PC->SetControlRotation(NewControlRotation);
			}
		}
		else if (CurrentTarget && CurrentTarget->IsDead())
		{
			LockOnBestTarget();
		}
	}
	else if (AAIController* AICon = Cast<AAIController>(CurrentController))
	{
		if (isCombatMode && CurrentTarget && !CurrentTarget->IsDead())
		{
			FVector AILocation = GetActorLocation();
			FVector TargetLocation = CurrentTarget->GetActorLocation();

			FRotator TargetRotation = UKismetMathLibrary::FindLookAtRotation(AILocation, TargetLocation);

			TargetRotation.Pitch = 0.0f;
			TargetRotation.Roll = 0.0f;
			float AIRotationInterpSpeed = 10.0f;
			FRotator NewActorRotation = UKismetMathLibrary::RInterpTo(
				GetActorRotation(),
				TargetRotation,
				DeltaTime,
				AIRotationInterpSpeed
			);

			SetActorRotation(NewActorRotation);
		}
	}
}

void APlayableBaseCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	InitializeIconUI();
}

void APlayableBaseCharacter::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	if (BodyComponent)
	{
		if (HeadComponent)
			HeadComponent->SetLeaderPoseComponent(BodyComponent);
		if (ArmComponent)
			ArmComponent->SetLeaderPoseComponent(BodyComponent);
		if (LegComponent)
			LegComponent->SetLeaderPoseComponent(BodyComponent);
		if (FootComponent)
			FootComponent->SetLeaderPoseComponent(BodyComponent);
		if (HairComponent)
			HairComponent->SetLeaderPoseComponent(BodyComponent);
	}
}

// Called to bind functionality to input
void APlayableBaseCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void APlayableBaseCharacter::SetIsActionLock(bool Lock)
{
	if (bIsActionLock == Lock)
		return;
	bIsActionLock = Lock;
	AController* CurrentController = GetController();
	if (!CurrentController)
		return;

	if (APlayerController* PC = Cast<APlayerController>(CurrentController))
	{
		PC->SetIgnoreMoveInput(Lock);
		UE_LOG(LogTemp, Warning,
			TEXT("SetIsActionLock : %s / SetIgnoreMoveInput : %s"),
			Lock ? TEXT("true") : TEXT("false"),
			GetController()->IsMoveInputIgnored()
			? TEXT("true")
			: TEXT("false"));
	}
	else if (AAIController* AICon = Cast<AAIController>(CurrentController))
	{
		UBrainComponent* BrainComp = AICon->GetBrainComponent();

		if (Lock)
		{
			AICon->StopMovement();

			if (BrainComp)
			{
				BrainComp->PauseLogic(TEXT("ActionLock"));
			}
		}
		else
		{
			if (BrainComp)
			{
				BrainComp->ResumeLogic(TEXT("ActionLock"));
			}
		}
	}
}

void APlayableBaseCharacter::SetMoveSpeed()
{
	if (isSprint)
		SetWalk();
	else
		SetSprint();
}

void APlayableBaseCharacter::SetSprint()
{
	if (UBaseStateComponent* StatusComp = GetStatusComponent())
	{
		StatusComp->SetBaseWalkSpeed(SprintSpeed);
	}
	isSprint = true;

}

void APlayableBaseCharacter::SetWalk()
{
	if (UBaseStateComponent* StatusComp = GetStatusComponent())
	{
		StatusComp->SetBaseWalkSpeed(WalkSpeed);
	}
	isSprint = false;
}

void APlayableBaseCharacter::PlayEvade()
{
	if(GetIsActionLock())
		return;

	if (nullptr != EvadeMontage &&
		GetMovementComponent()->IsFalling() == false &&
		BodyComponent->GetAnimInstance()->Montage_IsPlaying(EvadeMontage) == false &&
		GetMesh()->GetAnimInstance()->GetCurrentActiveMontage() == nullptr)
	{
		PlayMontageFullBody(EvadeMontage);

		//저스트회피 성공 판정
		SetPerfectDodgeWindow(true);
		GetWorldTimerManager().ClearTimer(PerfectDodgeTimerHandle);
		FTimerDelegate TimerDel;
		TimerDel.BindUObject(this, &APlayableBaseCharacter::SetPerfectDodgeWindow, false);
		//Test용 3초, 향후 수정 예정
		GetWorldTimerManager().SetTimer(PerfectDodgeTimerHandle, TimerDel, 0.5f, false);
	}
}

void APlayableBaseCharacter::SetBrakingDecelerationFalling()
{
	GetCharacterMovement()->BrakingDecelerationFalling = GetCharacterMovement()->MaxWalkSpeed;
}

void APlayableBaseCharacter::Jump()
{
	if (GetIsActionLock())
		return;
	Super::Jump();
}

bool APlayableBaseCharacter::PlayJumpMontage()
{
	if (GetMovementComponent()->IsFalling() == false &&
		nullptr != JumpMontage &&
		BodyComponent->GetAnimInstance()->Montage_IsPlaying(JumpMontage) == false &&
		IsEvading() == false)
	{
		PlayMontageFullBody(JumpMontage);
		return true;
	}
	return false;
}

void APlayableBaseCharacter::PlayJump()
{
	if (GetIsActionLock())
		return;

	if(PlayJumpMontage())
	{
		SetBrakingDecelerationFalling();
	}
}

bool APlayableBaseCharacter::PlayMontageFullBody(TObjectPtr<UAnimMontage> Montage, FName SectionName, float MontageSpeed)
{
	if(Montage == nullptr)
		return false;
	GetMesh()->GetAnimInstance()->Montage_Play(Montage, MontageSpeed);

	if(SectionName.IsNone() == false)
	{
		GetMesh()->GetAnimInstance()->Montage_JumpToSection(SectionName, Montage);
	}
	return true;
}

void APlayableBaseCharacter::SetCombatMode()
{
	//Test Code
	isCombatMode = !isCombatMode;
	if (!isCombatMode)
	{
		StopMontage(nullptr, 0.1f);
		SetIsActionLock(true);
		RequestUnblockContinuousInput();
		UE_LOG(LogTemp, Warning, TEXT("BBBBB"));
		GetCurSwordStanceComponent()->ResetAttackInfo();
		GetCurSwordStanceComponent()->ResetNextAttack();
		//전투 종료 후 검을 쥐고 있을 때만 검을 넣음
		if (IsWeaponEquip)
		{
			PlayEquipWeaponStateMontage_New(isCombatMode);
		}
		else
		{
			SetIsActionLock(false);
		}
	}
	else
	{
		PlayEquipWeaponStateMontage_New(isCombatMode);
	}
	SetWalk();
	FString ModeText = isCombatMode ? TEXT("true") : TEXT("false");
	GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Blue, TEXT("CombatMode : ") + ModeText);
	InitializeSwordStance();
	AController* CurrentController = GetController();
	if (AAIController* AICon = Cast<AAIController>(CurrentController))
	{
		if (UBlackboardComponent* BB = AICon->GetBlackboardComponent())
		{
			if (isCombatMode)
			{
				// 만약 이오리가 타겟으로 박혀있다면 즉시 클리어하여 이오리에게 달려가는 오동작 방지
				AActor* CurrentTargetActor = Cast<AActor>(BB->GetValueAsObject(FName("TargetActor")));
				APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);

				if (CurrentTargetActor == PlayerPawn)
				{
					BB->ClearValue(FName("TargetActor"));
				}
			}
		}
	}
}

void APlayableBaseCharacter::ResetCameraPosition()
{
	APlayerController* PlayerController = Cast<APlayerController>(GetController());
	FRotator ControlRotation = GetActorRotation();
	ControlRotation.Pitch = 0.0f;
	ControlRotation.Roll = 0.0f;
	PlayerController->SetControlRotation(ControlRotation);
}

void APlayableBaseCharacter::PlayEquipWeaponMontage()
{
	//if (GetMovementComponent()->IsFalling() == true ||
	if (BodyComponent->GetAnimInstance()->Montage_IsPlaying(CurSwordStanceComponent->GetNormalAttackMontage()) == true ||
		BodyComponent->GetAnimInstance()->Montage_IsPlaying(CurSwordStanceComponent->GetHeavyAttackMontage()) == true)
		return;
	UAnimMontage* Montage = nullptr;
	if (isCombatMode == true)
	{
		if (nullptr == EquipMontage ||
			BodyComponent->GetAnimInstance()->Montage_IsPlaying(EquipMontage) == true)
			return;
		Montage = EquipMontage;
	}
	else
	{
		if (nullptr == UnEquipMontage ||
			BodyComponent->GetAnimInstance()->Montage_IsPlaying(UnEquipMontage) == true)
			return;
		Montage = UnEquipMontage;
	}

	SetIsActionLock(true);
	PlayMontageFullBody(Montage);
}

void APlayableBaseCharacter::WeaponEquip()
{
	FirstWeaponComponent->AttachToComponent(BodyComponent,
		FAttachmentTransformRules::SnapToTargetNotIncludingScale,
		FName(TEXT("FirstWeaponHand")));
	SetIsWeaponEquip(true);
}

void APlayableBaseCharacter::WeaponUnEquip()
{
	FirstWeaponComponent->AttachToComponent(BodyComponent,
		FAttachmentTransformRules::SnapToTargetNotIncludingScale,
		FName(TEXT("FirstWeapon")));
	SetIsWeaponEquip(false);
}

void APlayableBaseCharacter::PlayEquipWeaponStateMontage_New(bool bIsEquip)
{
	if (BodyComponent->GetAnimInstance()->Montage_IsPlaying(CurSwordStanceComponent->GetNormalAttackMontage()) == true ||
		BodyComponent->GetAnimInstance()->Montage_IsPlaying(CurSwordStanceComponent->GetHeavyAttackMontage()) == true ||
		BodyComponent->GetAnimInstance()->Montage_IsPlaying(EquipMontage) == true ||
		BodyComponent->GetAnimInstance()->Montage_IsPlaying(UnEquipMontage) == true)
		return;
	TObjectPtr<UAnimMontage> TargetMontage = nullptr;

	TargetMontage = bIsEquip ? EquipMontage : UnEquipMontage;

	if (TargetMontage != nullptr)
	{
		SetIsActionLock(true);
		PlayMontageFullBody(TargetMontage);
	}
}

void APlayableBaseCharacter::InitializeSwordStance()
{
	if (CurSwordStanceComponent == nullptr)
		return;
	CurSwordStanceComponent->InitSwordStance();
	if(CurSwordStanceComponent->GetHikenDataAsset() != nullptr)
		SkillActionComponent->SetHikenSkill(CurSwordStanceComponent->GetHikenDataAsset());
}

void APlayableBaseCharacter::InitializeStatus()
{
	//StatusComponent->InitState(NewStat);
}

void APlayableBaseCharacter::TryInitializeUI()
{
}

void APlayableBaseCharacter::ExecuteHeal(float RecoverAmount)
{
	GetStatusComponent()->RecoverHP(RecoverAmount);
}

void APlayableBaseCharacter::StopMontage(TObjectPtr<UAnimMontage> Montage, float blend)
{
	BodyComponent->GetAnimInstance()->Montage_Stop(blend, Montage);
	HeadComponent->GetAnimInstance()->Montage_Stop(blend, Montage);
	HairComponent->GetAnimInstance()->Montage_Stop(blend, Montage);
	ArmComponent->GetAnimInstance()->Montage_Stop(blend, Montage);
	LegComponent->GetAnimInstance()->Montage_Stop(blend, Montage);
	FootComponent->GetAnimInstance()->Montage_Stop(blend, Montage);
}

void APlayableBaseCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();


	HeadComponent->SetMasterPoseComponent(GetMesh());
	HairComponent->SetMasterPoseComponent(GetMesh());
	ArmComponent->SetMasterPoseComponent(GetMesh());
	LegComponent->SetMasterPoseComponent(GetMesh());
	FootComponent->SetMasterPoseComponent(GetMesh());

	if (USkeletalMeshComponent* CharacterMesh = GetMesh())
	{
		if (UAnimInstance* AnimInstance = CharacterMesh->GetAnimInstance())
		{
			GetMesh()->GetAnimInstance()->OnMontageEnded.AddUniqueDynamic(this, &APlayableBaseCharacter::AttackMontageEnded);
			GetMesh()->GetAnimInstance()->OnMontageEnded.AddUniqueDynamic(this, &APlayableBaseCharacter::OnMontageEndedGeneral);
			GetMesh()->GetAnimInstance()->OnMontageEnded.AddUniqueDynamic(this, &APlayableBaseCharacter::EquipMontageEnded);
			GetMesh()->GetAnimInstance()->OnMontageEnded.AddUniqueDynamic(this, &APlayableBaseCharacter::UnEquipMontageEnded);
		}
	}
	
	SkillActionComponent->BindSkillMontageDelegate(GetMesh()->GetAnimInstance());
}

void APlayableBaseCharacter::AttackMontageStarted(UAnimMontage* Montage)
{
	
}

void APlayableBaseCharacter::AttackMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	if (CurSwordStanceComponent == nullptr)
		return;
	if (Montage != CurSwordStanceComponent->GetHeavyAttackMontage() && Montage != CurSwordStanceComponent->GetNormalAttackMontage())
		return;

	//공격 성공 여부와 관계없이 특수 공격력 초기화
	GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Blue, FString::Printf(TEXT("SpeicalAttackPower : %d, Montage : %s, Interrupted : %s"), CurSwordStanceComponent->GetSpeicalAttackPower(), *Montage->GetName(), bInterrupted ? TEXT("true") : TEXT("false")));

	//공격이 중단되었을 때
	if (bInterrupted == true)
	{
		//공격이 중단되었지만 다음 공격으로의 전환이 이루어지고 있지 않을 때 공격 정보 초기화
		//피격과 같은 강제적인 중단이 발생했을 때
		if(CurSwordStanceComponent->GetbIsNextAttackTransitioning() == false)
		{
			CurSwordStanceComponent->ResetAttackInfo();
		}
		//다음 공격으로의 전환이 이루어지고 있을 때 공격 정보 초기화 지연
		CurSwordStanceComponent->ResetbIsNextAttackTransitioning();
	}
	//공격이 중단되지 않고 정상적으로 끝났을 때 공격 정보 초기화
	else
	{
		CurSwordStanceComponent->ResetAttackInfo();
	}
}

void APlayableBaseCharacter::StopAttackMontage()
{
	UAnimInstance* AnimInstance = BodyComponent->GetAnimInstance();
	if (!AnimInstance)
		return;
	UAnimMontage* NormalAttackMontage =
		CurSwordStanceComponent->GetNormalAttackMontage();

	UAnimMontage* HeavyAttackMontage =
		CurSwordStanceComponent->GetHeavyAttackMontage();

	if (AnimInstance->Montage_IsPlaying(NormalAttackMontage))
	{
		AnimInstance->Montage_Stop(0.1f, NormalAttackMontage);
	}

	if (AnimInstance->Montage_IsPlaying(HeavyAttackMontage))
	{
		AnimInstance->Montage_Stop(0.1f, HeavyAttackMontage);
	}
}

void APlayableBaseCharacter::LockOnBestTarget()
{
	if (CurrentTarget != nullptr)
	{
		CurrentTarget->SetLockOnMarkerVisibility(false);
	}
	if (nullptr == CurrentCombatZone)
	{
		CurrentTarget = nullptr;
		return;
	}
	TArray<ABaseMonster*> TrackedMonsters = CurrentCombatZone->GetLiveMonsters();
	TrackedMonsters.RemoveAll([](ABaseMonster* Monster) {
		return nullptr == Monster || Monster->IsDead();
		});

	if (TrackedMonsters.Num() == 0)
	{
		CurrentTarget = nullptr;
		return;
	}
	FVector PlayerLoc = GetActorLocation();
	Algo::Sort(TrackedMonsters, [PlayerLoc](ABaseMonster* A, ABaseMonster* B)
		{
			uint8 TierA = (uint8)A->GetMonsterStateComponent()->GetMonsterTier();
			uint8 TierB = (uint8)B->GetMonsterStateComponent()->GetMonsterTier();

			if (TierA != TierB)
			{
				return TierA > TierB;
			}

			float DistA = FVector::DistSquared(PlayerLoc, A->GetActorLocation());
			float DistB = FVector::DistSquared(PlayerLoc, B->GetActorLocation());
			return DistA < DistB;
		});

	CurrentTarget = TrackedMonsters[0];
	CurrentTarget->SetLockOnMarkerVisibility(true);
}

void APlayableBaseCharacter::SetTargetingMode(bool bEnable)
{
	if (!bEnable)
	{
		if(CurrentTarget != nullptr)
		{
			CurrentTarget->SetLockOnMarkerVisibility(false);
			CurrentTarget = nullptr;
		}
	}
}

void APlayableBaseCharacter::OnTargetingPressed()
{
	if (CurrentTarget != nullptr)
		SetTargetingMode(false);
	else
		LockOnBestTarget();
}

void APlayableBaseCharacter::SwitchTarget(bool bSwitchRight)
{
	if (CurrentTarget == nullptr || CurrentCombatZone == nullptr)
		return;
	APlayerController* PC = Cast<APlayerController>(GetController());
	if (PC == nullptr)
		return;

	FVector2D CurrentScreenPos;
	if (!PC->ProjectWorldLocationToScreen(CurrentTarget->GetActorLocation(), CurrentScreenPos))
		return;

	TArray<ABaseMonster*> TrackedMonsters = CurrentCombatZone->GetLiveMonsters();
	//타겟 변경 방향(현재 타겟 몬스터 기준 왼쪽, 오른쪽)의 몬스터 수집
	TArray<ABaseMonster*> CandidateMonsters;
	for (ABaseMonster* Monster : TrackedMonsters)
	{
		if (Monster == nullptr || Monster == CurrentTarget || Monster->IsDead())
			continue;
		FVector2D CandidateScreenPos;
		if (PC->ProjectWorldLocationToScreen(Monster->GetActorLocation(), CandidateScreenPos))
		{
			//bSwitchRight true면 오른쪽 false면 왼쪽
			if (bSwitchRight && CandidateScreenPos.X > CurrentScreenPos.X)
			{
				CandidateMonsters.Add(Monster);
			}
			else if (!bSwitchRight && CandidateScreenPos.X < CurrentScreenPos.X)
			{
				CandidateMonsters.Add(Monster);
			}
		}
	}
	//변경 대상이 없으면 종료
	if (CandidateMonsters.Num() == 0)
		return;
	Algo::Sort(CandidateMonsters, [PC, CurrentScreenPos](ABaseMonster* A, ABaseMonster* B)
		{
			uint8 TierA = (uint8)A->GetMonsterStateComponent()->GetMonsterTier();
			uint8 TierB = (uint8)B->GetMonsterStateComponent()->GetMonsterTier();

			if (TierA != TierB) { return TierA > TierB; }

			FVector2D ScreenPosA, ScreenPosB;
			PC->ProjectWorldLocationToScreen(A->GetActorLocation(), ScreenPosA);
			PC->ProjectWorldLocationToScreen(B->GetActorLocation(), ScreenPosB);

			float DistA = FMath::Abs(ScreenPosA.X - CurrentScreenPos.X);
			float DistB = FMath::Abs(ScreenPosB.X - CurrentScreenPos.X);

			return DistA < DistB;
		});

	CurrentTarget->SetLockOnMarkerVisibility(false);
	CurrentTarget = CandidateMonsters[0];
	CurrentTarget->SetLockOnMarkerVisibility(true);
}

void APlayableBaseCharacter::SnapToTargetEnemy()
{
	if (nullptr == CurrentTarget)
		return;
	FVector MyLocation = GetActorLocation();
	FVector TargetLocation = CurrentTarget->GetActorLocation();

	TargetLocation.Z = MyLocation.Z;

	FRotator LookAtRotation = UKismetMathLibrary::FindLookAtRotation(MyLocation, TargetLocation);

	SetActorRotation(LookAtRotation);
}

void APlayableBaseCharacter::ResetCounterAttackTimer()
{
	GetWorld()->GetTimerManager().ClearTimer(GuardCounterAttackTimerHandle);
	DisableCounterAttack();
}

bool APlayableBaseCharacter::IsPlayingAttackMontage() const
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance == nullptr)
		return false;
	if (AnimInstance->Montage_IsPlaying(CurSwordStanceComponent->GetNormalAttackMontage()) ||
		AnimInstance->Montage_IsPlaying(CurSwordStanceComponent->GetHeavyAttackMontage()))
		return true;
	return false;
}

EWeaponVFXTarget APlayableBaseCharacter::GetCurrentWeaponVFXTarget() const
{
	return CurSwordStanceComponent->GetWeaponVFXTaraget();
}

float APlayableBaseCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	AController* CurrentController = GetController();
	bool bIsControlledByAI = false;
	if (CurrentController != nullptr)
	{
		AAIController* AIController = Cast<AAIController>(CurrentController);

		if (AIController != nullptr)
		{
			bIsControlledByAI = true;
		}
	}
	float ActualDamage = 0.0f;
	//AI가 컨트롤 중일 때는 데미지를 받지 않음
	if(!bIsControlledByAI)
	{
		float DefencePower = StatusComponent->GetTotalDefencePower();
		//최소데미지 1로 설정
		DamageAmount = FMath::Max(DamageAmount - DefencePower, 1.0f);
		//공격 형에 따른 조정
		//땅의 형 쉴드
		//불의 형 데미지 계산 이후 공격력, 공격 속도 증가
		DamageAmount = CurSwordStanceComponent->SwordStanceBeforeUpdateHp(DamageAmount);
		ActualDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
		CurSwordStanceComponent->SwordStanceAfterUpdateHp(ActualDamage);
		StatusComponent->TakeDamage(ActualDamage);
	}


	if(CurSwordStanceComponent->GetIsCharging())
	{
		//저스트 가드 판정
		IsCanGuardConuterAttack = true;
		//1.5초 후 저스트 가드 판정 초기화
		GetWorld()->GetTimerManager().ClearTimer(GuardCounterAttackTimerHandle);
		GetWorld()->GetTimerManager().SetTimer(
			GuardCounterAttackTimerHandle,
			this,
			&APlayableBaseCharacter::DisableCounterAttack,
			1.5f,
			false);
	}
	else
	{
		UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
		if (AnimInstance)
		{
			//StopMontage(HitMontage);
				PlayMontageFullBody(HitMontage);
				
				SetIsActionLock(true);
		}
	}

	return ActualDamage;
}

void APlayableBaseCharacter::OnPerfectDodgeSuccess(AActor* Attacker)
{
	bIsPerfectDodgeWindow = false;
	bIsWaitingForCounterInput = true;
	LastAttacker = Cast<ACharacter>(Attacker);
	//저스트 회피 UI
	CounterAttackWidgetComponent->SetVisibility(true);

	SetIsActionLock(true);

	float DelayTime = 0.05f;
	UGameplayStatics::SetGlobalTimeDilation(GetWorld(), DelayTime);

	float RealTimeDelay = 3.0f;
	float ScaledDelay = RealTimeDelay * DelayTime;
	GetWorldTimerManager().ClearTimer(CounterInputTimerHandle);
	GetWorldTimerManager().SetTimer(CounterInputTimerHandle, this, &APlayableBaseCharacter::EndCounterInputWindow, ScaledDelay, false);
}

void APlayableBaseCharacter::EndCounterInputWindow()
{
	if(!bIsWaitingForCounterInput)
		return;
	bIsWaitingForCounterInput = false;
	SetIsActionLock(false);
	LastAttacker = nullptr;
	CounterAttackWidgetComponent->SetVisibility(false);
	GetWorldTimerManager().ClearTimer(CounterInputTimerHandle);
	UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 1.0f);
}

void APlayableBaseCharacter::AttackTrace(EAttackVariety AttackVariety)
{
	FAttackData AttackData;
	//통상 공격류는 소드 스탠스 컴포넌트에서 공격 데이터 가져오기
	if (AttackVariety == EAttackVariety::Normal || AttackVariety == EAttackVariety::Heavy || AttackVariety == EAttackVariety::Counter)
	{
		AttackData = CurSwordStanceComponent->GetAttackData(AttackVariety);
	}
	//스킬, 비검류는 스킬 액션 컴포넌트에서 공격 데이터 가져오기
	else if (AttackVariety == EAttackVariety::Special || AttackVariety == EAttackVariety::Skill)
	{
		AttackData = SkillActionComponent->GetCurrentActiveSkillAttackData();
		//적 모으기 종료
		SkillActionComponent->EndGathering();
	}
	TArray<FHitResult> HitResults;
	bool isHit = UKismetSystemLibrary::BoxTraceMulti(
		this,
		GetActorLocation(), // 박스의 시작 위치
		GetActorLocation() + GetActorForwardVector() * AttackData.AttackTraceData.ForwardDistance, // 박스의 끝 위치
		FVector(AttackData.AttackTraceData.BoxHalfSize), // 박스의 반지름 (X, Y, Z)
		FRotator::ZeroRotator, // 박스의 회전값
		UEngineTypes::ConvertToTraceType(ECC_GameTraceChannel4), // 트레이스 채널
		false, // 복잡한 충돌첼 충돌 무시 여부
		{}, // 무시할 액터 배열
		EDrawDebugTrace::ForDuration, // 디버그 드로잉 옵션
		HitResults, //HitResults에 결과 저장
		true// Trace에 자기을 무시할지 여부
	);

	if (isHit)
	{
		//플레이어 스탯, 소드 스탠스 컴포넌트에서 데미지 가져오기
		//수정 예정

		CurSwordStanceComponent->SwordStanceUpdateAttack();
		int Damage;
		if (AttackVariety == EAttackVariety::Skill)
			Damage = StatusComponent->GetMat();
		else
			Damage = StatusComponent->GetTotalAttackPower() + CurSwordStanceComponent->GetSpeicalAttackPower();
		
		//데미지 랜덤화 (0.8~1.2배)
		const float Rand = FMath::FRandRange(0.8f, 1.2f);
		int32 FinalDamage = Damage * Rand;
		//크리티컬 및 강공격 사용 여부 확인 bool 함수 작성 예정
		//if (CurSwordStanceComponent->GetIsPlayHeavyAttackMontage())
		FinalDamage *= AttackData.DamageMultiplier;

		GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Red, FString::Printf(TEXT("Damage : %d"), FinalDamage));
		for(const FHitResult& HitResult : HitResults)
		{
			ABaseMonster* Monster = Cast<ABaseMonster>(HitResult.GetActor());
			if(Monster != nullptr)
			{
				//Monster->HitBy(FinalDamage);
				float ActualDamage = UGameplayStatics::ApplyDamage(Monster, FinalDamage, GetController(), this, UDamageType::StaticClass());
				UBaseStateComponent* AttackerState = GetStatusComponent();
				UBaseStateComponent* VictimState = Monster->GetMonsterStateComponent();

				if (AttackerState && VictimState)
				{
					for (const FDebuffData& OnHitDebuff : AttackerState->GetActiveOnHitDebuffs())
					{
						VictimState->ApplyDebuff(OnHitDebuff);
					}
				}
				
				//비검, 공명, 서번트 게이지 상승
				if(ActualDamage > 0.0f)
				{
					if (AFatePlayerState* FatePlayerState = Cast<AFatePlayerState>(GetPlayerState()))
					{
						UResonanceComponent* RC = FatePlayerState->ResonanceComponent;
						if(RC->GetServantActive() == true)
						{
							RC->CalculateLinkSkillGauge(ActualDamage, Monster->GetMonsterMaxHP());
							//서번트로 변경된상태에서는 서번트 게이지 안 오름
							if(RC->GetCurrentActiveServant() == NAME_None)
							{
								RC->CalculateServnatGauge(ActualDamage, Monster->GetMonsterMaxHP());
							}
						}

					}
					//비검, 스킬은 게이지 안 채움
					if (AttackVariety != EAttackVariety::Special && AttackVariety != EAttackVariety::Skill)
					{
						StatusComponent->CalculateHikenGauge(ActualDamage, Monster->GetMonsterMaxHP());
					}
				}
			}
		}
	}
}

void APlayableBaseCharacter::OnMontageEndedGeneral(UAnimMontage* Montage, bool bInterrupted)
{
	//EquipMontageEnded, UnEquipMontageEnded에서 처리하므로 패스
	if (Montage == UnEquipMontage || Montage == EquipMontage)
		return;

	/*if (GetCharacterMovement()->IsFalling() == true)
		return;*/

	ProcessMontageEndedGeneral(Montage, bInterrupted);
}
void APlayableBaseCharacter::SetStrafeMovementMode(bool bIsStrafing)
{
	if (UCharacterMovementComponent* MoveComp = GetCharacterMovement())
	{
		MoveComp->bOrientRotationToMovement = !bIsStrafing;
		MoveComp->bUseControllerDesiredRotation = bIsStrafing;

		if (bIsStrafing)
		{
			MoveComp->RotationRate = FRotator(0.0f, 500.0f, 0.0f);
		}
	}
}
void APlayableBaseCharacter::ClearLockOnTargetAI()
{
	SetTargetingMode(false);
	if (AAIController* AICon = Cast<AAIController>(GetController()))
	{
		if (UBlackboardComponent* BB = AICon->GetBlackboardComponent())
		{
			BB->ClearValue(FName("TargetActor"));
		}
	}
}
void APlayableBaseCharacter::ProcessMontageEndedGeneral(UAnimMontage* Montage, bool bInterrupted)
{
	if (bInterrupted)
		return;
	if (SkillActionComponent &&
		SkillActionComponent->IsPlayingCinematic())
	{
		return;
	}
	UE_LOG(LogTemp, Warning,
		TEXT("Montage End : %s Interrupted=%d"),
		*GetNameSafe(Montage),
		bInterrupted);

	if(bInterrupted == false && GetCharacterMovement()->IsFalling() == false)
	{
		SetIsActionLock(false);
	}
}

FKey APlayableBaseCharacter::GetCounterAttackInputKey() const
{
	//InputMappingContext 데이터를 바로 직접 참고하는 방식
	//선정이유
	//1. 게임 시작 후 IMC에 등록해도 내부에서 빌드하는 약간의 시간으로 인해 키를 받아오지 못하는 현상이 발생
	//NextTick으로 처리해도 동일한 현상이 발생
	//0.1초의 딜레이를 주었을 때 정상적으로 키를 받아오는 것을 확인
	//이런 문제를 해결하기 위해 IMC 데이터를 직접 참고하는 방식으로 변경
	if (AMiyamotoIoriController* PC = Cast<AMiyamotoIoriController>(GetController()))
	{
		UInputMappingContext* IMC = PC->GetDefaultMappingContext();

		if (!IMC)
			return EKeys::Invalid;

		const TArray<FEnhancedActionKeyMapping>& Mappings = IMC->GetMappings();
		TArray<FKey> MappedKeys;
		for (const FEnhancedActionKeyMapping& Mapping : Mappings)
		{
			if (Mapping.Action == PC->GetNormalAttackAction())
			{
				MappedKeys.Add(Mapping.Key);
			}
		}
		if (MappedKeys.Num() > 0)
		{
			return MappedKeys[0];
		}
	}

	return EKeys::Invalid;
}

void APlayableBaseCharacter::InitializeIconUI()
{
	if (CounterAttackWidget)
	{
		FKey Key = GetCounterAttackInputKey();
		if (EKeys::Invalid == Key)
		{
			Key = EKeys::I;
		}
		CounterAttackWidget->UpdateKeyIcon(Key);
	}
}

void APlayableBaseCharacter::EquipMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	if (Montage == nullptr || Montage != EquipMontage)
		return;

	if (bInterrupted)
		return;

	SetIsActionLock(false);
}

void APlayableBaseCharacter::UnEquipMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	if (Montage == nullptr || Montage != UnEquipMontage)
		return;

	if (bInterrupted)
		return;

	SetIsActionLock(false);
}