// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayableCharacter/Notify/WeaponEquipAnimNotify.h"
#include "PlayableCharacter/PlayableBaseCharacter.h"

void UWeaponEquipAnimNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	APlayableBaseCharacter* Owner = Cast<APlayableBaseCharacter>(MeshComp->GetOwner());
	if (nullptr != Owner)
		Owner->WeaponEquip();
}
