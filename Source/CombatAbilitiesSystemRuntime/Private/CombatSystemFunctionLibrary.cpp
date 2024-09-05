// Fill out your copyright notice in the Description page of Project Settings.


#include "CombatSystemFunctionLibrary.h"

#include "AbilitySystemComponent.h"
#include "Data/DamageData.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(CombatSystemFunctionLibrary)

void UCombatSystemFunctionLibrary::ApplyDamage(const FDamageEffectData& InDamageEffect)
{
	const auto* SourceComponent {InDamageEffect.SourceAbilityComponent.Get()};
	FGameplayEffectContextHandle EffectContext {SourceComponent->MakeEffectContext()};
	EffectContext.AddSourceObject(SourceComponent->GetAvatarActor());
	
	const FGameplayEffectSpecHandle& SpecHandle = SourceComponent->MakeOutgoingSpec(InDamageEffect.DamageEffectClass, 1, EffectContext); 
	InDamageEffect.TargetAbilityComponent->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data);

}