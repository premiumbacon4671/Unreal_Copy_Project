// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayableCharacter/PlayableBaseCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

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
#pragma endregion

#pragma region Montage
	static ConstructorHelpers::FObjectFinder<UAnimMontage> JumpMontageFinder(
		TEXT("/Script/Engine.AnimMontage'/Game/Blueprint/PlayableCharacter/MiyamotoIori/Animation/AM_Jump.AM_Jump'"));
	if (JumpMontageFinder.Succeeded())
		JumpMontage = JumpMontageFinder.Object;
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

void APlayableBaseCharacter::PlayJumpMontage()
{
	if (GetMovementComponent()->IsFalling() == false &&
		nullptr != JumpMontage &&
		BodyComponent->GetAnimInstance()->Montage_IsPlaying(JumpMontage) == false)
	{
		PlayMontageFullBody(JumpMontage);
	}
}

void APlayableBaseCharacter::PlayMontageFullBody(TObjectPtr<UAnimMontage> Montage)
{
	if(Montage == nullptr)
		return;
	BodyComponent->GetAnimInstance()->Montage_Play(Montage);
	HeadComponent->GetAnimInstance()->Montage_Play(Montage);
	HairComponent->GetAnimInstance()->Montage_Play(Montage);
	ArmComponent->GetAnimInstance()->Montage_Play(Montage);
	LegComponent->GetAnimInstance()->Montage_Play(Montage);
	FootComponent->GetAnimInstance()->Montage_Play(Montage);
}
