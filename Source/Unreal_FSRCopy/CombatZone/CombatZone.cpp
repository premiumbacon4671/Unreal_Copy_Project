// Fill out your copyright notice in the Description page of Project Settings.


#include "CombatZone/CombatZone.h"
#include "Components/BoxComponent.h"
#include "PlayableCharacter/PlayableBaseCharacter.h"

// Sets default values
ACombatZone::ACombatZone()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

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
	}
}

