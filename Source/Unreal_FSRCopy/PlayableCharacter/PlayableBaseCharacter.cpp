// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayableCharacter/PlayableBaseCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "PlayableCharacter/Miyamoto_Iori/ActorComponent/BaseSwordStanceActorComponent.h"

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

	static ConstructorHelpers::FObjectFinder<UAnimMontage> EvadeMontageFinder(
		TEXT("/Script/Engine.AnimMontage'/Game/Blueprint/PlayableCharacter/Animation/AM_Evade.AM_Evade'"));
	if (EvadeMontageFinder.Succeeded())
		EvadeMontage = EvadeMontageFinder.Object;
#pragma endregion

	SpringArm->bUsePawnControlRotation = true;
	bUseControllerRotationYaw = false;
	GetCharacterMovement()->bOrientRotationToMovement = true;
}

// Called when the game starts or when spawned
void APlayableBaseCharacter::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void APlayableBaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if(isSprint && GetVelocity().Size2D() < WalkSpeed)
	{
		SetWalk();
	}
}

// Called to bind functionality to input
void APlayableBaseCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

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
	if (nullptr != EvadeMontage &&
		BodyComponent->GetAnimInstance()->Montage_IsPlaying(EvadeMontage) == false &&
		CurSwordStanceComponent->IsAttacking() == false)
		PlayMontageFullBody(EvadeMontage);
}

void APlayableBaseCharacter::SetBrakingDecelerationFalling()
{
	GetCharacterMovement()->BrakingDecelerationFalling = GetCharacterMovement()->MaxWalkSpeed;
}

bool APlayableBaseCharacter::PlayJumpMontage()
{
	if (GetMovementComponent()->IsFalling() == false &&
		nullptr != JumpMontage &&
		BodyComponent->GetAnimInstance()->Montage_IsPlaying(JumpMontage) == false)
	{
		PlayMontageFullBody(JumpMontage);
		return true;
	}
	return false;
}

void APlayableBaseCharacter::PlayJump()
{
	if(PlayJumpMontage())
	{
		SetBrakingDecelerationFalling();
	}
}

bool APlayableBaseCharacter::PlayMontageFullBody(TObjectPtr<UAnimMontage> Montage, FName SectionName)
{
	if(Montage == nullptr)
		return false;
	BodyComponent->GetAnimInstance()->Montage_Play(Montage);
	HeadComponent->GetAnimInstance()->Montage_Play(Montage);
	HairComponent->GetAnimInstance()->Montage_Play(Montage);
	ArmComponent->GetAnimInstance()->Montage_Play(Montage);
	LegComponent->GetAnimInstance()->Montage_Play(Montage);
	FootComponent->GetAnimInstance()->Montage_Play(Montage);

	if(SectionName.IsNone() == false)
	{
		BodyComponent->GetAnimInstance()->Montage_JumpToSection(SectionName, Montage);
		HeadComponent->GetAnimInstance()->Montage_JumpToSection(SectionName, Montage);
		HairComponent->GetAnimInstance()->Montage_JumpToSection(SectionName, Montage);
		ArmComponent->GetAnimInstance()->Montage_JumpToSection(SectionName, Montage);
		LegComponent->GetAnimInstance()->Montage_JumpToSection(SectionName, Montage);
		FootComponent->GetAnimInstance()->Montage_JumpToSection(SectionName, Montage);
		
	}
	return true;
}

void APlayableBaseCharacter::SetCombatMode()
{
	//Test Code
	//isCombatMode = true;
	isCombatMode = !isCombatMode;
	GetController()->SetIgnoreMoveInput(true);
	PlayEquipWeaponMontage();
	FString ModeText = isCombatMode ? TEXT("true") : TEXT("false");
	GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Blue, TEXT("CombatMode : ") + ModeText);
}

void APlayableBaseCharacter::PlayEquipWeaponMontage()
{
	if (GetMovementComponent()->IsFalling() == true ||
		nullptr == EquipMontage ||
		BodyComponent->GetAnimInstance()->Montage_IsPlaying(EquipMontage) == true ||
		BodyComponent->GetAnimInstance()->Montage_IsPlaying(CurSwordStanceComponent->GetNormalAttackMontage()) == true ||
		BodyComponent->GetAnimInstance()->Montage_IsPlaying(CurSwordStanceComponent->GetHeavyAttackMontage()) == true)
		return;

	if(isCombatMode == false)
		PlayMontageFullBody(EquipMontage, TEXT("UnEquip"));
	else
		PlayMontageFullBody(EquipMontage, TEXT("equip"));
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

void APlayableBaseCharacter::StopMontage(TObjectPtr<UAnimMontage> Montage)
{
	if(Montage == nullptr)
		return;
	BodyComponent->GetAnimInstance()->Montage_Stop(0.2, Montage);
	HeadComponent->GetAnimInstance()->Montage_Stop(0.2, Montage);
	HairComponent->GetAnimInstance()->Montage_Stop(0.2, Montage);
	ArmComponent->GetAnimInstance()->Montage_Stop(0.2, Montage);
	LegComponent->GetAnimInstance()->Montage_Stop(0.2, Montage);
	FootComponent->GetAnimInstance()->Montage_Stop(0.2, Montage);
}

void APlayableBaseCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	GetMesh()->GetAnimInstance()->OnMontageStarted.AddDynamic(this, &APlayableBaseCharacter::AttackMontageStarted);
	GetMesh()->GetAnimInstance()->OnMontageEnded.AddDynamic(this, &APlayableBaseCharacter::AttackMontageEnded);

}

void APlayableBaseCharacter::AttackMontageStarted(UAnimMontage* Montage)
{
	//공격 콤보 bool false 초기화
	if(nullptr == Montage)
		return;
	if (Montage == CurSwordStanceComponent->GetNormalAttackMontage() ||
		Montage == CurSwordStanceComponent->GetHeavyAttackMontage())
	{
		CurSwordStanceComponent->ResetIsPossibleNextAttack();
		if (Montage == CurSwordStanceComponent->GetNormalAttackMontage())
			CurSwordStanceComponent->SetIsPlayingNormalAttack();
		else if (Montage == CurSwordStanceComponent->GetHeavyAttackMontage())
		{
			GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Red, TEXT("Started") + Montage->GetName() + CurSwordStanceComponent->GetCurHeavyAttackSectionName().ToString());
			CurSwordStanceComponent->SetIsPlayingHeavyAttack();
		}
		else
			return;
	}
}

void APlayableBaseCharacter::AttackMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	//공격 콤보 bool true 확인 후 다음 몽타주 재생
	if (nullptr == Montage)
		return;
	if (Montage == CurSwordStanceComponent->GetNormalAttackMontage() ||
		Montage == CurSwordStanceComponent->GetHeavyAttackMontage())
	{
		GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Blue, TEXT("Ended") + Montage->GetName() + CurSwordStanceComponent->GetCurHeavyAttackSectionName().ToString());
		if (Montage == CurSwordStanceComponent->GetNormalAttackMontage())
			CurSwordStanceComponent->ResetIsPlayingNormalAttack();
		else if (Montage == CurSwordStanceComponent->GetHeavyAttackMontage())
			CurSwordStanceComponent->ResetIsPlayingHeavyAttack();
		else
			return;

		if(CurSwordStanceComponent->GetIsPossibleNextAttack() == true)
			CurSwordStanceComponent->PlayNextAttackMontage();
		else
			CurSwordStanceComponent->ResetAttackInfo();
	}
}
