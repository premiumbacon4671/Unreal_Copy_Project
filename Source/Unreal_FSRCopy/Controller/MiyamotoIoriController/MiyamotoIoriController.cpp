// Fill out your copyright notice in the Description page of Project Settings.


#include "Controller/MiyamotoIoriController/MiyamotoIoriController.h"
#include "InputAction.h"
#include "InputMappingContext.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "InputActionValue.h"
#include "PlayableCharacter/PlayableBaseCharacter.h"

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

void AMiyamotoIoriController::SetupInputComonent()
{
	Super::SetupInputComponent();
	UEnhancedInputComponent* input = Cast<UEnhancedInputComponent>(InputComponent);
	if (nullptr != input)
	{
		input->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AMiyamotoIoriController::MoveInput);
		input->BindAction(MoveAction, ETriggerEvent::Completed, this, &AMiyamotoIoriController::MoveEndInput);
		input->BindAction(RotationAction, ETriggerEvent::Completed, this, &AMiyamotoIoriController::LookInput);
	}
}

void AMiyamotoIoriController::MoveInput(const FInputActionValue& value)
{
	isMoveInput = true;
	FVector2D MoveValue = value.Get<FVector2D>();
	FVector Forward = GetTransformComponent()->GetForwardVector();
	Forward.Z = 0.0f;
	Forward.Normalize();
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
	AddYawInput(MoveValue.X);
	AddPitchInput(MoveValue.Y);
}
