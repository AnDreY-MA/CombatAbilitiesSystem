// Fill out your copyright notice in the Description page of Project Settings.


#include "Abilities/Attributes/CombatAttributeSet.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "CombatGameplayTags.h"
#include "GameplayEffectExtension.h"


#include UE_INLINE_GENERATED_CPP_BY_NAME(CombatAttributeSet)


void UCombatAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);

	if(Attribute == GetHealthAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.0f, GetMaxHealth());
	}
	
}

void UCombatAttributeSet::PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);

	if(Data.EvaluatedData.Attribute == GetHealthAttribute())
	{
		UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(Data.Target.GetAvatarActor(), CombatGameplayTags::DeathAbility,
			FGameplayEventData());
	}

}