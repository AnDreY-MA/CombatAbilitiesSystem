// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/Notifies/TriggerComboAnimNotify.h"
#include "Interfaces/CombatComponentInterface.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(TriggerComboAnimNotify)


void UTriggerComboAnimNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
                                     const FAnimNotifyEventReference& EventReference)
{
	if(auto* CombatComponent{MeshComp->GetOwner()->FindComponentByInterface<UCombatComponentInterface>()}; CombatComponent)
	{
		ICombatComponentInterface::Execute_RequestTriggerCombo(CombatComponent);
	}
	
}