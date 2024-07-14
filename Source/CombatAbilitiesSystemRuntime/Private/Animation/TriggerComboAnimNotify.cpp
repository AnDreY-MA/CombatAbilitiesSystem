// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/TriggerComboAnimNotify.h"

#include "CombatComponentInterface.h"

void UTriggerComboAnimNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
                                     const FAnimNotifyEventReference& EventReference)
{
	if(auto* CombatComponent{MeshComp->GetOwner()->FindComponentByInterface<UCombatComponentInterface>()}; CombatComponent)
	{
		ICombatComponentInterface::Execute_RequestTriggerCombo(CombatComponent);
	}
	
}