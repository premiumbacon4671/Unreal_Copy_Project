// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/SwordStanceUI.h"
#include "Components/Image.h"
#include "PlayableCharacter/Miyamoto_Iori/Miyamoto_Iori.h"
#include "UI/SwordStanceButtonUI.h"

void USwordStanceUI::Init(AMiyamoto_Iori& Miyamoto)
{
	EarthStance->SetColor(TEXT("#AD8C00FF"));
	WaterStance->SetColor(TEXT("#00B9E3FF"));
	VoidStance->SetColor(TEXT("#84349CFF"));
	FireStance->SetColor(TEXT("#CA0000FF"));
	WindStance->SetColor(TEXT("#008A00FF"));

	if (&Miyamoto == nullptr)
		return;
	if (Miyamoto.GetIsUnlockSwordStance(ESWORDSTANCE::EST_EARTH) == true)
	{
		EarthStance->SetSwordStanceVisibility(ESlateVisibility::Visible);
		if(Miyamoto.GetCurSwordStance() == ESWORDSTANCE::EST_EARTH)
		{
			EarthStance->SetHighlightVisibility(ESlateVisibility::Visible);
			CurStanceIndex = FVector2D(2.0f, 1.0f);
		}
	}
	if (Miyamoto.GetIsUnlockSwordStance(ESWORDSTANCE::EST_WATER) == true)
	{
		WaterStance->SetSwordStanceVisibility(ESlateVisibility::Visible);
		if (Miyamoto.GetCurSwordStance() == ESWORDSTANCE::EST_WATER)
		{
			WaterStance->SetHighlightVisibility(ESlateVisibility::Visible);
			CurStanceIndex = FVector2D(1.0f, 2.0f);
		}
	}
	if (Miyamoto.GetIsUnlockSwordStance(ESWORDSTANCE::EST_FIRE) == true)
	{
		FireStance->SetSwordStanceVisibility(ESlateVisibility::Visible);
		if (Miyamoto.GetCurSwordStance() == ESWORDSTANCE::EST_FIRE)
		{
			FireStance->SetHighlightVisibility(ESlateVisibility::Visible);
			CurStanceIndex = FVector2D(1.0f, 0.0f);
		}
	}
	if (Miyamoto.GetIsUnlockSwordStance(ESWORDSTANCE::EST_WIND) == true)
	{
		WindStance->SetSwordStanceVisibility(ESlateVisibility::Visible);
		if (Miyamoto.GetCurSwordStance() == ESWORDSTANCE::EST_WIND)
		{
			WindStance->SetHighlightVisibility(ESlateVisibility::Visible);
			CurStanceIndex = FVector2D(0.0f, 1.0f);
		}
	}
	if (Miyamoto.GetIsUnlockSwordStance(ESWORDSTANCE::EST_VOID) == true)
	{
		VoidStance->SetSwordStanceVisibility(ESlateVisibility::Visible);
		if (Miyamoto.GetCurSwordStance() == ESWORDSTANCE::EST_VOID)
		{
			VoidStance->SetHighlightVisibility(ESlateVisibility::Visible);
			CurStanceIndex = FVector2D(1.0f, 1.0f);
		}
	}
}

void USwordStanceUI::NativeConstruct()
{
	Super::NativeConstruct();
	Buttons.SetNum(3);
	Buttons[0].SetNum(3);
	Buttons[1].SetNum(3);
	Buttons[2].SetNum(3);
	if (SwordButtonClass)
	{
		EarthStance = CreateWidget<USwordStanceButtonUI>(GetWorld(), SwordButtonClass);
		Buttons[2][1] = EarthStance;
		WaterStance = CreateWidget<USwordStanceButtonUI>(GetWorld(), SwordButtonClass);
		Buttons[1][2] = WaterStance;
		VoidStance = CreateWidget<USwordStanceButtonUI>(GetWorld(), SwordButtonClass);
		Buttons[1][1] = VoidStance;
		FireStance = CreateWidget<USwordStanceButtonUI>(GetWorld(), SwordButtonClass);
		Buttons[1][0] = FireStance;
		WindStance = CreateWidget<USwordStanceButtonUI>(GetWorld(), SwordButtonClass);
		Buttons[0][1] = WindStance;
	}
}

bool USwordStanceUI::Initialize()
{
	Super::Initialize();
	Init(*Cast<AMiyamoto_Iori>(GetOwningPlayerPawn()));
	return false;
}

void USwordStanceUI::SelectSwordStance(FVector2D StanceIndex)
{
}
