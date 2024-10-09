// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/Notifies/TraceNotifyState.h"

#include "Components/CombatTraceComponent.h"
#include <Interfaces/CombatTraceComponentInterface.h>

#include UE_INLINE_GENERATED_CPP_BY_NAME(TraceNotifyState)


void UTraceNotifyState::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration,
                                    const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);

	AActor* Owner{ MeshComp->GetOwner() };
	if(!MeshComp || !Owner) return;
	UCombatTraceComponent* TraceComponent{ TryGetTraceComponent(Owner) };
	
	if (!TraceComponent) return;

	if (bChangeSockets)
	{
		TraceComponent->SetSocketsName(StartSocketName, EndSocketName);
	}

	TraceComponent->ActivateTrace();
	
}

void UTraceNotifyState::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);

	AActor* Owner{ MeshComp->GetOwner() };
	if (!MeshComp || !Owner) return;
	UCombatTraceComponent* TraceComponent{ TryGetTraceComponent(Owner) };
	if (!TraceComponent) return;

	TraceComponent->DeactivateTrace();
	
}

UCombatTraceComponent* UTraceNotifyState::TryGetTraceComponent(const AActor* InOwner)
{
	if (InOwner->Implements<UCombatTraceComponentInterface>())
	{
		return ICombatTraceComponentInterface::Execute_GetCombatTraceComponent(InOwner);
	}

	return InOwner->GetComponentByClass<UCombatTraceComponent>();
}