// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/Notifies/UpdateWarpNotifyState.h"

#include "Components/CombatWarpingComponent.h"
#include "Interfaces/CombatWarpComponentInterface.h"

void UUpdateWarpNotifyState::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
                                         float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);

	const auto* AvatarOwner{MeshComp->GetOwner()};
	if(!AvatarOwner) return;

	if(AvatarOwner->Implements<UCombatWarpComponentInterface>())
	{
		auto* WarpComponent{ICombatWarpComponentInterface::Execute_GetCombatWarpingComponent(AvatarOwner)};
		WarpComponent->UpdateWarpTarget();
	}
	else if(auto* WarpComponent {AvatarOwner->FindComponentByClass<UCombatWarpingComponent>()}; WarpComponent)
	{
		WarpComponent->UpdateWarpTarget();
	}
}

void UUpdateWarpNotifyState::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);

	const auto* AvatarOwner{MeshComp->GetOwner()};
	if(!AvatarOwner) return;
	
	if(AvatarOwner->Implements<UCombatWarpComponentInterface>())
	{
		auto* WarpComponent{ICombatWarpComponentInterface::Execute_GetCombatWarpingComponent(AvatarOwner)};
		WarpComponent->ResetWarpTarget();
	}
	else if(auto* WarpComponent {AvatarOwner->FindComponentByClass<UCombatWarpingComponent>()}; WarpComponent)
	{
		WarpComponent->UpdateWarpTarget();
	}
	
}