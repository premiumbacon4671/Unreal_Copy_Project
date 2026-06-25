// Fill out your copyright notice in the Description page of Project Settings.


#include "Monster/CloseRangeMonster.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/DamageEvents.h"

#include "ActorComponent/StateComponent/BaseStateComponent.h"
#include "ActorComponent/StateComponent/MonsterStateComponent.h"
#include "PlayableCharacter/PlayableBaseCharacter.h"

ACloseRangeMonster::ACloseRangeMonster()
{
	//지금 현재 웨어울프 하나이기에 여기에 작성하지만 나중에 몬스터 종류가 많아지면 몬스터 종류별로 애니메이션을 설정할 수 있도록 수정예정
	static ConstructorHelpers::FObjectFinder<UAnimMontage> HitByMontageObjectFinder(
		TEXT("/Script/Engine.AnimMontage'/Game/Blueprint/Monster/Animation/Werewolf/AM_WerewolfHitBy.AM_WerewolfHitBy'"));
	if(HitByMontageObjectFinder.Succeeded())
		HitByMontage = HitByMontageObjectFinder.Object;

	static ConstructorHelpers::FObjectFinder<UAnimMontage> DeathMontageObjectFinder(
		TEXT("/Script/Engine.AnimMontage'/Game/Blueprint/Monster/Animation/Werewolf/AM_WerewolfDeath.AM_WerewolfDeath'"));
	if (DeathMontageObjectFinder.Succeeded())
		DeathMontage = DeathMontageObjectFinder.Object;

	static ConstructorHelpers::FObjectFinder<UAnimMontage> NormalAttackMontageObjectFinder(
		TEXT("/Script/Engine.AnimMontage'/Game/Blueprint/Monster/Animation/Werewolf/AM_WerewolfAttack.AM_WerewolfAttack'"));
	if (NormalAttackMontageObjectFinder.Succeeded())
		NormalAttackMontage = NormalAttackMontageObjectFinder.Object;
}

void ACloseRangeMonster::AttackTrace(EAttackVariety AttackVariety)
{
	//AttackVariety 일반 몬스터는 일반 공격만 존재하기에 AttackVariety는 사용하지 않음
	TArray<FHitResult> HitResults;
	bool isHit;
	switch (MonsterAttackData.AttackTraceData.AttackShape)
	{
	default:
	case EAttackShape::Box:
		isHit = UKismetSystemLibrary::BoxTraceMulti(
			this,
			GetActorLocation(), // 박스의 시작 위치
			GetActorLocation() + GetActorForwardVector() * MonsterAttackData.AttackTraceData.ForwardDistance, // 박스의 끝 위치
			MonsterAttackData.AttackTraceData.BoxHalfSize, // 박스의 반지름 (X, Y, Z)
			FRotator::ZeroRotator, // 박스의 회전값
			UEngineTypes::ConvertToTraceType(ECC_GameTraceChannel5), // 트레이스 채널
			false, // 복잡한 충돌첼 충돌 무시 여부
			{}, // 무시할 액터 배열
			EDrawDebugTrace::ForDuration, // 디버그 드로잉 옵션
			HitResults, //HitResults에 결과 저장
			true// Trace에 자기을 무시할지 여부
		);
		break;
	}
	

	if (isHit)
	{
		//중복 타격 방지
		TSet<AActor*> HitActors;
		for (FHitResult& CharacterHitResult : HitResults)
		{
			APlayableBaseCharacter* HitPlayableCharacter = Cast<APlayableBaseCharacter>(CharacterHitResult.GetActor());
			if (HitPlayableCharacter && !HitActors.Contains(HitPlayableCharacter))
			{
				HitActors.Add(HitPlayableCharacter);
				//저스트 회피 판정
				if (HitPlayableCharacter->GetIsPerfectDodgeWindow())
				{
					HitPlayableCharacter->OnPerfectDodgeSuccess(this);
				}
				//일반 회피 판정
				else if (HitPlayableCharacter->IsEvading())
				{
					GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::White, TEXT("일반 회피 성공 (데미지 무시)"));
				}
				//공격 피격 판정
				else
				{
					int DamageAmount = StatusComponent->GetAttackPower();
					//데미지 랜덤화 (0.8~1.2배)
					const float Rand = FMath::FRandRange(0.8f, 1.2f);
					const int32 FinalDamage = DamageAmount * Rand;
					UGameplayStatics::ApplyDamage(HitPlayableCharacter, FinalDamage, GetController(), this, nullptr);
				}
			}
		}
	}
}