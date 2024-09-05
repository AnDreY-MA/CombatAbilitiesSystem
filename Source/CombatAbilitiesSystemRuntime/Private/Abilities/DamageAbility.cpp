// Fill out your copyright notice in the Description page of Project Settings.


#include "Abilities/DamageAbility.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystemGlobals.h"

FDamageEffectData UDamageAbility::MakeDamageData() const
{
	FDamageEffectData DamageData;
	DamageData.DamageEffectClass = DamageEffectClass;
	DamageData.SourceAbilityComponent = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(CurrentActorInfo->AvatarActor.Get());

	return DamageData;
}
