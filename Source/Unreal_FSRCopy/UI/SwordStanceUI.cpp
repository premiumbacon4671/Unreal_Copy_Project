// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/SwordStanceUI.h"
#include "Components/Image.h"
#include "PlayableCharacter/Miyamoto_Iori/Miyamoto_Iori.h"

void USwordStanceUI::Init(ESWORDSTANCE CurSwordStance, TMap<ESWORDSTANCE, bool> isUnlockSwordStance)
{
	for (auto& iter : isUnlockSwordStance)
	{
		if(iter.Value == false)
			continue;
		switch (iter.Key)
		{
			case ESWORDSTANCE::EST_EARTH:
				EarthStance->SetVisibility(ESlateVisibility::Visible);
				if (iter.Key == CurSwordStance)
					Earth_Highlight->SetVisibility(ESlateVisibility::Visible);
				break;
			case ESWORDSTANCE::EST_WATER:
				WaterStance->SetVisibility(ESlateVisibility::Visible);
				if (iter.Key == CurSwordStance)
					Water_Highlight->SetVisibility(ESlateVisibility::Visible);
				break;
			case ESWORDSTANCE::EST_FIRE:
				FireStance->SetVisibility(ESlateVisibility::Visible);
				if (iter.Key == CurSwordStance)
					Fire_Highlight->SetVisibility(ESlateVisibility::Visible);
				break;
			case ESWORDSTANCE::EST_WIND:
				WindStance->SetVisibility(ESlateVisibility::Visible);
				if (iter.Key == CurSwordStance)
					Wind_Highlight->SetVisibility(ESlateVisibility::Visible);
				break;
			case ESWORDSTANCE::EST_VOID:
				VoidStance->SetVisibility(ESlateVisibility::Visible);
				if (iter.Key == CurSwordStance)
					Void_Highlight->SetVisibility(ESlateVisibility::Visible);
				break;
		}
	}
}
