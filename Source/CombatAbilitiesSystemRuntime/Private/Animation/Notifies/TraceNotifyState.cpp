// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/Notifies/TraceNotifyState.h"

#include "Components/CombatTraceComponent.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(TraceNotifyState)


void UTraceNotifyState::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration,
                                    const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);

	if(!MeshComp || !MeshComp->GetOwner()) return;

	if(auto* TraceComponent{MeshComp->GetOwner()->GetComponentByClass<UCombatTraceComponent>()}; TraceComponent)
	{
		TraceComponent->ActivateTrace();
	}
	
}

void UTraceNotifyState::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);

	if (!MeshComp || !MeshComp->GetOwner()) return;

	if(auto* TraceComponent{MeshComp->GetOwner()->GetComponentByClass<UCombatTraceComponent>()}; TraceComponent)
	{
		TraceComponent->DeactivateTrace();
	}
	
}