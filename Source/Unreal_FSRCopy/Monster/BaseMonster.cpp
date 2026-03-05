// Fill out your copyright notice in the Description page of Project Settings.


#include "Monster/BaseMonster.h"
#include "ActorComponent/StateComponent/BaseStateComponent.h"
#include "Components/WidgetComponent.h"
#include "Monster/MonsterUI/MonsterHPBarUserWidget.h"
#include "Kismet/KismetSystemLibrary.h"
#include "PlayableCharacter/PlayableBaseCharacter.h"

// Sets default values
ABaseMonster::ABaseMonster()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	StatusComponent = CreateDefaultSubobject<UBaseStateComponent>(TEXT("StatusComponent"));
	HPBarWidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("HPBarWidgetComponent"));
	HPBarWidgetComponent->SetupAttachment(GetRootComponent());
	static ConstructorHelpers::FClassFinder<UUserWidget> HPBarWidgetClass(
		TEXT("/Game/Blueprint/Monster/UI/BP_MonsterHPBar.BP_MonsterHPBar_C"));
	if (HPBarWidgetClass.Succeeded())
		HPBarWidgetComponent->SetWidgetClass(HPBarWidgetClass.Class);
	HPBarWidgetComponent->SetWidgetSpace(EWidgetSpace::Screen);
}

// Called when the game starts or when spawned
void ABaseMonster::BeginPlay()
{
	Super::BeginPlay();
	UMonsterHPBarUserWidget* HPBarWidget = Cast<UMonsterHPBarUserWidget>(HPBarWidgetComponent->GetUserWidgetObject());
	if(HPBarWidget && StatusComponent)
	{
		HPBarWidget->SetHPBarPercent(StatusComponent->GetHPPercent());
		StatusComponent->OnTakeDamage.BindLambda([this](float Percent)
			{
				UMonsterHPBarUserWidget* HPBarUI = Cast<UMonsterHPBarUserWidget>(HPBarWidgetComponent->GetUserWidgetObject());
				if(HPBarUI)
					HPBarUI->SetHPBarPercent(Percent);
			});
	}
}

// Called every frame
void ABaseMonster::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ABaseMonster::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ABaseMonster::HitBy(int DamageAmount)
{
	StatusComponent->TakeDamage(DamageAmount);
}

void ABaseMonster::MonAttackTrace()
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
		for(FHitResult& CharacterHitResult : HitResults)
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

