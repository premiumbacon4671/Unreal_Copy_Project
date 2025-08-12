// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/SwordStanceButtonUI.h"
#include "Components/Image.h"
#include "PlayableCharacter/Miyamoto_Iori/Miyamoto_Iori.h"

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

		// Slate 다시 계산 강제
		Highlight->InvalidateLayoutAndVolatility(); // 이걸로도 안 되면 아래 방법 추가
		this->InvalidateLayoutAndVolatility(); // 부모도 다시 계산하도록 요청
		GetRootWidget()->InvalidateLayoutAndVolatility(); // 루트 위젯에 대해서도 다시 계산 요청
		
	}
}

void USwordStanceButtonUI::SetSwordStanceType(ESWORDSTANCE eSwordStance)
{
	SwordStanceType = eSwordStance;
}

ESlateVisibility USwordStanceButtonUI::GetHVisiblility()
{
	return Highlight->GetVisibility();
}
