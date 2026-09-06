// Fill out your copyright notice in the Description page of Project Settings.


#include "CombatZone/CombatZone.h"
#include "Components/BoxComponent.h"
#include "Engine/DataTable.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/GameplayStatics.h"
#include "LevelSequencePlayer.h"

#include "PlayableCharacter/PlayableBaseCharacter.h"
#include "PlayableCharacter/Servant/ServantBaseCharacter.h"
#include "ActorComponent/SkillActionComponent/SkillActionComponent.h"
#include "Monster/BaseMonster.h"
#include "ActorComponent/StateComponent/BaseStateComponent.h"
#include "ActorComponent/StateComponent/MonsterStateComponent.h"
#include "ActorComponent/ResonanceComponent/ResonanceComponent.h"
#include "Controller/MiyamotoIoriController/MiyamotoIoriController.h"

// Sets default values
ACombatZone::ACombatZone()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	static ConstructorHelpers::FObjectFinder<UDataTable> MonsterDataTableFinder(TEXT("/Script/Engine.DataTable'/Game/Blueprint/Monster/Data/Werewolf/DT_WerewolfStatV2.DT_WerewolfStatV2'"));
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
			EntranceComponent->AttachToComponent(GetRootComponent(), FAttachmentTransformRules::KeepWorldTransform);
			EntranceComponent->SetWorldLocation(Entrance.EntranceLocation);
			EntranceComponent->SetWorldRotation(Entrance.EntranceRotation);
			EntranceComponent->SetBoxExtent(Entrance.EntranceSize);
			EntranceComponent->RegisterComponent();
			EntranceComponent->SetHiddenInGame(true);
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
		PC->SetCurrentCombatZone(this);
		ActivePlayer = PC;
		AMiyamotoIoriController* PlayerController = Cast<AMiyamotoIoriController>(PC->GetController());
		if (PlayerController)
		{
			AServantBaseCharacter* ServantChar = PlayerController->GetServantCharacter("Saber");

			if (ServantChar != nullptr && ServantChar != PC)
			{
				ServantChar->SetCombatMode();
				FVector RightOffset = PC->GetActorRightVector() * 150.0f;
				FVector BackOffset = PC->GetActorForwardVector() * 50.0f;
				FVector TargetLocation = PC->GetActorLocation() + RightOffset + BackOffset;

				FRotator TargetRotation = PC->GetActorRotation();

				ServantChar->SetActorLocationAndRotation(TargetLocation, TargetRotation, false, nullptr, ETeleportType::TeleportPhysics);

				ServantChar->SetCurrentCombatZone(this);
				ActiveServant = ServantChar;
			}
			for (UBoxComponent* Entrance : CombatZoneEntranceComponents)
			{
				Entrance->OnComponentEndOverlap.RemoveDynamic(this, &ACombatZone::CombatZoneEntranceOnEndOverlap);

				Entrance->SetCollisionResponseToChannel(ECC_GameTraceChannel1, ECR_Block);
				ActivePlayer->GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_GameTraceChannel2, ECR_Block);
				if(ActiveServant)
					ActiveServant->GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_GameTraceChannel2, ECR_Block);
			}
			SpawnMonsters();
		}
	}
}

void ACombatZone::SpawnMonsters()
{
	CurrentWaveIndex++;

	FName MonsterName = StatDataNames[0];
	FMonsterStat* MonsterStat = MonsterDataTable->FindRow<FMonsterStat>(MonsterName, TEXT("MonsterDataTable"));
	if(nullptr != MonsterStat)
	{
		for(int32 i = 0; i < MonstersPerWave; ++i)
		{
			FVector RandomOffset(FMath::RandRange(-550.0f, 550.0f), FMath::RandRange(-550.0f, 550.0f), 0.0f);
			FVector SpawnLocation = GetActorLocation() + RandomOffset;
			FRotator SpawnRotation = GetActorRotation();
			if(ActivePlayer)
			{
				FVector DirectionToPlayer = ActivePlayer->GetActorLocation() - SpawnLocation;
				DirectionToPlayer.Z = 0.0f;
				SpawnRotation = DirectionToPlayer.Rotation();
			}
			ABaseMonster* Monster = GetWorld()->SpawnActor<ABaseMonster>(SpawnMonsterClass, SpawnLocation, SpawnRotation, FActorSpawnParameters());

			if (nullptr == Monster)
				return;
			Monster->InitStat(*MonsterStat);
			Monster->SetCurrentCombatZone(this);
			LiveMonsters.Add(Monster);
		}
	}
}

void ACombatZone::OnMonsterDestroyed(ABaseMonster* DestroyedActor)
{
	LiveMonsters.Remove(DestroyedActor);

	if (LiveMonsters.Num() == 0 && !bIsCleared)
	{
		if (CurrentWaveIndex < MaxWaves)
		{
			FTimerHandle WaveSpawnTimerHandle;
			GetWorldTimerManager().SetTimer(WaveSpawnTimerHandle,
				this,
				&ACombatZone::SpawnMonsters, 1.0f, false);;
		}
		else
		{
			bIsCleared = true;
			for (UBoxComponent* Entrance : CombatZoneEntranceComponents)
			{
				if (Entrance)
				{
					Entrance->SetCollisionEnabled(ECollisionEnabled::NoCollision);
				}
			}
			
			FTimerHandle ClearCombatModeTimerHandle;
			GetWorldTimerManager().SetTimer(ClearCombatModeTimerHandle, [this]()
				{
					/*UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 1.0f);
					if (ActivePlayer)
						ActivePlayer->CustomTimeDilation = 1.0f;
					if (ActiveServant)
						ActiveServant->CustomTimeDilation = 1.0f;*/
					
					APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
					if (PC)
					{
						if (PC->GetPawn() != ActivePlayer)
						{
							AMiyamotoIoriController* PlayerController = Cast<AMiyamotoIoriController>(PC);
							if (PlayerController)
							{
								//PlayerController->SwapWithServant();
								PlayerController->HandleGaugeDepleted();
							}
						}
					}
					if (ActivePlayer)
					{
						ActivePlayer->SetCombatMode(); // 吵档 根鸥林 角青
						ActivePlayer->SetCurrentCombatZone(nullptr);
					}

					if (ActiveServant)
					{
						ActiveServant->SetCombatMode(); // 吵档 根鸥林 角青
						ActiveServant->SetCurrentCombatZone(nullptr);
					}
				}, 1.0f, false);
			
		}
	}
}

