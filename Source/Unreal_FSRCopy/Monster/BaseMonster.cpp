// Fill out your copyright notice in the Description page of Project Settings.


#include "Monster/BaseMonster.h"
#include "ActorComponent/StateComponent/BaseStateComponent.h"
#include "Components/WidgetComponent.h"
#include "Monster/MonsterUI/MonsterHPBarUserWidget.h"

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

void ABaseMonster::HitBy(int DamageAmount)
{
	if(HitByMontage == nullptr)
		return;
	StatusComponent->TakeDamage(DamageAmount);
	if(StatusComponent->IsDead())
	{
		PlayAnimMontage(DeathMontage);
		GetWorld()->GetTimerManager().SetTimer(DeathTimerHandle, [this]()
			{
				Destroy();
			}, 5.0f, false);
	}
	else
		PlayAnimMontage(HitByMontage);
}

bool ABaseMonster::IsDead()
{
	return StatusComponent->IsDead();
}
