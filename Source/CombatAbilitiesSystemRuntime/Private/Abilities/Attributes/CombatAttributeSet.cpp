// Fill out your copyright notice in the Description page of Project Settings.


#include "Abilities/Attributes/CombatAttributeSet.h"

void UCombatAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);

	if(Attribute == GetHealthAttribute())
	{
		//NewValue = FMath::Clamp(0.0f, GetHe)
	}
}

void UCombatAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);
}
