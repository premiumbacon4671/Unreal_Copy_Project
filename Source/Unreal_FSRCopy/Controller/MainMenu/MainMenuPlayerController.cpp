// Fill out your copyright notice in the Description page of Project Settings.


#include "Controller/MainMenu/MainMenuPlayerController.h"
#include "UI/MainMenu/MainMenuButtonUserWidget.h"
#include "UI/MainMenu/MainMenuUserWidget.h"

#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"
#include "InputAction.h"
#include "EnhancedInputComponent.h"

AMainMenuPlayerController::AMainMenuPlayerController()
{
	static ConstructorHelpers::FClassFinder<UMainMenuUserWidget> MainMenuUserWidgetClass(
		TEXT("/Game/Blueprint/Mainmenu/UI/BP_MainMenu"));
		//TEXT("/Game/Blueprint/Mainmenu/UI/BP_MainMenu.BP_MainMenu_C"));
	if (MainMenuUserWidgetClass.Succeeded())
		MainMenuWidgetClass = MainMenuUserWidgetClass.Class;

	static ConstructorHelpers::FObjectFinder<UInputMappingContext> InputMappingContextFinder(
		TEXT("/Script/EnhancedInput.InputMappingContext'/Game/Blueprint/Mainmenu/Input/IMC_MainMenu.IMC_MainMenu'"));
	if (InputMappingContextFinder.Succeeded())
		IMC_MainMenu = InputMappingContextFinder.Object;

	static ConstructorHelpers::FObjectFinder<UInputAction> MoveActionFinder(
		TEXT("/Script/EnhancedInput.InputAction'/Game/Blueprint/Mainmenu/Input/IA_MoveMainMenu.IA_MoveMainMenu'"));
	if (MoveActionFinder.Succeeded())
		MoveAction = MoveActionFinder.Object;

	static ConstructorHelpers::FObjectFinder<UInputAction> ConfirmActionFinder(
		TEXT("/Script/EnhancedInput.InputAction'/Game/Blueprint/Mainmenu/Input/IA_ConfirmMainMenu.IA_ConfirmMainMenu'"));
	if (ConfirmActionFinder.Succeeded())
		ConfirmAction = ConfirmActionFinder.Object;
}

void AMainMenuPlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (MainMenuWidgetClass)
	{
		if (MainMenuWidgetClass)
		{
			MainMenuWidget = CreateWidget<UMainMenuUserWidget>(this, MainMenuWidgetClass);
			if (MainMenuWidget)
			{
				MainMenuWidget->AddToViewport();
			}
		}
	}

	UEnhancedInputLocalPlayerSubsystem* InputSystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
	if (InputSystem != nullptr)
		InputSystem->AddMappingContext(IMC_MainMenu, 0);
}

void AMainMenuPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent))
	{
		if (MoveAction)
		{
			EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AMainMenuPlayerController::MenuUIMoveInput);
		}
		if (ConfirmAction)
		{
			EnhancedInputComponent->BindAction(ConfirmAction, ETriggerEvent::Started, this, &AMainMenuPlayerController::MenuUIConfirmInput);
		}
	}
}

void AMainMenuPlayerController::MenuUIMoveInput(const FInputActionValue& value)
{
	FVector2D MoveValue = value.Get<FVector2D>();
	FIntPoint iMoveValue(MoveValue.X, MoveValue.Y);
	if(MainMenuWidget)
		MainMenuWidget->OnInterfaceMove(iMoveValue);
}

void AMainMenuPlayerController::MenuUIConfirmInput(const FInputActionValue& value)
{
	if (MainMenuWidget)
		MainMenuWidget->OnInterfaceConfirm();
}
