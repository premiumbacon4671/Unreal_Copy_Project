// Fill out your copyright notice in the Description page of Project Settings.


#include "Controller/MiyamotoIoriController/MiyamotoIoriController.h"
#include "InputAction.h"
#include "InputMappingContext.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "InputActionValue.h"
#include "PlayableCharacter/PlayableBaseCharacter.h"
#include "PlayableCharacter/Miyamoto_Iori/Miyamoto_Iori.h"
#include "HUD/PlayerHUD.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "PlayableCharacter/Miyamoto_Iori/ActorComponent/BaseSwordStanceActorComponent.h"
#include "Kismet/GameplayStatics.h"

#include "UI/SwordStanceUI.h"

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

	static ConstructorHelpers::FObjectFinder<UInputAction> EvadeActionFinder(
		TEXT("/Script/EnhancedInput.InputAction'/Game/Blueprint/PlayableCharacter/Input/IA_PCEvade.IA_PCEvade'"));
	if (EvadeActionFinder.Succeeded())
		EvadeAction = EvadeActionFinder.Object;

	static ConstructorHelpers::FObjectFinder<UInputAction> JumpActionFinder(
		TEXT("/Script/EnhancedInput.InputAction'/Game/Blueprint/PlayableCharacter/Input/IA_PCJump.IA_PCJump'"));
	if (JumpActionFinder.Succeeded())
		JumpAction = JumpActionFinder.Object;

	static ConstructorHelpers::FObjectFinder<UInputAction> NormalAttackActionFinder(
		TEXT("/Script/EnhancedInput.InputAction'/Game/Blueprint/PlayableCharacter/Input/IA_PCNormalAttack.IA_PCNormalAttack'"));
	if (NormalAttackActionFinder.Succeeded())
		NormalAttackAction = NormalAttackActionFinder.Object;

	static ConstructorHelpers::FObjectFinder<UInputAction> HeavyAttackActionFinder(
		TEXT("/Script/EnhancedInput.InputAction'/Game/Blueprint/PlayableCharacter/Input/IA_PCHeavyAttack.IA_PCHeavyAttack'"));
	if (HeavyAttackActionFinder.Succeeded())
		HeavyAttackAction = HeavyAttackActionFinder.Object;

	static ConstructorHelpers::FObjectFinder<UInputAction> ChangeStanceActionFinder(
		TEXT("/Script/EnhancedInput.InputAction'/Game/Blueprint/PlayableCharacter/Input/IA_ChangeStance.IA_ChangeStance'"));
	if (ChangeStanceActionFinder.Succeeded())
		ChangeStanceAction = ChangeStanceActionFinder.Object;

	static ConstructorHelpers::FObjectFinder<UInputAction> UIMoveActionFinder(
		TEXT("/Script/EnhancedInput.InputAction'/Game/Blueprint/PlayableCharacter/Input/IA_UIMove.IA_UIMove'"));
	if (UIMoveActionFinder.Succeeded())
		UIMoveAction = UIMoveActionFinder.Object;

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
	PlayerHUD = Cast<APlayerHUD>(GetHUD());
}

void AMiyamotoIoriController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	isCombat = CurPlayableCharacter->GetIsCombatMode();
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
		input->BindAction(EvadeAction, ETriggerEvent::Started, this, &AMiyamotoIoriController::EvadeInput);
		input->BindAction(JumpAction, ETriggerEvent::Started, this, &AMiyamotoIoriController::JumpInput);
		input->BindAction(NormalAttackAction, ETriggerEvent::Started, this, &AMiyamotoIoriController::NormalAttackInput);
		input->BindAction(HeavyAttackAction, ETriggerEvent::Started, this, &AMiyamotoIoriController::HeavyAttackInput);
		input->BindAction(HeavyAttackAction, ETriggerEvent::Triggered, this, &AMiyamotoIoriController::HeavyAttackTriggeredInput);
		input->BindAction(HeavyAttackAction, ETriggerEvent::Completed, this, &AMiyamotoIoriController::HeavyAttackCompletedInput);
		input->BindAction(ChangeStanceAction, ETriggerEvent::Started, this, &AMiyamotoIoriController::ChangeStanceInput);
		input->BindAction(ChangeStanceAction, ETriggerEvent::Completed, this, &AMiyamotoIoriController::ChangeStanceCompletedInput);
		input->BindAction(UIMoveAction, ETriggerEvent::Triggered, this, &AMiyamotoIoriController::UIMoveInput);
	}
}

void AMiyamotoIoriController::MoveInput(const FInputActionValue& value)
{
	if (isUIMode == true)
		return;
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

	FString test = FString::Printf(TEXT("X: %f, Y: %f"), MoveValue.X, MoveValue.Y);
	GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Green, test);
	CurPlayableCharacter->AddMovementInput(Forward, MoveValue.X);
	CurPlayableCharacter->AddMovementInput(GetTransformComponent()->GetRightVector(), MoveValue.Y);
}

void AMiyamotoIoriController::MoveEndInput(const FInputActionValue& value)
{
	if (isUIMode == true)
		return;
	isMoveInput = false;
}

void AMiyamotoIoriController::LookInput(const FInputActionValue& value)
{
	if (isUIMode == true)
		return;
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
	if (isUIMode == true)
		return;
	if(isCombat)
		return;
	CurPlayableCharacter->SetMoveSpeed();
}

void AMiyamotoIoriController::EvadeInput(const FInputActionValue& value)
{
	if (isUIMode == true)
		return;
	if (isCombat == false)
		return;
	CurPlayableCharacter->PlayEvade();
}

void AMiyamotoIoriController::JumpInput(const FInputActionValue& value)
{
	if (isUIMode == true)
		return;
	CurPlayableCharacter->Jump();
	CurPlayableCharacter->PlayJump();
}

void AMiyamotoIoriController::NormalAttackInput(const FInputActionValue& value)
{
	GEngine->AddOnScreenDebugMessage(1, 3.0f, FColor::Red, TEXT("Normal Attack Input"));
	CurPlayableCharacter->GetCurSwordStanceComponent()->PlayNormalAttackMontage();
}

void AMiyamotoIoriController::HeavyAttackInput(const FInputActionValue& value)
{
	GEngine->AddOnScreenDebugMessage(1, 3.0f, FColor::Red, TEXT("Heavy Attack Input"));
	CurPlayableCharacter->GetCurSwordStanceComponent()->PlayHeavyAttackMontage();
}

void AMiyamotoIoriController::HeavyAttackTriggeredInput(const FInputActionValue& value)
{
	GEngine->AddOnScreenDebugMessage(1, 3.0f, FColor::Red, TEXT("Triggered Heavy Attack Input"));
	CurPlayableCharacter->GetCurSwordStanceComponent()->PlayTriggeredHeavyAttackMontage();
}

void AMiyamotoIoriController::HeavyAttackCompletedInput(const FInputActionValue& valuve)
{
	GEngine->AddOnScreenDebugMessage(1, 3.0f, FColor::Red, TEXT("Completed Heavy Attack Input"));
	CurPlayableCharacter->GetCurSwordStanceComponent()->PlayCompletedHeavyAttackMontage();
}

void AMiyamotoIoriController::ChangeStanceInput(const FInputActionValue& value)
{
	UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 0.0f);
	isUIMode = true;
	PlayerHUD->SetSwordStanceUIVisibility(ESlateVisibility::Visible);
	GEngine->AddOnScreenDebugMessage(1, 5.0f, FColor::Red, TEXT("Pause Start"));
	//UGameplayStatics::SetGamePaused(GetWorld(), true);
}

void AMiyamotoIoriController::ChangeStanceCompletedInput(const FInputActionValue& value)
{
	if (isUIMode == false)
		return;
	UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 1.0f);
	isUIMode = false;
	PlayerHUD->SetSwordStanceUIVisibility(ESlateVisibility::Hidden);
	GEngine->AddOnScreenDebugMessage(2, 5.0f, FColor::Blue, TEXT("Pause End"));
	//UGameplayStatics::SetGamePaused(GetWorld(), false);
}

void AMiyamotoIoriController::UIMoveInput(const FInputActionValue& value)
{
	if (isUIMode == false)
		return;
	//GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Green, TEXT("UI Move Input"));
	float CurrentTime = GetWorld()->GetTimeSeconds();
	FVector2D MoveValue = value.Get<FVector2D>();
	FString test = FString::Printf(TEXT("X: %f, Y: %f"), MoveValue.X, MoveValue.Y);
	FString CurrentTimeString = FString::Printf(TEXT("Current Time: %f"), CurrentTime);
	GEngine->AddOnScreenDebugMessage(1, 3.0f, FColor::Red, CurrentTimeString);
}
