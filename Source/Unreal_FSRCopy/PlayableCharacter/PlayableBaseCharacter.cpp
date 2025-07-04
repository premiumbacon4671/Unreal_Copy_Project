// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayableCharacter/PlayableBaseCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"

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

}

// Called to bind functionality to input
void APlayableBaseCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

