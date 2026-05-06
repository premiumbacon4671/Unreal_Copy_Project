// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/EngineTypes.h"
#include "AttackCombatStruct.generated.h"

/**
 * 
 */
//공격 판정에 필요한 데이터들을 담는 구조체를 관리하는 헤더 파일
//cpp파일 사용 안함
//AttackTrace 형태
UENUM(BlueprintType)
enum class EAttackShape : uint8
{
	Box,
	Sphere,
	Capsule
};

UENUM(BlueprintType)
enum class EAttackType : uint8
{
	CloseRange,
	LongRange
};

UENUM(BlueprintType)
enum class EAttackVariety : uint8
{
	Normal,
	Heavy,
	Special
};

//AttackTrace에 필요한 데이터들을 담는 구조체
USTRUCT(BlueprintType)
struct FAttackTraceData
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EAttackShape AttackShape;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float ForwardDistance;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector BoxHalfSize;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float SphereRadius;
};

//공격 판정에 필요한 데이터들을 담는 구조체
USTRUCT(BlueprintType)
struct FAttackData
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName MontageSectionName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float DamageMultiplier;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int AttackMaxCount;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int AttackCount;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EAttackType AttackType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FAttackTraceData AttackTraceData;
};