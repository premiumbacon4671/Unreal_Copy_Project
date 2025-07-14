// Fill out your copyright notice in the Description page of Project Settings.


#include "Controller/MiyamotoIoriController/MiyamotoIoriController.h"
#include "InputAction.h"
#include "InputMappingContext.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "InputActionValue.h"
#include "PlayableCharacter/PlayableBaseCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"

AMiyamotoIoriController::AMiyamotoIoriController()
{
	PrimaryActorTick.bCanEverTick = true;
	static ConstructorHelpers::FObjectFinder<UInputAction> MoveActionFinder(
		TEXT("/Script/EnhancedInput.InputAction'/Game/Blueprint/PlayableCharacter/Input/IA_PCMove.IA_PCMove'"));
	if (MoveActionFinder.Succeeded())
		MoveAction = MoveActionFinder.Object;
	static ConstructorHelpers::FObjectFinder<UInputAction> RotationActionFinder(
		TEXT("/Script/EnhancedInput.InputAction'/Game/Blueprint/PlayableCharacter/Input/IA_PCRotation.IA_PCRotation'"));
	if (RotationActionFinder.Succeeded())
		RotationAction = RotationActionFinder.Object;

	static ConstructorHelpers::FObjectFinder<UInputAction> SprintActionFinder(
		TEXT("/Script/EnhancedInput.InputAction'/Game/Blueprint/PlayableCharacter/Input/IA_PCSprint.IA_PCSprint'"));
	if (SprintActionFinder.Succeeded())
		SprintAction = SprintActionFinder.Object;

	static ConstructorHelpers::FObjectFinder<UInputAction> JumpActionFinder(
		TEXT("/Script/EnhancedInput.InputAction'/Game/Blueprint/PlayableCharacter/Input/IA_PCJump.IA_PCJump'"));
	if (JumpActionFinder.Succeeded())
		JumpAction = JumpActionFinder.Object;

	static ConstructorHelpers::FObjectFinder<UInputMappingContext> InputMappingContextFinder(
		TEXT("/Script/EnhancedInput.InputMappingContext'/Game/Blueprint/PlayableCharacter/Input/IMC_PlayableCharacter.IMC_PlayableCharacter'"));
	if (InputMappingContextFinder.Succeeded())
		MappingContext = InputMappingContextFinder.Object;
}

void AMiyamotoIoriController::BeginPlay()
{
	Super::BeginPlay();
	UEnhancedInputLocalPlayerSubsystem* InputSystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
	if (InputSystem != nullptr)
		InputSystem->AddMappingContext(MappingContext, 0);
	MiyamotoIori = Cast<APlayableBaseCharacter>(GetCharacter());
	CurPlayableCharacter = MiyamotoIori;
}

void AMiyamotoIoriController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AMiyamotoIoriController::SetupInputComponent()
{
	Super::SetupInputComponent();
	UEnhancedInputComponent* input = Cast<UEnhancedInputComponent>(InputComponent);
	if (nullptr != input)
	{
		input->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AMiyamotoIoriController::MoveInput);
		input->BindAction(MoveAction, ETriggerEvent::Completed, this, &AMiyamotoIoriController::MoveEndInput);
		input->BindAction(RotationAction, ETriggerEvent::Triggered, this, &AMiyamotoIoriController::LookInput);
		input->BindAction(SprintAction, ETriggerEvent::Started, this, &AMiyamotoIoriController::SprintInput);
		input->BindAction(JumpAction, ETriggerEvent::Started, this, &AMiyamotoIoriController::JumpInput);
	}
}

void AMiyamotoIoriController::MoveInput(const FInputActionValue& value)
{
	isMoveInput = true;
	FVector2D MoveValue = value.Get<FVector2D>();
	FVector Forward = GetTransformComponent()->GetForwardVector();
	Forward.Z = 0.0f;
	Forward.Normalize();
	//Gamepad Deadzone
	if (FMath::Abs(MoveValue.X) <= 0.2f)
		MoveValue.X = 0.0f;
	if (FMath::Abs(MoveValue.Y) <= 0.2f)
		MoveValue.Y = 0.0f;

	CurPlayableCharacter->AddMovementInput(Forward, MoveValue.X);
	CurPlayableCharacter->AddMovementInput(GetTransformComponent()->GetRightVector(), MoveValue.Y);
}

void AMiyamotoIoriController::MoveEndInput(const FInputActionValue& value)
{
	isMoveInput = false;
}

void AMiyamotoIoriController::LookInput(const FInputActionValue& value)
{
	FVector2D MoveValue = value.Get<FVector2D>(); 
	//Gamepad Deadzone
	if (FMath::Abs(MoveValue.X) <= 0.2f)
		MoveValue.X = 0.0f;
	if (FMath::Abs(MoveValue.Y) <= 0.2f)
		MoveValue.Y = 0.0f;
	AddYawInput(MoveValue.X * CAMERA_SPIN_SPEED * GetWorld()->DeltaTimeSeconds);
	AddPitchInput(MoveValue.Y * CAMERA_SPIN_SPEED * GetWorld()->DeltaTimeSeconds);
}

void AMiyamotoIoriController::SprintInput(const FInputActionValue& value)
{
	if(isCombat)
		return;
	CurPlayableCharacter->SetMoveSpeed();
}

void AMiyamotoIoriController::JumpInput(const FInputActionValue& value)
{
	CurPlayableCharacter->Jump();
	CurPlayableCharacter->PlayJump();
}
