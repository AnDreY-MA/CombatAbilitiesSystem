// Fill out your copyright notice in the Description page of Project Settings.


#include "Abilities/CombatAttackAbility.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "Interfaces/CombatComponentInterface.h"
#include "CombatSystemFunctionLibrary.h"
#include "Engine/DamageEvents.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(CombatAttackAbility)

void UCombatAttackAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                           const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
                                           const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	if(!CommitAbility(Handle, ActorInfo, ActivationInfo))
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, false, true);
		return;
	}

	if(auto* AbilityComponent{ActorInfo->AvatarActor.Get()->FindComponentByInterface<UCombatComponentInterface>()}; AbilityComponent)
	{
		ICombatComponentInterface::Execute_IncrementComboIndex(AbilityComponent);

		AttackAnimation = ICombatComponentInterface::Execute_GetComboMontageAction(AbilityComponent, AbilityTags.First());

		PlayMontageWaitEventsDefault(AttackAnimation.Montage, AttackAnimation.Speed);
	}

}

bool UCombatAttackAbility::ApplyDamageToTargetByEvent_Implementation(const FGameplayTag& InEventTag, FGameplayEventData InEventData)
{
	AActor* HitActor{ InEventData.Target };

	if (HitActors.Contains(HitActor)) return false;

	HitActors.AddUnique(HitActor);

	if (Super::ApplyDamageToTargetByEvent_Implementation(InEventTag, InEventData))
	{
		CurrentActorInfo->AnimInstance.Get()->Montage_Pause(AttackAnimation.Montage.Get());
		FTimerHandle TimerHandle;
		GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &UCombatAttackAbility::ResetMontage, PauseHitMontage);
		UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(HitActor, HitReactTag, InEventData);
		const FDamageEvent DamageEvent;
		HitActor->TakeDamage(1, DamageEvent, CurrentActorInfo->AvatarActor->GetInstigatorController(), CurrentActorInfo->AvatarActor.Get());
	}

	return false;
}

void UCombatAttackAbility::ResetMontage() const
{
	CurrentActorInfo->AnimInstance.Get()->Montage_Resume(AttackAnimation.Montage.Get());

}