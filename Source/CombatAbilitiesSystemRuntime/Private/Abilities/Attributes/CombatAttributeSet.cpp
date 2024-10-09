// Fill out your copyright notice in the Description page of Project Settings.


#include "Abilities/Attributes/CombatAttributeSet.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "CombatGameplayTags.h"
#include "GameplayEffectExtension.h"
#include <Interfaces/CombatUIComponentInterface.h>
#include "Components/CombatUIComponent.h"


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
		auto* OwnerController = Data.Target.GetAvatarActor()->GetInstigatorController();
		if (OwnerController && OwnerController->Implements<UCombatUIComponentInterface>())
		{
			const float PercentHealth = GetHealth() / GetMaxHealth();
			auto* UIComponent{ ICombatUIComponentInterface::Execute_GetCombatUIComponent(OwnerController) };
			UIComponent->OnHealthChanged.Broadcast(PercentHealth);
		}

		UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(Data.Target.GetAvatarActor(), CombatGameplayTags::DeathAbility,
			FGameplayEventData());
	}

}