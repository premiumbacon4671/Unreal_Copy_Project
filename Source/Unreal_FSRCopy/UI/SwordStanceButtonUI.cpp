// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/SwordStanceButtonUI.h"
#include "Components/Image.h"

void USwordStanceButtonUI::SetColor(FString HexColor)
{
    FColor SRGBColor = FColor::FromHex(HexColor);
    FLinearColor LinearColor = FLinearColor(SRGBColor);
    SwordStance->Brush.TintColor = FSlateColor(LinearColor);
}

void USwordStanceButtonUI::SetSwordStanceVisibility(ESlateVisibility eVisibility)
{
	if (SwordStance)
	{
		SwordStance->SetVisibility(eVisibility);
	}
}

void USwordStanceButtonUI::SetHighlightVisibility(ESlateVisibility eVisibility)
{
	if (Highlight)
	{
		Highlight->SetVisibility(eVisibility);
	}
}

ESlateVisibility USwordStanceButtonUI::GetHVisiblility()
{
	return Highlight->GetVisibility();
}
