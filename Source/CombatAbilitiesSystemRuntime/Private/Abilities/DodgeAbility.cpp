// Fill out your copyright notice in the Description page of Project Settings.


#include "Abilities/DodgeAbility.h"

#include "AbilitySystemComponent.h"
#include "Abilities/Tasks/AbilityTask_ApplyRootMotionMoveToForce.h"

#include "Interfaces/CombatComponentInterface.h"

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

	if(const auto* AbilityComponent{ActorInfo->AvatarActor.Get()->FindComponentByInterface<UCombatComponentInterface>()}; AbilityComponent)
	{
		const auto [Montage, Speed] {ICombatComponentInterface::Execute_GetMontageAction(AbilityComponent, AbilityTags.First())[0]};

		if(!Montage)
		{
			K2_EndAbility();
		}
		PlayMontageWaitEventsDefault(Montage.Get(), Speed);
	}

}