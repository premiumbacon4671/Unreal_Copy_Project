// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/SwordStanceUI.h"
#include "Components/Image.h"
#include "Components/CanvasPanel.h"
#include "PlayableCharacter/Miyamoto_Iori/Miyamoto_Iori.h"
#include "UI/SwordStanceButtonUI.h"
#include "PlayableCharacter/Miyamoto_Iori/ActorComponent/BaseSwordStanceActorComponent.h"

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
		//ui가 켜질 때 상태를 파악해서 하일라이트를 켜는 방향으로 생각 중
		if(Miyamoto.GetCurSwordStance() == ESWORDSTANCE::EST_EARTH)
		{
			CurStanceIndex = FIntPoint(2, 1);
		}
	}
	if (Miyamoto.GetIsUnlockSwordStance(ESWORDSTANCE::EST_WATER) == true)
	{
		WaterStance->SetSwordStanceVisibility(ESlateVisibility::Visible);
		if (Miyamoto.GetCurSwordStance() == ESWORDSTANCE::EST_WATER)
		{
			CurStanceIndex = FIntPoint(1, 2);
		}
	}
	if (Miyamoto.GetIsUnlockSwordStance(ESWORDSTANCE::EST_FIRE) == true)
	{
		FireStance->SetSwordStanceVisibility(ESlateVisibility::Visible);
		if (Miyamoto.GetCurSwordStance() == ESWORDSTANCE::EST_FIRE)
		{
			CurStanceIndex = FIntPoint(1, 0);
		}
	}
	if (Miyamoto.GetIsUnlockSwordStance(ESWORDSTANCE::EST_WIND) == true)
	{
		WindStance->SetSwordStanceVisibility(ESlateVisibility::Visible);
		if (Miyamoto.GetCurSwordStance() == ESWORDSTANCE::EST_WIND)
		{
			CurStanceIndex = FIntPoint(0, 1);
		}
	}
	if (Miyamoto.GetIsUnlockSwordStance(ESWORDSTANCE::EST_VOID) == true)
	{
		VoidStance->SetSwordStanceVisibility(ESlateVisibility::Visible);
		if (Miyamoto.GetCurSwordStance() == ESWORDSTANCE::EST_VOID)
		{
			CurStanceIndex = FIntPoint(1, 1);
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
		//EarthStance = CreateWidget<USwordStanceButtonUI>(GetWorld(), SwordButtonClass);
		Buttons[2][1] = EarthStance;
		EarthStance->SetSwordStanceType(ESWORDSTANCE::EST_EARTH);
		//WaterStance = CreateWidget<USwordStanceButtonUI>(GetWorld(), SwordButtonClass);
		Buttons[1][2] = WaterStance;
		WaterStance->SetSwordStanceType(ESWORDSTANCE::EST_WATER);
		//VoidStance = CreateWidget<USwordStanceButtonUI>(GetWorld(), SwordButtonClass);
		Buttons[1][1] = VoidStance;
		VoidStance->SetSwordStanceType(ESWORDSTANCE::EST_VOID);
		//FireStance = CreateWidget<USwordStanceButtonUI>(GetWorld(), SwordButtonClass);
		Buttons[1][0] = FireStance;
		FireStance->SetSwordStanceType(ESWORDSTANCE::EST_FIRE);
		//WindStance = CreateWidget<USwordStanceButtonUI>(GetWorld(), SwordButtonClass);
		Buttons[0][1] = WindStance; 
		WindStance->SetSwordStanceType(ESWORDSTANCE::EST_WIND);
	}
}

bool USwordStanceUI::Initialize()
{
	Super::Initialize();
	Init(*Cast<AMiyamoto_Iori>(GetOwningPlayerPawn()));
	return false;
}

void USwordStanceUI::SelectSwordStance(FIntPoint StanceIndex)
{
	FIntPoint NextStanceIndex = ChangeStanceIndex + StanceIndex;

	//3X3 배열 범위 밖
	if (NextStanceIndex.X < 0 || NextStanceIndex.X >= Buttons.Num()
		|| NextStanceIndex.Y < 0 || NextStanceIndex.Y >= Buttons[NextStanceIndex.X].Num())
		return;

	//검증 예정
	//좌상단
	if (NextStanceIndex == FIntPoint(0, 0))
	{
		if (StanceIndex.X < 0)
			NextStanceIndex.Y -= StanceIndex.X;
		else if (StanceIndex.Y < 0)
			NextStanceIndex.X -= StanceIndex.Y;
	}
	//우상단
	else if (NextStanceIndex == FIntPoint(0, 2))
	{
		if (StanceIndex.X < 0)
			NextStanceIndex.Y += StanceIndex.X;
		else if (StanceIndex.Y > 0)
			NextStanceIndex.X += StanceIndex.Y;
	}
	//좌하단
	else if (NextStanceIndex == FIntPoint(2, 0))
	{
		if (StanceIndex.X > 0)
			NextStanceIndex.Y += StanceIndex.X;
		else if (StanceIndex.Y < 0)
			NextStanceIndex.X += StanceIndex.Y;
	}
	//우하단
	else if (NextStanceIndex == FIntPoint(2, 2))
	{
		if (StanceIndex.X > 0)
			NextStanceIndex.Y -= StanceIndex.X;
		else if (StanceIndex.Y > 0)
			NextStanceIndex.X -= StanceIndex.Y;
	}
	//좌우 안먹음 확인필요
	FString DebugString = FString::Printf(TEXT("NextStanceIndex : %d, %d"), NextStanceIndex.X, NextStanceIndex.Y);
	GEngine->AddOnScreenDebugMessage(1, 5.0f, FColor::Red, DebugString);
	FString DebugString2 = FString::Printf(TEXT("CurStanceIndex : %d, %d"), ChangeStanceIndex.X, ChangeStanceIndex.Y);
	GEngine->AddOnScreenDebugMessage(2, 5.0f, FColor::Blue, DebugString2);
	Buttons[ChangeStanceIndex.X][ChangeStanceIndex.Y]->SetHighlightVisibility(ESlateVisibility::Hidden);
	Buttons[NextStanceIndex.X][NextStanceIndex.Y]->SetHighlightVisibility(ESlateVisibility::Visible);
	ChangeStanceIndex = NextStanceIndex;
}

void USwordStanceUI::StartedSwordStanceUI()
{
	ChangeStanceIndex = CurStanceIndex;
	Buttons[CurStanceIndex.X][CurStanceIndex.Y]->SetHighlightVisibility(ESlateVisibility::Visible);
}

void USwordStanceUI::EndedSwordStanceUI(AMiyamoto_Iori* Miyamoto)
{
	for (int x = 0; x < Buttons.Num(); x++)
	{
		for (int y = 0; y < Buttons[x].Num(); y++)
		{
			if (Buttons[x][y].Get() == nullptr)
				continue;
			Buttons[x][y]->SetHighlightVisibility(ESlateVisibility::Hidden);
		}
	}
	ESWORDSTANCE NextStance = Buttons[ChangeStanceIndex.X][ChangeStanceIndex.Y]->GetSwordStanceType();
	//현재 타입과 바꿀 타입이 같을 때
	if (Miyamoto->GetCurSwordStance() == NextStance)
		return;
	//바꿀 타입이 해금이 안됐을 때
	if (Miyamoto->GetIsUnlockSwordStance(NextStance) == false)
		return;
	CurStanceIndex = ChangeStanceIndex;
	Miyamoto->ChangeSwordStance(NextStance);
}
