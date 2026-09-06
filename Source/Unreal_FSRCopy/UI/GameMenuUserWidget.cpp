// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/GameMenuUserWidget.h"
#include "UI/MainMenu/MainMenuButtonUserWidget.h"

#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"

void UGameMenuUserWidget::NativeConstruct()
{
	Super::NativeConstruct();
	CurrentSelectedIndex = 0;
	if (GoMainMenu)
	{
		GoMainMenu->OnCustomMainMenuButtonClicked.AddUniqueDynamic(this, &UGameMenuUserWidget::OnGoMainMenuClicked);
		GoMainMenu->InitializeButton(FName(TEXT("Go MainMenu")));
		ButtonArray.Add(GoMainMenu);
	}
	if (ExitGame)
	{
		ExitGame->OnCustomMainMenuButtonClicked.AddUniqueDynamic(this, &UGameMenuUserWidget::OnExitGameClicked);
		ExitGame->InitializeButton(FName(TEXT("Exit Game")));
		ButtonArray.Add(ExitGame);
	}
}

void UGameMenuUserWidget::OnInterfaceOpen()
{
	CurrentSelectedIndex = 0;
	ButtonArray[CurrentSelectedIndex]->SetButtonFocusState(true);
	SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	UGameplayStatics::SetGamePaused(GetWorld(), true);
}

void UGameMenuUserWidget::OnInterfaceClose()
{
}

void UGameMenuUserWidget::OnInterfaceConfirm()
{
	ButtonArray[CurrentSelectedIndex]->ConfirmButtonClick();
}

void UGameMenuUserWidget::OnInterfaceCancel()
{
	ButtonArray[CurrentSelectedIndex]->SetButtonFocusState(false);
	SetVisibility(ESlateVisibility::Hidden);
	UGameplayStatics::SetGamePaused(GetWorld(), false);
}

void UGameMenuUserWidget::OnInterfaceMove(FIntPoint MoveDirection)
{
	if (ButtonArray.Num() == 0)
		return;
	ButtonArray[CurrentSelectedIndex]->SetButtonFocusState(false);
	if (MoveDirection.X > 0)
		CurrentSelectedIndex--;
	else if (MoveDirection.X < 0)
		CurrentSelectedIndex++;
	CurrentSelectedIndex = FMath::Clamp(CurrentSelectedIndex, 0, ButtonArray.Num() - 1);
	ButtonArray[CurrentSelectedIndex]->SetButtonFocusState(true);
}


void UGameMenuUserWidget::OnGoMainMenuClicked()
{
	UGameplayStatics::OpenLevel(GetWorld(), TEXT("MainMenu"));
}


void UGameMenuUserWidget::OnExitGameClicked()
{
	UKismetSystemLibrary::QuitGame(GetWorld(), GetOwningPlayer(), EQuitPreference::Quit, false);
}