// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "BaseMonster.generated.h"

UCLASS()
class UNREAL_FSRCOPY_API ABaseMonster : public ACharacter
{
	GENERATED_BODY()

private:
	UPROPERTY(VisibleAnywhere, Category = "Status")
	TObjectPtr<class UBaseStateComponent> StatusComponent;
	UPROPERTY(VisibleAnywhere, Category = "UI")
	TObjectPtr<class UWidgetComponent> HPBarWidgetComponent;

	FTimerHandle DeathTimerHandle;
protected:
	//자식 클래스에서 설정
	UPROPERTY(VisibleAnywhere, Category = "Montage")
	TObjectPtr<UAnimMontage> HitByMontage;
	UPROPERTY(VisibleAnywhere, Category = "Montage")
	TObjectPtr<UAnimMontage> DeathMontage;
public:
	// Sets default values for this character's properties
	ABaseMonster();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void HitBy(int DamageAmount);
	bool IsDead();
};
