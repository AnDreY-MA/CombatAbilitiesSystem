// Fill out your copyright notice in the Description page of Project Settings.


#include "Abilities/DamageAbility.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystemGlobals.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(DamageAbility)


FDamageEffectData UDamageAbility::MakeDamageData() const
{
	FDamageEffectData DamageData;
	DamageData.DamageEffectClass = DamageEffectClass;
	DamageData.SourceAbilityComponent = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(CurrentActorInfo->AvatarActor.Get());

	return DamageData;
}

void UDamageAbility::OnEventReceived_Implementation(FGameplayTag InEventTag, FGameplayEventData InEventData)
{
	(void)ApplyDamageToTargetByEvent(InEventTag, InEventData);
}

bool UDamageAbility::ApplyDamageToTargetByEvent_Implementation(const FGameplayTag& InEventTag, FGameplayEventData InEventData)
{
	if (InEventTag == DamageHitTag)
	{
		const FGameplayEffectSpecHandle SpecHandle = MakeOutgoingGameplayEffectSpec(DamageEffectClass, 1);
		(void)K2_ApplyGameplayEffectSpecToTarget(SpecHandle, InEventData.TargetData);
		return true;
	}

	return false;

}