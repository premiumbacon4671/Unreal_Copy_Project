// Fill out your copyright notice in the Description page of Project Settings.

#include "Projectile/FateProjectile.h"

#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/GameplayStatics.h"

#include "PublicUse/AttackCombatStruct/AttackCombatStruct.h"

// Sets default values
AFateProjectile::AFateProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionComponent"));
	CollisionComponent->InitSphereRadius(20.0f);
	CollisionComponent->SetCollisionProfileName(TEXT("Projectile"));
	RootComponent = CollisionComponent;

	ProjectileMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ProjectileMeshComponent"));
	ProjectileMeshComponent->SetupAttachment(RootComponent);
	ProjectileMeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComponent"));
	ProjectileMovementComponent->UpdatedComponent = CollisionComponent;
	ProjectileMovementComponent->bRotationFollowsVelocity = true; //회전
	ProjectileMovementComponent->ProjectileGravityScale = 0.0f; // 중력
}

// Called when the game starts or when spawned
void AFateProjectile::BeginPlay()
{
	Super::BeginPlay();
	CollisionComponent->OnComponentBeginOverlap.AddDynamic(this, &AFateProjectile::OnProjectileOverlap);
	SpawnLocation = GetActorLocation();
}

void AFateProjectile::OnProjectileOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && OtherActor != this && OtherActor != GetOwner())
	{
		//UGameplayStatics::ApplyDamage(OtherActor, AttackData.DamageMultiplier, GetInstigatorController(), this, nullptr);
		Destroy();
	}
}

// Called every frame
void AFateProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AFateProjectile::LaunchProjectile(float Speed, const FAttackData& InAttackData)
{
	AttackData = InAttackData;
	if (ProjectileMovementComponent)
	{
		ProjectileMovementComponent->InitialSpeed = Speed;
		ProjectileMovementComponent->MaxSpeed = Speed;
		ProjectileMovementComponent->Velocity = GetActorForwardVector() * Speed;
	}
}

