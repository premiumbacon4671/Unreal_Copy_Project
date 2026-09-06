// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/MainMenu/MainMenuButtonUserWidget.h"

#include "Components/TextBlock.h"
#include "Components/Image.h"

void UMainMenuButtonUserWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	FocusCover->SetVisibility(ESlateVisibility::Hidden);
}

void UMainMenuButtonUserWidget::InitializeButton(FName InButtonText)
{
	ButtonText->SetText(FText::FromName(InButtonText));
}

void UMainMenuButtonUserWidget::SetButtonFocusState(bool bIsFocused)
{
	if (bIsFocused)
		FocusCover->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	else
		FocusCover->SetVisibility(ESlateVisibility::Hidden);
}

void UMainMenuButtonUserWidget::ConfirmButtonClick()
{
	if (OnCustomMainMenuButtonClicked.IsBound())
	{
		OnCustomMainMenuButtonClicked.Broadcast();
	}
}
