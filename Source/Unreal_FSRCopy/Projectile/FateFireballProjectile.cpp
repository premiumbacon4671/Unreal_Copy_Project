// Fill out your copyright notice in the Description page of Project Settings.


#include "Projectile/FateFireballProjectile.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "GameFramework/Character.h"
#include "NiagaraFunctionLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"

#include "PlayableCharacter/PlayableBaseCharacter.h"
#include "Monster/BaseMonster.h"
#include "ActorComponent/StateComponent/MonsterStateComponent.h"

AFateFireballProjectile::AFateFireballProjectile()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AFateFireballProjectile::BeginPlay()
{
	Super::BeginPlay();
	if (CollisionComponent)
	{
		//부모클래스(FateProjectTile)이 가진 OnComponentHit을 초기화시켜 기존의 폭발시스템 제거
		CollisionComponent->OnComponentHit.Clear();

		CollisionComponent->OnComponentBeginOverlap.AddDynamic(this, &AFateFireballProjectile::OnComponentOverlapBegin);
		CollisionComponent->OnComponentHit.AddDynamic(this, &AFateFireballProjectile::OnComponentHit);
	}
}

void AFateFireballProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	//일반 몬스터 파이어볼 진행방향으로 계속 밀어내기
	if (PushedEnemies.Num() > 0 && ProjectileMovementComponent)
	{
		FVector PushVelocity = GetActorForwardVector() * ProjectileMovementComponent->InitialSpeed;
		float CheckRadius = 150.0f;
		/*for (ACharacter* Enemy : PushedEnemies)
		{
			if (IsValid(Enemy))
			{
				Enemy->LaunchCharacter(PushVelocity, true, true);
			}
		}*/
		for (int32 i = PushedEnemies.Num() - 1; i >= 0; --i)
		{
			ACharacter* Enemy = PushedEnemies[i];
			if (!IsValid(Enemy))
			{
				PushedEnemies.RemoveAt(i);
				continue;
			}
			float Distance = FVector::Dist(GetActorLocation(), Enemy->GetActorLocation());
			if (Distance > CheckRadius)
			{
				PushedEnemies.RemoveAt(i);
				continue;
			}
			Enemy->LaunchCharacter(PushVelocity, true, true);
		}
	}
}

void AFateFireballProjectile::OnComponentOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!OtherActor || OtherActor == GetOwner())
		return;
	//아군 무시
	if (AllyClassFilter && OtherActor->IsA(AllyClassFilter))
		return;

	//몬스터가 발사하고 플레이블캐릭터가 맞은 경우
	if (OtherActor->IsA(APlayableBaseCharacter::StaticClass()))
	{
		Explode();
		return;
	}

	ABaseMonster* EnemyMonster = Cast<ABaseMonster>(OtherActor);
	if (EnemyMonster)
	{
		UMonsterStateComponent* StatComp = EnemyMonster->FindComponentByClass<UMonsterStateComponent>();
		if (StatComp)
		{
			//플레이어블캐릭터가 발사하고 정예 또는 보스 몬스터가 맞을 경우
			if (StatComp->GetMonsterTier() == EMonsterTier::Elite || StatComp->GetMonsterTier() == EMonsterTier::Boss)
			{
				Explode();
				return;
			}
		}
		if (!PushedEnemies.Contains(EnemyMonster))
		{
			const float Rand = FMath::FRandRange(0.8, 1.2f);
			float CalculatedDamage = FinalDamage * Rand * DamageMultiplier;
			UGameplayStatics::ApplyDamage(
				EnemyMonster,
				CalculatedDamage,	// 데미지 량
				GetInstigatorController(),
				this,
				UDamageType::StaticClass()
			);
			PushedEnemies.AddUnique(EnemyMonster);
		}
	}
}

void AFateFireballProjectile::OnComponentHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (!OtherActor || OtherActor == GetOwner())
		return;

	//벽과 같은 오브젝트에 부딪힐 경우
	if (OtherActor->IsA(APlayableBaseCharacter::StaticClass()) || OtherActor->IsA(ABaseMonster::StaticClass()))
		return;
	
	Explode();
}

void AFateFireballProjectile::Explode()
{
	Super::Explode();
}

void AFateFireballProjectile::LaunchProjectile(float Speed, float Damage, float DamageMult, float InRadius, float MaxDistance, ECollisionChannel InTargetChannel, TSubclassOf<ACharacter> InAllyClass)
{
	Super::LaunchProjectile(Speed, Damage, DamageMult, InRadius, MaxDistance, InTargetChannel, InAllyClass);
}
