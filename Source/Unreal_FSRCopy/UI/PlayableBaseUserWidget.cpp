// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/PlayableBaseUserWidget.h"
#include "Kismet/GameplayStatics.h"

void UPlayableBaseUserWidget::NativeConstruct()
{
	Super::NativeConstruct();
	bIsFocusable = true;
}

FReply UPlayableBaseUserWidget::NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent)
{
	if (InKeyEvent.GetKey() == EKeys::BackSpace)
	{
		if (HandleBackAction())
		{
			return FReply::Handled();
		}
		else
		{
			if (UICloseBehavior == EUICloseBehavior::HideAndKeepState)
			{
				SetVisibility(ESlateVisibility::Hidden);
			}
			else
			{
				RemoveFromParent();
			}
			UGameplayStatics::SetGamePaused(GetWorld(), false);
			APlayerController* PC = GetOwningPlayer();
			if (PC)
			{
				FInputModeGameOnly InputMode;
				PC->SetInputMode(InputMode);
			}
			return FReply::Handled();
		}
	}
	return Super::NativeOnKeyDown(InGeometry, InKeyEvent);
}

bool UPlayableBaseUserWidget::HandleBackAction()
{
	return false;
}
