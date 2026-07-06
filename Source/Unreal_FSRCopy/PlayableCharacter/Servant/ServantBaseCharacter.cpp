// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayableCharacter/Servant/ServantBaseCharacter.h"
#include "ActorComponent/StateComponent/PlayableStateComponent.h"

AServantBaseCharacter::AServantBaseCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	StatusComponent = CreateDefaultSubobject<UPlayableStateComponent>(TEXT("StatusComponent"));

	static ConstructorHelpers::FObjectFinder<UDataTable> SaberDataTable(
		TEXT("/Script/Engine.DataTable'/Game/Blueprint/PlayableCharacter/Data/DT_SaberStat.DT_SaberStat'"));
	if (SaberDataTable.Succeeded())
		PlayableDataTable = SaberDataTable.Object;
}

void AServantBaseCharacter::BeginPlay()
{
	Super::BeginPlay();
	InitializeStatus();
}

void AServantBaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AServantBaseCharacter::InitializeStatus()
{
	FName StatDataName = *FString::Printf(TEXT("Level%d"), 1);
	FPlayableStat* SaberStat = PlayableDataTable->FindRow<FPlayableStat>(StatDataName, TEXT("SaberDataTable"));
	if (nullptr != SaberStat)
	{
		StatusComponent->InitState(*SaberStat);
	}
}
