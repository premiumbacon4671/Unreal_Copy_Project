// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayableCharacter/PlayableBaseCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "PlayableCharacter/Miyamoto_Iori/ActorComponent/BaseSwordStanceActorComponent.h"
#include "ActorComponent/StateComponent/PlayableStateComponent.h"
#include "Monster/BaseMonster.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "UI/CounterAttackUI.h"
#include "Components/WidgetComponent.h"
#include "Controller/MiyamotoIoriController/MiyamotoIoriController.h"

#include "EnhancedInputSubsystems.h"
#include "EnhancedInputSubsystemInterface.h"
#include "EnhancedInputComponent.h"
#include "InputMappingContext.h"
#include "InputAction.h"
//#include "EnhancedInputLocalPlayerSubsystem.h" // 인풋 서브시스템용
#include "Blueprint/UserWidget.h"              // CreateWidget 및 UI용

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
#pragma endregion

#pragma region Montage
	static ConstructorHelpers::FObjectFinder<UAnimMontage> JumpMontageFinder(
		TEXT("/Script/Engine.AnimMontage'/Game/Blueprint/PlayableCharacter/MiyamotoIori/Animation/AM_Jump.AM_Jump'"));
	if (JumpMontageFinder.Succeeded())
		JumpMontage = JumpMontageFinder.Object;

	static ConstructorHelpers::FObjectFinder<UAnimMontage> EquipMontageFinder(
		TEXT("/Script/Engine.AnimMontage'/Game/Blueprint/PlayableCharacter/MiyamotoIori/Animation/AM_Equip.AM_Equip'"));
	if (EquipMontageFinder.Succeeded())
		EquipMontage = EquipMontageFinder.Object;

	static ConstructorHelpers::FObjectFinder<UAnimMontage> UnEquipMontageFinder(
		TEXT("/Script/Engine.AnimMontage'/Game/Blueprint/PlayableCharacter/MiyamotoIori/Animation/AM_UnEquip.AM_UnEquip'"));
	if (UnEquipMontageFinder.Succeeded())
		UnEquipMontage = UnEquipMontageFinder.Object;

	static ConstructorHelpers::FObjectFinder<UAnimMontage> EvadeMontageFinder(
		TEXT("/Script/Engine.AnimMontage'/Game/Blueprint/PlayableCharacter/Animation/AM_Evade.AM_Evade'"));
	if (EvadeMontageFinder.Succeeded())
		EvadeMontage = EvadeMontageFinder.Object;
#pragma endregion

	SpringArm->bUsePawnControlRotation = true;
	bUseControllerRotationYaw = false;
	GetCharacterMovement()->bOrientRotationToMovement = true;

	//응격UI 기본 세팅

	CounterAttackWidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("CounterAttackWidget"));
	CounterAttackWidgetComponent->SetupAttachment(GetRootComponent());
	static ConstructorHelpers::FClassFinder<UUserWidget> CounterAttackWidgetClassFinder(
		TEXT("/Game/Blueprint/PlayableCharacter/UI/BP_CounterAttack.BP_CounterAttack_C"));
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
	GEngine->AddOnScreenDebugMessage(2, 0.f, FColor::Yellow, FString::Printf(TEXT("ActionLock : %s"), bIsActionLock ? TEXT("true") : TEXT("false")));
	if(isSprint && GetVelocity().Size2D() < WalkSpeed)
	{
		SetWalk();
	}
}

void APlayableBaseCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	//CounterAttackUI->SetVisibility(ESlateVisibility::Hidden);
}

// Called to bind functionality to input
void APlayableBaseCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void APlayableBaseCharacter::SetIsActionLock(bool Lock)
{
	bIsActionLock = Lock;
	if(GetController())
	{
		GetController()->SetIgnoreMoveInput(Lock);
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
	GetCharacterMovement()->MaxWalkSpeed = SprintSpeed;
	isSprint = true;

}

void APlayableBaseCharacter::SetWalk()
{
	GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
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
	}
}

void APlayableBaseCharacter::SetBrakingDecelerationFalling()
{
	GetCharacterMovement()->BrakingDecelerationFalling = GetCharacterMovement()->MaxWalkSpeed;
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
	//isCombatMode = true;
	isCombatMode = !isCombatMode;
	//PlayEquipWeaponMontage();
	PlayEquipWeaponStateMontage_New(isCombatMode);
	FString ModeText = isCombatMode ? TEXT("true") : TEXT("false");
	GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Blue, TEXT("CombatMode : ") + ModeText);
	CurSwordStanceComponent->InitSwordStance();
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
	//GetController()->SetIgnoreMoveInput(true);
	PlayMontageFullBody(Montage);
}

void APlayableBaseCharacter::WeaponEquip()
{
	FirstWeaponComponent->AttachToComponent(BodyComponent,
		FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true),
		FName(TEXT("FirstWeaponHand")));
}

void APlayableBaseCharacter::WeaponUnEquip()
{
	FirstWeaponComponent->AttachToComponent(BodyComponent,
		FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true),
		FName(TEXT("FirstWeapon")));
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
		//GetController()->SetIgnoreMoveInput(true);
		PlayMontageFullBody(TargetMontage);
	}
}

void APlayableBaseCharacter::InitializeStatus()
{
	//StatusComponent->InitState(NewStat);
}

void APlayableBaseCharacter::StopMontage(TObjectPtr<UAnimMontage> Montage)
{
	/*if(Montage == nullptr)
		return;*/
	BodyComponent->GetAnimInstance()->Montage_Stop(0.0f, Montage);
	HeadComponent->GetAnimInstance()->Montage_Stop(0.0f, Montage);
	HairComponent->GetAnimInstance()->Montage_Stop(0.0f, Montage);
	ArmComponent->GetAnimInstance()->Montage_Stop(0.0f, Montage);
	LegComponent->GetAnimInstance()->Montage_Stop(0.0f, Montage);
	FootComponent->GetAnimInstance()->Montage_Stop(0.0f, Montage);
}

void APlayableBaseCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();


	HeadComponent->SetMasterPoseComponent(GetMesh());
	HairComponent->SetMasterPoseComponent(GetMesh());
	ArmComponent->SetMasterPoseComponent(GetMesh());
	LegComponent->SetMasterPoseComponent(GetMesh());
	FootComponent->SetMasterPoseComponent(GetMesh());

	//GetMesh()->GetAnimInstance()->OnMontageStarted.AddDynamic(this, &APlayableBaseCharacter::AttackMontageStarted);
	//GetMesh()->GetAnimInstance()->OnMontageEnded.AddDynamic(this, &APlayableBaseCharacter::AttackMontageEnded);
	GetMesh()->GetAnimInstance()->OnMontageEnded.AddDynamic(this, &APlayableBaseCharacter::OnMontageEndedGeneral);
	GetMesh()->GetAnimInstance()->OnMontageEnded.AddDynamic(this, &APlayableBaseCharacter::EquipMontageEnded);
	GetMesh()->GetAnimInstance()->OnMontageEnded.AddDynamic(this, &APlayableBaseCharacter::UnEquipMontageEnded);

}

void APlayableBaseCharacter::AttackMontageStarted(UAnimMontage* Montage)
{
	//공격 콤보 bool false 초기화
	if(nullptr == Montage)
		return;
	GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Red, TEXT("Started : ") + Montage->GetName());

	if (Montage == CurSwordStanceComponent->GetNormalAttackMontage() ||
		Montage == CurSwordStanceComponent->GetHeavyAttackMontage())
	{
		CurSwordStanceComponent->ResetIsPossibleNextAttack();
	}
}

void APlayableBaseCharacter::AttackMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	//공격 콤보 bool true 확인 후 다음 몽타주 재생
	if (nullptr == Montage)
		return;
	if (bInterrupted)
		return;
	CurSwordStanceComponent->ResetAttackInfo();
	GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Blue, TEXT("Ended : ") + Montage->GetName());
}

void APlayableBaseCharacter::ResetCounterAttackTimer()
{
	GetWorld()->GetTimerManager().ClearTimer(GuardCounterAttackTimerHale);
	IsCanGuardConuterAttack = false;
}

void APlayableBaseCharacter::PCTakeDamage(int Damage)
{
	//Test Code
	//반격 기능
	//체력 감소 기능 추가 예정
	IsCanGuardConuterAttack = true;
	GetWorld()->GetTimerManager().ClearTimer(GuardCounterAttackTimerHale);
	GetWorld()->GetTimerManager().SetTimer(
		GuardCounterAttackTimerHale,
		this,
		&APlayableBaseCharacter::DisableCounterAttack,
		1.5f,
		false);
}

void APlayableBaseCharacter::OnPerfectDodgeSuccess(AActor* Attacker)
{
	bIsPerfectDodgeWindow = false;
	bIsWaitingForCounterInput = true;

	float DelayTime = 0.05f;
	SetIsActionLock(true);
	UGameplayStatics::SetGlobalTimeDilation(GetWorld(), DelayTime);

	//응격 UI 추가 예정
	//ShowCounterAttackUI();

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
	//CounterAttackUI->SetVisibility(ESlateVisibility::Hidden);
	CounterAttackWidgetComponent->SetVisibility(false);
	GetWorldTimerManager().ClearTimer(CounterInputTimerHandle);
	UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 1.0f);
	//응격 UI 제거 예정
	//HideCounterAttackUI();
	//카운터 공격 몽타주 실행
}

void APlayableBaseCharacter::AttackTrace()
{
	TArray<FHitResult> HitResults;
	bool isHit = UKismetSystemLibrary::BoxTraceMulti(
		this,
		GetActorLocation(), // 박스의 시작 위치
		GetActorLocation() + GetActorForwardVector() * 100.0f, // 박스의 끝 위치
		FVector(50.0f, 50.0f, 50.0f), // 박스의 반지름 (X, Y, Z)
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
		int Damage = StatusComponent->GetTotalAttackPower() + CurSwordStanceComponent->GetSpeicalAttackPower();
		
		//크리티컬 및 강공격 사용 여부 확인 bool 함수 작성 예정
		if (CurSwordStanceComponent->GetIsPlayHeavyAttackMontage())
			Damage *= 1.3f;

		GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Red, FString::Printf(TEXT("Damage : %d"), Damage));
		for(const FHitResult& HitResult : HitResults)
		{
			ABaseMonster* Monster = Cast<ABaseMonster>(HitResult.GetActor());
			if(Monster != nullptr)
			{
				Monster->HitBy(Damage);
			}
		}
	}
}

void APlayableBaseCharacter::OnMontageEndedGeneral(UAnimMontage* Montage, bool bInterrupted)
{
	
	//if (bInterrupted)
	//{
	//	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("Montage Interrupted222"));
	//	return;
	//}

	//EquipMontageEnded, UnEquipMontageEnded에서 처리하므로 패스
	if (Montage == UnEquipMontage || Montage == EquipMontage)
		return;

	/*if (GetCharacterMovement()->IsFalling() == true)
		return;*/

	ProcessMontageEndedGeneral(Montage, bInterrupted);
}

void APlayableBaseCharacter::ProcessMontageEndedGeneral(UAnimMontage* Montage, bool bInterrupted)
{
	if (bInterrupted)
		return;

	if(bInterrupted == false && GetCharacterMovement()->IsFalling() == false)
	{
		SetIsActionLock(false);
	}

	if (bInterrupted == false)
	{
		if (Montage == CurSwordStanceComponent->GetHeavyAttackMontage() || Montage == CurSwordStanceComponent->GetNormalAttackMontage())
			CurSwordStanceComponent->ResetAttackInfo();
	}

	//Test Code
	if(Montage == EvadeMontage)
	{
		OnPerfectDodgeSuccess(nullptr);
		//CounterAttackWidget->SetVisibility(ESlateVisibility::Visible);
		CounterAttackWidgetComponent->SetVisibility(true);
	}
	//GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Blue, TEXT("ProcessMontageEndedGeneral"));
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

	//if (APlayerController* PC = Cast<APlayerController>(GetController()))
	//{
	//	// 1. 인풋 서브시스템 가져오기
	//	if (auto* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PC->GetLocalPlayer()))
	//	{
	//		AMiyamotoIoriController* IoriController = Cast<AMiyamotoIoriController>(PC);
	//		UInputAction* Test = IoriController->GetNormalAttackAction();
	//		// 2. IA_CounterAttack 액션에 매핑된 모든 키 리스트 가져오기
	//		TArray<FKey> MappedKeys = Subsystem->QueryKeysMappedToAction(Test);

	//		// 3. 매핑된 키가 있다면 첫 번째 키를 반환 (보통 0번이 주 입력키)
	//		if (MappedKeys.Num() > 0)
	//		{
	//			return MappedKeys[0];
	//		}
	//	}
	//}

	//return EKeys::Invalid;
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
	//0.1초 딜레이 코드
	/*if (CounterAttackWidget)
	{
		CounterAttackUI = CreateWidget<UCounterAttackUI>(GetWorld(), CounterAttackWidget);
		if (CounterAttackUI)
		{
			CounterAttackUI->AddToViewport();
			CounterAttackUI->SetVisibility(ESlateVisibility::Visible);

			// NextTick 대신 명시적으로 0.1초의 시간을 줍니다.
			FTimerHandle TempHandle;
			GetWorldTimerManager().SetTimer(TempHandle, [this]()
				{
					if (CounterAttackUI)
					{
						FKey Key = GetCounterAttackInputKey();
						// ... (이하 동일)
						CounterAttackUI->UpdateKeyIcon(Key);
					}
				}, 0.1f, false); // 0.1초 뒤에 실행
		}
	}*/
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