// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayableCharacter/Miyamoto_Iori/Miyamoto_Iori.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Engine/DataTable.h"


#include "PlayableCharacter/Miyamoto_Iori/ActorComponent/EarthStanceActorComponent.h"
#include "PlayableCharacter/Miyamoto_Iori/ActorComponent/FireStanceActorComponent.h"
#include "ActorComponent/StateComponent/Miyamoto_ioriStateComponent.h"

AMiyamoto_Iori::AMiyamoto_Iori()
{
	PrimaryActorTick.bCanEverTick = true;
	eCurSwordStance = ESWORDSTANCE::EST_FIRE;

#pragma region CreateComponent
	//SwordStanceComponents[static_cast<int32>(CurSwordStance)] = CreateDefaultSubobject<UEarthStanceActorComponent>(TEXT("EarthStance"));

	SwordStanceComponents.SetNum(static_cast<int>(ESWORDSTANCE::EST_MAX));
	UEarthStanceActorComponent* EarthStance = CreateDefaultSubobject<UEarthStanceActorComponent>(TEXT("EarthStanceComponent"));
	EarthStance->SetIsUnlockSwordStance(true);
	SwordStanceComponents[static_cast<int>(ESWORDSTANCE::EST_EARTH)] = EarthStance;
	UFireStanceActorComponent* FireStance = CreateDefaultSubobject<UFireStanceActorComponent>(TEXT("FireStanceComponent"));
	SwordStanceComponents[static_cast<int>(ESWORDSTANCE::EST_FIRE)] = FireStance;
	FireStance->SetIsUnlockSwordStance(true);

	UBaseSwordStanceActorComponent* a1Stance = CreateDefaultSubobject<UBaseSwordStanceActorComponent>(TEXT("a1StanceComponent"));
	SwordStanceComponents[1] = a1Stance;
	UBaseSwordStanceActorComponent* a2Stance = CreateDefaultSubobject<UBaseSwordStanceActorComponent>(TEXT("a2StanceComponent"));
	SwordStanceComponents[3] = a2Stance;
	UBaseSwordStanceActorComponent* a3Stance = CreateDefaultSubobject<UBaseSwordStanceActorComponent>(TEXT("a3StanceComponent"));
	SwordStanceComponents[4] = a3Stance;

	StatusComponent = CreateDefaultSubobject<UMiyamoto_IoriStateComponent>(TEXT("Miyamoto_IoriStatusComponent"));

	/*IsUnlockedSwordStance.Add(ESWORDSTANCE::EST_EARTH, true);
	IsUnlockedSwordStance.Add(ESWORDSTANCE::EST_WATER, false);
	IsUnlockedSwordStance.Add(ESWORDSTANCE::EST_FIRE, true);
	IsUnlockedSwordStance.Add(ESWORDSTANCE::EST_WIND, false);
	IsUnlockedSwordStance.Add(ESWORDSTANCE::EST_VOID, false);*/
#pragma endregion
	static ConstructorHelpers::FObjectFinder<UDataTable> MiyamotoDataTable(TEXT("/Script/Engine.DataTable'/Game/Blueprint/PlayableCharacter/Data/DT_MiyamotoIoriStat.DT_MiyamotoIoriStat'"));
	if (MiyamotoDataTable.Succeeded())
		PlayableDataTable = MiyamotoDataTable.Object;

	/*switch (CurSwordStance)
	{
	case ESWORDSTANCE::EST_EARTH:
		CurSwordStanceComponent = SwordStanceComponents[0];
		break;
	case ESWORDSTANCE::EST_FIRE:
		CurSwordStanceComponent = SwordStanceComponents[1];
		break;
	default:
		break;
	}*/
	//CurSwordStanceComponent = SwordStanceComponents[static_cast<int>(CurSwordStance)];
}

void AMiyamoto_Iori::BeginPlay()
{
	Super::BeginPlay();
	CurSwordStanceComponent = SwordStanceComponents[static_cast<int>(eCurSwordStance)];
	CurSwordStanceComponent->SetIsUseStance(true);
	eNextSwordStance = eCurSwordStance;
	//stat 초기화 예정
	InitializeStatus();
}

void AMiyamoto_Iori::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	//NextMontage 실험 코드
	/*if (isCombatMode == true && NextMontage != nullptr)
	{
		if (GetMesh()->GetAnimInstance()->GetCurrentActiveMontage() == nullptr &&
			GetMovementComponent()->IsFalling() == false)
		{
			PlayMontageFullBody(NextMontage);
			NextMontage = nullptr;
		}
	}*/
}


void AMiyamoto_Iori::InitializeStatus()
{
	//세이브 파일이 없는 관계로 레벨1로 고정, 추후 세이브 파일이 생기면 레벨에 맞는 스탯으로 초기화
	FName StatDataName = *FString::Printf(TEXT("Level%d"), 1);
	FMiyamoto_IoriStat* IoriStat = PlayableDataTable->FindRow<FMiyamoto_IoriStat>(StatDataName, TEXT("MiyamotoIoriDataTable"));
	if (nullptr != IoriStat)
		StatusComponent->InitState(*IoriStat);
}

void AMiyamoto_Iori::PlayEquipWeaponMontage()
{
	//검을 뽑는 애니메이션 재생
	//if (GetMovementComponent()->IsFalling() == true ||
	if(nullptr == EquipMontage ||
		BodyComponent->GetAnimInstance()->Montage_IsPlaying(EquipMontage) == true ||
		BodyComponent->GetAnimInstance()->Montage_IsPlaying(CurSwordStanceComponent->GetNormalAttackMontage()) == true)
		return;
	FName SectionName;
	UAnimMontage* montage = nullptr;
	switch (eCurSwordStance)
	{
	//형태 별로 다른 몽타주를 재생시켜야 하지만 지금은 동일 몽타주 사용
	//one hand sword equip
	case ESWORDSTANCE::EST_EARTH:
		montage = EquipMontage;
		break;
	//two hand sword equip
	case ESWORDSTANCE::EST_FIRE:
		montage = EquipMontage;
		break;
	}

	if (montage != nullptr)
	{
		//칼 넣는 애니 여기서 작동 안함
		//칼을 넣을 때 이동 함
		SetIsActionLock(true);
		//GetController()->SetIgnoreMoveInput(true);
		PlayMontageFullBody(montage);
	}
}

void AMiyamoto_Iori::PlayUnEquipWeaponMontage()
{
	//검을 넣는 애니메이션 재생
	if (nullptr == UnEquipMontage ||
		BodyComponent->GetAnimInstance()->Montage_IsPlaying(EquipMontage) == true ||
		BodyComponent->GetAnimInstance()->Montage_IsPlaying(CurSwordStanceComponent->GetNormalAttackMontage()) == true)
		return;
	FName SectionName;
	UAnimMontage* montage = nullptr;
	switch (eCurSwordStance)
	{
		//형태 별로 다른 몽타주를 재생시켜야 하지만 지금은 동일 몽타주 사용
		//one hand sword equip
	case ESWORDSTANCE::EST_EARTH:
		montage = UnEquipMontage;
		break;
		//two hand sword equip
	case ESWORDSTANCE::EST_FIRE:
		montage = UnEquipMontage;
		break;
	}

	if (montage != nullptr)
	{
		//칼 넣는 애니 여기서 작동 안함
		//칼을 넣을 때 이동 함
		SetIsActionLock(true);
		//GetController()->SetIgnoreMoveInput(true);
		PlayMontageFullBody(montage);
	}
}

void AMiyamoto_Iori::PlayEquipWeaponStateMontage_New(bool bIsEquip)
{
	if (BodyComponent->GetAnimInstance()->Montage_IsPlaying(CurSwordStanceComponent->GetNormalAttackMontage()) == true ||
		BodyComponent->GetAnimInstance()->Montage_IsPlaying(CurSwordStanceComponent->GetHeavyAttackMontage()) == true ||
		BodyComponent->GetAnimInstance()->Montage_IsPlaying(EquipMontage) == true ||
		BodyComponent->GetAnimInstance()->Montage_IsPlaying(UnEquipMontage) == true)
		return;
	TObjectPtr<UAnimMontage> TargetMontage = nullptr;

	switch (eCurSwordStance)
	{
		//형태 별로 다른 몽타주를 재생시켜야 하지만 지금은 동일 몽타주 사용
		case ESWORDSTANCE::EST_EARTH:
			TargetMontage = bIsEquip ? EquipMontage : UnEquipMontage;
			break;
		case ESWORDSTANCE::EST_FIRE:
			TargetMontage = bIsEquip ? EquipMontage : UnEquipMontage;
			break;
	}


	if (TargetMontage != nullptr)
	{
		SetIsActionLock(true);
		//GetController()->SetIgnoreMoveInput(true);
		PlayMontageFullBody(TargetMontage);
	}
}

void AMiyamoto_Iori::WeaponEquip()
{
	Super::WeaponEquip();
	switch (eCurSwordStance)
	{
	case ESWORDSTANCE::EST_EARTH:
		break;
	case ESWORDSTANCE::EST_FIRE:
		SecondWeaponComponent->AttachToComponent(BodyComponent,
		FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true),
		FName(TEXT("SecondWeaponHand")));
		break;
	default:
		break;
	}
}

void AMiyamoto_Iori::WeaponUnEquip()
{
	Super::WeaponUnEquip();
	switch (eCurSwordStance)
	{
	case ESWORDSTANCE::EST_EARTH:
		break;
	case ESWORDSTANCE::EST_FIRE:
		SecondWeaponComponent->AttachToComponent(BodyComponent,
			FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true),
			FName(TEXT("SecondWeapon")));
		break;
	default:
		break;
	}
}

bool AMiyamoto_Iori::GetIsUnlockSwordStance(ESWORDSTANCE SwordStance) const
{
	return SwordStanceComponents[static_cast<int>(SwordStance)]->GetIsUnlockSwordStance();
}

void AMiyamoto_Iori::ChangeSwordStance(ESWORDSTANCE SwordStance)
{
	if(eCurSwordStance == SwordStance)
		return;

	//end 델리게이트 사용
	//전투 상태일 시 상태전환
	if (isCombatMode == true)
	{
		eNextSwordStance = SwordStance;

		//NextMontage 실험 코드
		//NextMontage = UnEquipMontage;

		TObjectPtr<UAnimInstance> AnimInstance = BodyComponent->GetAnimInstance();

		if (AnimInstance && AnimInstance->IsAnyMontagePlaying())
			return;
		if(GetMovementComponent()->IsFalling() == true)
			return;

		PlayUnEquipWeaponMontage_New();
	}
	//비전투 상태일 시 검의 형만 변경
	else
	{
		eCurSwordStance = SwordStance;
		eNextSwordStance = eCurSwordStance;
		CurSwordStanceComponent = SwordStanceComponents[static_cast<int>(eCurSwordStance)];
	}
	//NextMontageSectionName = TEXT("OneHandSwordUnEquip");
}

void AMiyamoto_Iori::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	//GetMesh()->GetAnimInstance()->OnMontageStarted.AddDynamic(this, &AMiyamoto_Iori::UnEquipMontageStarted);
	//GetMesh()->GetAnimInstance()->OnMontageEnded.AddDynamic(this, &AMiyamoto_Iori::UnEquipMontageEnded);
	//GetMesh()->GetAnimInstance()->OnMontageBlendingOut.AddDynamic(this, &AMiyamoto_Iori::UnEquipMontageEnded);
	//GetMesh()->GetAnimInstance()->OnMontageEnded.AddDynamic(this, &AMiyamoto_Iori::OnMontageEndedGeneral);
	//GetMesh()->GetAnimInstance()->OnMontageEnded.AddDynamic(this, &AMiyamoto_Iori::EquipMontageEnded);
}

void AMiyamoto_Iori::UnEquipMontageStarted(UAnimMontage* Montage)
{
	if (Montage == nullptr || Montage != EquipMontage)
		return;
	//NextMontageSectionName = GetMesh()->GetAnimInstance()->Montage_GetCurrentSection()
}


void AMiyamoto_Iori::UnEquipMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	if (Montage == nullptr || Montage != UnEquipMontage)
		return;

	if(bInterrupted)
		return;

	SetIsActionLock(false);
	//검 형태 변경
	if (eCurSwordStance != eNextSwordStance)
	{
		CurSwordStanceComponent->ReleaseSwordStance();
		eCurSwordStance = eNextSwordStance;
		CurSwordStanceComponent = SwordStanceComponents[static_cast<int>(eCurSwordStance)];
		CurSwordStanceComponent->InitSwordStance();
		//PlayEquipWeaponMontage();
		//TestCode
		PlayEquipWeaponMontage_New();
	}
}

void AMiyamoto_Iori::EquipMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	if(Montage != EquipMontage)
		return;

	if (bInterrupted)
	{
		bRetryEquip = true;
	}
	else
	{
		SetIsActionLock(false);
		bRetryEquip = false;
	}
}

void AMiyamoto_Iori::ProcessMontageEndedGeneral(UAnimMontage* Montage, bool bInterrupted)
{
	Super::ProcessMontageEndedGeneral(Montage, bInterrupted);

	if (bInterrupted)
	{
		return;
	}

	if(GetCharacterMovement()->IsFalling())
		return;

	if (eCurSwordStance != eNextSwordStance)
	{
		PlayUnEquipWeaponMontage_New();
	}
	else if (bRetryEquip)
	{
		PlayEquipWeaponMontage_New();
		bRetryEquip = false;
	}
	GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Blue, TEXT("ProcessMontageEndedGeneral"));
}

void AMiyamoto_Iori::Landed(const FHitResult& Hit)
{
	Super::Landed(Hit); 
	if (eCurSwordStance != eNextSwordStance)
	{
		PlayUnEquipWeaponMontage_New();
	}
	else if (bRetryEquip)
	{
		PlayEquipWeaponMontage_New();
		bRetryEquip = false;
	}
}
