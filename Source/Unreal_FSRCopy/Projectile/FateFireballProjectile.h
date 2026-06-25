// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Projectile/FateProjectile.h"
#include "FateFireballProjectile.generated.h"

/**
 * 
 */
UCLASS()
class UNREAL_FSRCOPY_API AFateFireballProjectile : public AFateProjectile
{
	GENERATED_BODY()
private:
	UPROPERTY()
	TArray<ACharacter*> PushedEnemies;

public:
	AFateFireballProjectile();
protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void OnComponentOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void OnComponentHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	void Explode() override;
	void LaunchProjectile(float Speed, float Damage, float DamageMult, float InRadius, float MaxDistance, ECollisionChannel InTargetChannel, TSubclassOf<ACharacter> InAllyClass) override;
};