// Fill out your copyright notice in the Description page of Project Settings.


#include "Monster/BaseMonster.h"
#include "Components/WidgetComponent.h"

#include "Controller/MonsterAI/BaseMonsterAIController.h"
#include "ActorComponent/StateComponent/BaseStateComponent.h"
#include "Monster/MonsterUI/MonsterHPBarUserWidget.h"
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

	AIControllerClass = ABaseMonster::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
}

// Called when the game starts or when spawned
void ABaseMonster::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABaseMonster::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABaseMonster::HitBy(int DamageAmount)
{
	if(HitByMontage == nullptr)
		return;

	StatusComponent->TakeDamage(DamageAmount);
	if(StatusComponent->IsDead())
	{
		PlayAnimMontage(DeathMontage);
		CurrentCombatZone->OnMonsterDestroyed(this);
		GetWorld()->GetTimerManager().SetTimer(DeathTimerHandle, [this]()
			{
				Destroy();
			}, 5.0f, false);
	}
	else
		PlayAnimMontage(HitByMontage);
}

float ABaseMonster::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	if (HitByMontage == nullptr)
		return 0.0f;
	float DefencePower = StatusComponent->GetDefencePower();
	//최소데미지 1로 설정
	DamageAmount = FMath::Max(DamageAmount - DefencePower, 1.0f);
	float ActualDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	StatusComponent->TakeDamage(ActualDamage);

	if (StatusComponent->IsDead())
	{
		PlayAnimMontage(DeathMontage);

		GetWorld()->GetTimerManager().SetTimer(DeathTimerHandle, [this]()
			{
				Destroy();
			}, 5.0f, false);
	}
	else
		PlayAnimMontage(HitByMontage);
	return ActualDamage;
}

bool ABaseMonster::IsDead()
{
	if (bIsInitialized == false)
		return false;
	return StatusComponent->IsDead();
}

void ABaseMonster::MonsterNormalAttack(APlayableBaseCharacter* Target)
{
	if(StatusComponent->IsDead() || nullptr == NormalAttackMontage || nullptr == Target)
		return;
	if(GetMesh()->GetAnimInstance()->Montage_IsPlaying(NormalAttackMontage))
		return;

	FVector TargetDirection = Target->GetActorLocation() - GetActorLocation();
	FRotator LookAtRotation = FRotationMatrix::MakeFromX(TargetDirection).Rotator();
	SetActorRotation(LookAtRotation);
	PlayAnimMontage(NormalAttackMontage);
}

void ABaseMonster::PostInitializeComponents()
{
	Super::PostInitializeComponents(); 
	if (UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance())
	{
		GetMesh()->GetAnimInstance()->OnMontageEnded.AddDynamic(this, &ABaseMonster::OnAttackMontageEnded);
	}
}

void ABaseMonster::OnAttackMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	if(Montage == NormalAttackMontage || Montage == HitByMontage)
	{
		OnAttackMontageEndedDelegate.ExecuteIfBound();
	}
}

void ABaseMonster::InitStat(const FBaseStat& Data)
{
	StatusComponent->InitState(Data);
	bIsInitialized = true;

	//CombatZone에서 Spawn되는 몬스터는 BeginPlay 당시 스탯이 초기화되지 않은 상태이므로, HPBarWidget의 초기 HPPercent를 설정하기 위해 InitStat에서 처리
	UMonsterHPBarUserWidget* HPBarWidget = Cast<UMonsterHPBarUserWidget>(HPBarWidgetComponent->GetUserWidgetObject());
	if (HPBarWidget && StatusComponent)
	{
		HPBarWidget->SetHPBarPercent(StatusComponent->GetHPPercent());
		StatusComponent->OnUpdateHp.BindLambda([this](float Percent)
			{
				UMonsterHPBarUserWidget* HPBarUI = Cast<UMonsterHPBarUserWidget>(HPBarWidgetComponent->GetUserWidgetObject());
				if (HPBarUI)
				{
					HPBarUI->SetHPBarPercent(Percent);
				}
			});
	}
}

void ABaseMonster::AttackTrace(EAttackVariety AttackVariety)
{
	GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, TEXT("BaseMonster AttackTrace"));
}

float ABaseMonster::GetMonsterMaxHP() const
{
	return StatusComponent->GetMaxHP();
}
