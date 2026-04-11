// Fill out your copyright notice in the Description page of Project Settings.


#include "Monster/CloseRangeMonster.h"
#include "Kismet/KismetSystemLibrary.h"
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

void ACloseRangeMonster::MonsterAttackTrace()
{
	TArray<FHitResult> HitResults;
	bool isHit = UKismetSystemLibrary::BoxTraceMulti(
		this,
		GetActorLocation(), // 박스의 시작 위치
		GetActorLocation() + GetActorForwardVector() * 100.0f, // 박스의 끝 위치
		FVector(50.0f, 50.0f, 50.0f), // 박스의 반지름 (X, Y, Z)
		FRotator::ZeroRotator, // 박스의 회전값
		UEngineTypes::ConvertToTraceType(ECC_GameTraceChannel4), // 트레이스 채널
		false, // 복잡한 충돌첼 충돌 무시 여부
		{}, // 무시할 액터 배열
		EDrawDebugTrace::ForDuration, // 디버그 드로잉 옵션
		HitResults, //HitResults에 결과 저장
		true// Trace에 자기을 무시할지 여부
	);

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
					//UGameplayStatics::ApplyDamage(HitPlayableCharacter, StatusComponent->GetTotalAttackPower(), GetController(), this, nullptr);
				}
			}
		}
	}
}