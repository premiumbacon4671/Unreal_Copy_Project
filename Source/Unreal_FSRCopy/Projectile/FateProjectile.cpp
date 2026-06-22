// Fill out your copyright notice in the Description page of Project Settings.

#include "Projectile/FateProjectile.h"

#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/StaticMeshComponent.h"
#include "NiagaraComponent.h"
#include "Engine/EngineTypes.h"
#include "Engine/OverlapResult.h"
#include "GameFramework/Actor.h"
#include "Kismet/GameplayStatics.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraSystem.h"

#include "PublicUse/AttackCombatStruct/AttackCombatStruct.h"
#include "PlayableCharacter/PlayableBaseCharacter.h"

void AFateProjectile::Explode()
{
	if (bHasExploded)
		return;
	bHasExploded = true;

	if (ExplosionVFX)
	{
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(
			GetWorld(),
			ExplosionVFX,
			GetActorLocation(),
			GetActorRotation()
		);
	}

	//폭발 범위 내 감지된 액터 보관
	TArray<FOverlapResult> OverlapResults;
	//폭발형태 구체
	FCollisionShape SphereShpe = FCollisionShape::MakeSphere(ExplosionRadius);
	
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(this);
	if(GetInstigator())
		QueryParams.AddIgnoredActor(GetInstigator());
	if (GetOwner())
		QueryParams.AddIgnoredActor(GetOwner());

	bool bHasHits = GetWorld()->OverlapMultiByChannel(
		OverlapResults,
		GetActorLocation(),
		FQuat::Identity,
		TargetCollisionChannel,
		SphereShpe,
		QueryParams
	);
	if (bHasHits)
	{
		for (const FOverlapResult& Result : OverlapResults)
		{
			AActor* HitActor = Result.GetActor();
			if (!HitActor)
				continue;
			//동맹 캐릭터 무시
			if (AllyClassFilter && HitActor->IsA(AllyClassFilter))
				continue;

			const float Rand = FMath::FRandRange(0.8f, 1.2f);
			float CalculatedDamage = FinalDamage * Rand * DamageMultiplier;

			UGameplayStatics::ApplyDamage(
				HitActor,
				CalculatedDamage,	// 데미지 량
				GetInstigatorController(),
				this,
				UDamageType::StaticClass()
			);
		}
	}

	Destroy();
}

// Sets default values
AFateProjectile::AFateProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	FinalDamage = 0.0f;
	DamageMultiplier = 0.0f;
	ExplosionRadius = 0.0f;
	MaxTravelDistance = 0.0f;
	MaxTravelDistanceSq = 0.0f;
	bHasExploded = false;
	
	CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionComponent"));
	CollisionComponent->InitSphereRadius(20.0f);
	CollisionComponent->SetCollisionProfileName(TEXT("Projectile"));
	RootComponent = CollisionComponent;

	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComponent"));
	ProjectileMovementComponent->UpdatedComponent = CollisionComponent;
	ProjectileMovementComponent->bRotationFollowsVelocity = true; //회전
	ProjectileMovementComponent->bShouldBounce = false; //튕김
	ProjectileMovementComponent->ProjectileGravityScale = 0.0f; // 중력

	ProjectileMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ProjectileMeshComponent"));
	ProjectileMesh->SetupAttachment(RootComponent);
	ProjectileMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	ProjectileVFX = CreateDefaultSubobject<UNiagaraComponent>(TEXT("ProjectileVFX"));
	ProjectileVFX->SetupAttachment(RootComponent);

	InitialLifeSpan = 7.0f; //안전장치 수명
}

// Called when the game starts or when spawned
void AFateProjectile::BeginPlay()
{
	Super::BeginPlay();
	CollisionComponent->OnComponentHit.AddDynamic(this, &AFateProjectile::OnProjectileHit);
	SpawnLocation = GetActorLocation();
}

void AFateProjectile::OnProjectileHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (OtherActor && OtherActor != this && OtherActor != GetInstigator() && OtherActor != GetOwner())
	{
		//플레이어블 캐릭터 무시
		if (AllyClassFilter && OtherActor->IsA(AllyClassFilter))
			return;
		//UGameplayStatics::ApplyDamage(OtherActor, AttackData.DamageMultiplier, GetInstigatorController(), this, nullptr);
		Explode();
	}
}

// Called every frame
void AFateProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	//이미 폭발하면 거리 확인 안함
	if (bHasExploded)
		return;
	float TraveledDistanceSq = FVector::DistSquared(SpawnLocation, GetActorLocation());
	if (TraveledDistanceSq >= MaxTravelDistanceSq)
	{
		Explode();
	}

}

void AFateProjectile::LaunchProjectile(float Speed, float Damage, float DamageMult, float InRadius, float MaxDistance, ECollisionChannel InTargetChannel, TSubclassOf<ACharacter> InAllyClass)
{
	FinalDamage = Damage;
	DamageMultiplier = DamageMult;
	ExplosionRadius = InRadius;
	MaxTravelDistance = MaxDistance;
	MaxTravelDistanceSq = FMath::Square(MaxTravelDistance);
	TargetCollisionChannel = InTargetChannel;
	AllyClassFilter = InAllyClass;

	if (CollisionComponent)
	{
		//일단 먼저 플레이어, 몬스터 모두 콜리전 무시
		CollisionComponent->SetCollisionResponseToChannel(ECC_GameTraceChannel1, ECR_Ignore); // ECC_GameTraceChannel1 = PlayerableCharacter 무시
		CollisionComponent->SetCollisionResponseToChannel(ECC_GameTraceChannel3, ECR_Ignore); // ECC_GameTraceChannel3 = Monster 무시
		//플레이어, 몬스터 실제 타겟만 block
		CollisionComponent->SetCollisionResponseToChannel(TargetCollisionChannel, ECR_Block);
	}

	if (ProjectileMovementComponent)
	{
		ProjectileMovementComponent->InitialSpeed = Speed;
		ProjectileMovementComponent->MaxSpeed = Speed;
		ProjectileMovementComponent->Velocity = GetActorForwardVector() * Speed;
	}
}
