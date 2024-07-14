// Fill out your copyright notice in the Description page of Project Settings.


#include "Abilities/DodgeAbility.h"

#include "AbilitySystemComponent.h"
#include "Abilities/Tasks/AbilityTask_ApplyRootMotionMoveToForce.h"

#include "CombatComponentInterface.h"

void UDodgeAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
                                    const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	if(!CommitAbility(Handle, ActorInfo, ActivationInfo))
	{
		UE_LOG(LogAbilitySystemComponent, Warning, TEXT("End from Commit"));
		EndAbility(Handle, ActorInfo, ActivationInfo, false, true);
		return;
	}

	if(auto* AbilityComponent{ActorInfo->AvatarActor.Get()->FindComponentByInterface<UCombatComponentInterface>()}; AbilityComponent)
	{
		auto* Montage {ICombatComponentInterface::Execute_GetDodgeMontage(AbilityComponent)};

		PlayMontageWaitEvent(Montage, {});
	}

}