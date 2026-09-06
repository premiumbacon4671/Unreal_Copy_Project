// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/MainMenu/MainMenuUserWidget.h"
#include "UI/MainMenu/MainMenuButtonUserWidget.h"

#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"

void UMainMenuUserWidget::NativeConstruct()
{
	Super::NativeConstruct();
	CurrentSelectedIndex = 0;
	if (GameStartButton)
	{
		GameStartButton->OnCustomMainMenuButtonClicked.AddUniqueDynamic(this, &UMainMenuUserWidget::OnStartGameClicked);
		GameStartButton->InitializeButton(FName(TEXT("Game Start")));
		ButtonArray.Add(GameStartButton);
	}
	if (ExitButton)
	{
		ExitButton->OnCustomMainMenuButtonClicked.AddUniqueDynamic(this, &UMainMenuUserWidget::OnExitGameClicked);
		ExitButton->InitializeButton(FName(TEXT("Exit Game")));
		ButtonArray.Add(ExitButton);
	}
	ButtonArray[CurrentSelectedIndex]->SetButtonFocusState(true);
}

void UMainMenuUserWidget::OnInterfaceOpen()
{
}

void UMainMenuUserWidget::OnInterfaceClose()
{
}

void UMainMenuUserWidget::OnInterfaceConfirm()
{
	ButtonArray[CurrentSelectedIndex]->ConfirmButtonClick();
}

void UMainMenuUserWidget::OnInterfaceCancel()
{
}

void UMainMenuUserWidget::OnInterfaceMove(FIntPoint MoveDirection)
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

void UMainMenuUserWidget::OnStartGameClicked()
{
	UGameplayStatics::OpenLevel(GetWorld(), TEXT("TestBattleLevel"));
}

void UMainMenuUserWidget::OnExitGameClicked()
{
	UKismetSystemLibrary::QuitGame(GetWorld(), GetOwningPlayer(), EQuitPreference::Quit, false);
}
