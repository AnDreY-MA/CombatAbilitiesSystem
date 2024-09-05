// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/Notifies/EventNotify.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemGlobals.h"

void UEventNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
                          const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	if(!MeshComp) return;
	if(!MeshComp->GetOwner()) return;

	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(MeshComp->GetOwner(), EventTag, FGameplayEventData());

	UE_LOG(LogTemp, Warning, TEXT("Send %s"), *MeshComp->GetOwner()->GetName());

	if(UAbilitySystemComponent* AbilityComponent{UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(MeshComp->GetOwner())}; AbilityComponent)
	{
		UE_LOG(LogTemp, Warning, TEXT("YES"))
	}
}
