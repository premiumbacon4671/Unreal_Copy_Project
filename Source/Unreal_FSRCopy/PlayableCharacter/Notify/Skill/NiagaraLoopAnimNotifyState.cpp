// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayableCharacter/Notify/Skill/NiagaraLoopAnimNotifyState.h"

#include "NiagaraSystem.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"

void UNiagaraLoopAnimNotifyState::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);
	if (!MeshComp || !NiagaraSystem)
		return;

	// 나이아가라 컴포넌트 스폰 및 부착
	UNiagaraComponent* SpawnedComp = UNiagaraFunctionLibrary::SpawnSystemAttached(
		NiagaraSystem,
		MeshComp,
		NAME_None,
		LocationOffset,
		FRotator::ZeroRotator,
		EAttachLocation::KeepRelativeOffset,
		true
	);

	if (SpawnedComp)
	{
		// 멀티플레이어나 여러 액터가 동시에 사용할 때 본인의 이펙트만 정확히 찾아 지우기 위해
		// 이 노티파이 블록의 고유 주소값(this)을 태그로 등록합니다.
		FString UniqueTag = FString::Printf(TEXT("VFXLoop_%p"), this);
		SpawnedComp->ComponentTags.Add(FName(*UniqueTag));
	}
}

void UNiagaraLoopAnimNotifyState::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);

	if (!MeshComp || !NiagaraSystem) return;

	// 삭제 타겟 태그 생성
	FString UniqueTag = FString::Printf(TEXT("VFXLoop_%p"), this);
	FName TargetTag(*UniqueTag);

	// 캐릭터 메쉬에 붙어있는 자식 컴포넌트들을 순회하며 내 노티파이가 만든 이펙트만 저격 제거
	TArray<USceneComponent*> AttachedChildren;
	MeshComp->GetChildrenComponents(true, AttachedChildren);

	for (USceneComponent* Child : AttachedChildren)
	{
		UNiagaraComponent* NiagaraComp = Cast<UNiagaraComponent>(Child);
		if (NiagaraComp && NiagaraComp->ComponentTags.Contains(TargetTag))
		{
			// 부드러운 감쇠 소멸을 원하면 Deactivate, 즉시 칼같이 지우려면 DestroyComponent
			NiagaraComp->Deactivate();
		}
	}
}
