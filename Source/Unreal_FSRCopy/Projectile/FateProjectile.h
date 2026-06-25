// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "PublicUse/AttackCombatStruct/AttackCombatStruct.h"

#include "FateProjectile.generated.h"

UCLASS()
class UNREAL_FSRCOPY_API AFateProjectile : public AActor
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(VisibleAnywhere, Category = "Collision")
	TObjectPtr<class USphereComponent> CollisionComponent;
	UPROPERTY(VisibleAnywhere, Category = "Movement")
	TObjectPtr<class UProjectileMovementComponent> ProjectileMovementComponent;
	UPROPERTY(VisibleAnywhere, Category = "Components")
	TObjectPtr<class UNiagaraComponent> ProjectileVFX;
	UPROPERTY(VisibleAnywhere, Category = "Components")
	TObjectPtr<class UStaticMeshComponent> ProjectileMesh;

	UPROPERTY(EditAnywhere, Category = "VFX")
	TObjectPtr<class UNiagaraSystem> ExplosionVFX;

	float FinalDamage;
	float DamageMultiplier;
	float ExplosionRadius;
	float MaxTravelDistance;
	float MaxTravelDistanceSq;
	FVector SpawnLocation;
	bool bHasExploded;

	ECollisionChannel TargetCollisionChannel;
	TSubclassOf<ACharacter> AllyClassFilter;

	virtual void Explode();
public:	
	// Sets default values for this actor's properties
	AFateProjectile();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnProjectileHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void LaunchProjectile(float Speed, float Damage, float DamageMult, float InRadius, float MaxDistance, ECollisionChannel InTargetChannel, TSubclassOf<ACharacter> InAllyClass);
};
