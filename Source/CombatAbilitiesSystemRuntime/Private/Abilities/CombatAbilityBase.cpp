// Fill out your copyright notice in the Description page of Project Settings.


#include "Abilities/CombatAbilityBase.h"

#include "Abilities/Tasks/PlayMontageAndWaitForEvent.h"

void UCombatAbilityBase::OnMontageCompleted(FGameplayTag EventTag, FGameplayEventData EventData)
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

void UCombatAbilityBase::OnMontageCancelled(FGameplayTag EventTag, FGameplayEventData EventData)
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
}

void UCombatAbilityBase::OnEventReceived(FGameplayTag EventTag, FGameplayEventData EventData)
{
}

void UCombatAbilityBase::PlayMontageWaitEvent(UAnimMontage* InMontage, const FGameplayTagContainer& InTagsEvents)
{
	auto* MontageTask {UPlayMontageAndWaitForEvent::PlayMontageAndWaitForEvent(this, NAME_None, InMontage, InTagsEvents,
			1.f)};
	MontageTask->OnCompleted.AddDynamic(this, &UCombatAbilityBase::OnMontageCompleted);
	MontageTask->OnBlendOut.AddDynamic(this, &UCombatAbilityBase::OnMontageCompleted);
	MontageTask->OnCancelled.AddDynamic(this, &UCombatAbilityBase::OnMontageCancelled);
	MontageTask->OnInterrupted.AddDynamic(this, &UCombatAbilityBase::OnMontageCancelled);
	MontageTask->EventReceived.AddDynamic(this, &UCombatAbilityBase::OnEventReceived);
	MontageTask->ReadyForActivation();
}
