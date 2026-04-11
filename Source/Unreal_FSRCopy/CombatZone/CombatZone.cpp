// Fill out your copyright notice in the Description page of Project Settings.


#include "CombatZone/CombatZone.h"
#include "Components/BoxComponent.h"
#include "Engine/DataTable.h"
#include "GameFramework/CharacterMovementComponent.h"

#include "PlayableCharacter/PlayableBaseCharacter.h"
#include "Monster/BaseMonster.h"
#include "ActorComponent/StateComponent/BaseStateComponent.h"

// Sets default values
ACombatZone::ACombatZone()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	static ConstructorHelpers::FObjectFinder<UDataTable> MonsterDataTableFinder(TEXT("/Script/Engine.DataTable'/Game/Blueprint/Monster/Data/Werewolf/DT_WerewolfStat.DT_WerewolfStat'"));
	if(MonsterDataTableFinder.Succeeded())
		MonsterDataTable = MonsterDataTableFinder.Object;
}

// Called when the game starts or when spawned
void ACombatZone::BeginPlay()
{
	Super::BeginPlay();
	CreateCombatZoneEntranceComponents();
}

// Called every frame
void ACombatZone::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ACombatZone::CreateCombatZoneEntranceComponents()
{
	for (const FCombatZoneEntrance& Entrance : CombatZoneEntrances)
	{
		UBoxComponent* EntranceComponent = NewObject<UBoxComponent>(this);
		if (EntranceComponent)
		{
			EntranceComponent->SetWorldLocation(Entrance.EntranceLocation);
			EntranceComponent->SetWorldRotation(Entrance.EntranceRotation);
			EntranceComponent->SetBoxExtent(Entrance.EntranceSize);
			EntranceComponent->RegisterComponent();
			EntranceComponent->SetHiddenInGame(false);
			EntranceComponent->SetCollisionProfileName(TEXT("CombatZoneEntrances"));
			EntranceComponent->OnComponentEndOverlap.AddDynamic(this, &ACombatZone::CombatZoneEntranceOnEndOverlap);
			CombatZoneEntranceComponents.Add(EntranceComponent);
		}
	}
}

void ACombatZone::CombatZoneEntranceOnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Red, TEXT("EndOverlap : ") + OtherActor->GetName());
	APlayableBaseCharacter* PC = Cast<APlayableBaseCharacter>(OtherActor);
	if (nullptr != PC)
	{
		PC->SetCombatMode();
		SpawnMonsters();
	}
}

void ACombatZone::SpawnMonsters()
{
	//몬스터 이름 찾는 코드 수정 예정
	FName MonsterName = StatDataNames[0];
	FBaseStat* MonsterStat = MonsterDataTable->FindRow<FBaseStat>(MonsterName, TEXT("MonsterDataTable"));
	if(nullptr != MonsterStat)
	{
		ABaseMonster* Monster = GetWorld()->SpawnActor<ABaseMonster>(SpawnMonsterClass, GetActorLocation(), GetActorRotation(), FActorSpawnParameters());

		if(nullptr == Monster)
			return;
		Monster->InitStat(*MonsterStat);
	}
}

