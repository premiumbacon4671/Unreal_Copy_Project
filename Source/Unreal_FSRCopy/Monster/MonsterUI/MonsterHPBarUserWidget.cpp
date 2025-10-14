// Fill out your copyright notice in the Description page of Project Settings.


#include "Monster/MonsterUI/MonsterHPBarUserWidget.h"
#include "Components/ProgressBar.h"

void UMonsterHPBarUserWidget::SetHPBarPercent(float Percent)
{
	if (HPBar)
		HPBar->SetPercent(Percent);
}
