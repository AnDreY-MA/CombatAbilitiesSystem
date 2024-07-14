// Fill out your copyright notice in the Description page of Project Settings.


#include "Abilities/CombatAttackAbility.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "CombatComponentInterface.h"

void UCombatAttackAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                           const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
                                           const FGameplayEventData* TriggerEventData)
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
		ICombatComponentInterface::Execute_IncrementComboIndex(AbilityComponent);

		AttackMontage = ICombatComponentInterface::Execute_GetCurrentMontage(AbilityComponent);

		PlayMontageWaitEvent(AttackMontage, GetEventsTags());
		
	}
}

void UCombatAttackAbility::OnEventReceived(FGameplayTag EventTag, FGameplayEventData EventData)
{
	CurrentActorInfo->AnimInstance.Get()->Montage_Pause(AttackMontage);
	FTimerHandle TimerHandle;
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &UCombatAttackAbility::ResetMontage, PauseHitMontage);
	(void)ApplyGameplayEffectToTarget(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, EventData.TargetData, DamageEffectClass, 1);
}

void UCombatAttackAbility::ResetMontage() const
{
	CurrentActorInfo->AnimInstance.Get()->Montage_Resume(AttackMontage);

}