// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CombatZone.generated.h"

USTRUCT(BlueprintType)
struct FCombatZoneEntrance
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CombatZoneEntrance")
	FVector EntranceLocation;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CombatZoneEntrance")
	FRotator EntranceRotation;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CombatZoneEntrance")
	FVector EntranceSize;
};

UCLASS()
class UNREAL_FSRCOPY_API ACombatZone : public AActor
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CombatZoneEntrance", meta = (AllowPrivateAccess = "true"))
	TArray<FCombatZoneEntrance> CombatZoneEntrances;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CombatZoneEntrance", meta = (AllowPrivateAccess = "true"))
	TArray<class UBoxComponent*> CombatZoneEntranceComponents;

	UPROPERTY(EditAnywhere, Category = "Spawn", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class ABaseMonster> SpawnMonsterClass;
	UPROPERTY(EditAnywhere, Category = "Spawn", meta = (AllowPrivateAccess = "true"))
	TArray<FName> StatDataNames;
	TObjectPtr<class UDataTable> MonsterDataTable;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn", meta = (AllowPrivateAccess = "true"))
	TArray<class ABaseMonster*> LiveMonsters;
public:
	// Sets default values for this actor's properties
	ACombatZone();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	void CreateCombatZoneEntranceComponents();
	UFUNCTION()
	void CombatZoneEntranceOnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	UFUNCTION()
	void SpawnMonsters();

	//전투존에 존재하는 몬스터들을 반환하는 함수
	const TArray<class ABaseMonster*>& GetLiveMonsters() const { return LiveMonsters; }
	void OnMonsterDestroyed(class ABaseMonster* DestroyedActor);
};
