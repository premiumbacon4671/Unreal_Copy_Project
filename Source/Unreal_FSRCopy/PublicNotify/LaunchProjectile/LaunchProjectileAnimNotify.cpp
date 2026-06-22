// Fill out your copyright notice in the Description page of Project Settings.


#include "PublicNotify/LaunchProjectile/LaunchProjectileAnimNotify.h"
#include "Projectile/FateProjectile.h"
#include "ActorComponent/SkillActionComponent/SkillActionComponent.h"
#include "DataAsset/PrimaryDataAsset/SkillDataAsset/ProjectileSkillDataAsset.h"

void ULaunchProjectileAnimNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);
	if (!MeshComp)
		return;
	AActor* OwnerActor = MeshComp->GetOwner();
	if (!OwnerActor)
		return;

	//플레이어블캐릭터와 몬스터 구분없이 스킬컴포넌트를 가지고 있으면 사용가능
	USkillActionComponent* SkillActionComp = OwnerActor->FindComponentByClass<USkillActionComponent>();
	if (SkillActionComp)
	{
		USkillDataAsset* ActiveSkill = SkillActionComp->GetCurrentSKillData();
		SkillActionComp->FireProjectileSkill(ActiveSkill);
	}
}
