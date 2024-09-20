// Fill out your copyright notice in the Description page of Project Settings.


#include "Abilities/CombatAbilityBase.h"

#include "AbilitySystemComponent.h"
#include "Abilities/Tasks/PlayMontageAndWaitForEvent.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(CombatAbilityBase)


UCombatAbilityBase::UCombatAbilityBase(const FObjectInitializer& InInitializer)
	: Super(InInitializer)
{
}

void UCombatAbilityBase::OnMontageCompleted_Implementation(FGameplayTag EventTag, FGameplayEventData EventData)
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

void UCombatAbilityBase::OnMontageCancelled_Implementation(FGameplayTag EventTag, FGameplayEventData EventData)
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
}

void UCombatAbilityBase::OnEventReceived_Implementation(FGameplayTag EventTag, FGameplayEventData EventData)
{
}

void UCombatAbilityBase::PlayMontageWaitEventsDefault(UAnimMontage* InMontage, const float InRateMontage,
	const FName& InStartSection, const bool InbStopWhenAbilityEnds)
{
	PlayMontageWaitEvents(InMontage, WaitMontageEvents, InRateMontage, InStartSection, InbStopWhenAbilityEnds);

}

void UCombatAbilityBase::PlayMontageWaitEvents(UAnimMontage* InMontage, const FGameplayTagContainer& InEventTags, const float InRateMontage,
	const FName& InStartSection, const bool InbStopWhenAbilityEnds)
{
	auto* MontageTask {UPlayMontageAndWaitForEvent::PlayMontageAndWaitForEvent(this, NAME_None, InMontage, InEventTags,
			InRateMontage, InStartSection, InbStopWhenAbilityEnds)};
	MontageTask->OnCompleted.AddDynamic(this, &UCombatAbilityBase::OnMontageCompleted);
	MontageTask->OnBlendOut.AddDynamic(this, &UCombatAbilityBase::OnMontageCompleted);
	MontageTask->OnCancelled.AddDynamic(this, &UCombatAbilityBase::OnMontageCancelled);
	MontageTask->OnInterrupted.AddDynamic(this, &UCombatAbilityBase::OnMontageCancelled);
	MontageTask->EventReceived.AddDynamic(this, &UCombatAbilityBase::OnEventReceived);
	MontageTask->ReadyForActivation();
}

void UCombatAbilityBase::ActivateGameplayCue(const FGameplayTag& InCueTag, FGameplayCueParameters InParameters)
{
	auto* SourceAbilityComponent{GetAbilitySystemComponentFromActorInfo_Checked()};

	if(InCueTag.IsValid())
	{
		InParameters.Instigator = SourceAbilityComponent->GetAvatarActor();
		InParameters.EffectContext = SourceAbilityComponent->MakeEffectContext();
		InParameters.SourceObject = SourceAbilityComponent;
		SourceAbilityComponent->GetOwnedGameplayTags(InParameters.AggregatedSourceTags);

		SourceAbilityComponent->AddGameplayCue(InCueTag, InParameters);
		TrackedGameplayCues.Add(InCueTag);
	}

}