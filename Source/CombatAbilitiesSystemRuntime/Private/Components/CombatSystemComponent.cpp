// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/CombatSystemComponent.h"

#include "CombatAbilitiesSystemModule.h"
#include "Abilities/CombatAttackAbility.h"
#include "Data/CombatActionData.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(CombatSystemComponent)

UCombatSystemComponent::UCombatSystemComponent(const FObjectInitializer& InInitializer) :
	Super(InInitializer), ComboIndex(0), bWindowComboAttack(false), bRequestTriggerCombo(false),
	bNextComboAbilityActivated(false),
	bShouldTriggerCombo(false)
{
}

void UCombatSystemComponent::AbilityLocalInputPressed(const int32 InputID)
{
	if(IsGenericConfirmInputBound(InputID))
	{
		LocalInputConfirm();
		return;
	}
	if(IsGenericCancelInputBound(InputID))
	{
		LocalInputCancel();
		return;
	}
	
	for(FGameplayAbilitySpec Spec : ActivatableAbilities.Items)
	{
		if(Spec.InputID == InputID && Spec.Ability)
		{
			Spec.InputPressed = true;

			if(Spec.Ability->IsA(UCombatAttackAbility::StaticClass()))
			{
				ActivateComboAbility(Spec.Ability.GetClass());
			}
			else
			{
				if(Spec.IsActive())
				{
					AbilitySpecInputPressed(Spec);
				}
				else
				{
					TryActivateAbility(Spec.Handle);
				}
			}
		}
	}
}

TArray<FCombatAnimationInfo> UCombatSystemComponent::GetMontageAction_Implementation(const FGameplayTag& InTagName) const
{
	if(!CombatActionTable)
	{
		UE_LOG(LogCombatAbilitySystem, Warning, TEXT("CombatActionTable is nullptr"));
		return {};
	}
	if(!InTagName.IsValid())
	{
		UE_LOG(LogCombatAbilitySystem, Warning, TEXT("Parrameter InTagName is no valid"));
		return {};
	}
	
	FCombatActionData ActionData = *CombatActionTable->FindRow<FCombatActionData>(InTagName.GetTagName(), "CombatContext");
	
	return ActionData.Animations;
}

FCombatAnimationInfo UCombatSystemComponent::GetComboMontageAction_Implementation(const FGameplayTag& InTagName)
{
	TArray<FCombatAnimationInfo> Animations = GetMontageAction_Implementation(InTagName);
	
	if(ComboIndex >= Animations.Num())
	{
		ComboIndex = 0;
	}

	return Animations.IsValidIndex(ComboIndex) ? Animations[ComboIndex] : FCombatAnimationInfo();
}

UGameplayAbility* UCombatSystemComponent::GetCurrentActiveComboAbility_Implementation() const
{
	TArray<UGameplayAbility*> Abilities = GetActiveAbilitiesByClass(UComboAbility::StaticClass());

	return Abilities.IsValidIndex(0) ? Abilities[0] : nullptr;
}

void UCombatSystemComponent::IncrementComboIndex_Implementation()
{
	if(bWindowComboAttack)
	{
		ComboIndex += 1;
	}
	
}

void UCombatSystemComponent::RequestTriggerCombo_Implementation()
{
	bRequestTriggerCombo = true;
}

void UCombatSystemComponent::ActivateNextCombo_Implementation()
{
	bNextComboAbilityActivated = true;

}

bool UCombatSystemComponent::IsActiveNextCombo_Implementation() const
{
	return bNextComboAbilityActivated;
}

void UCombatSystemComponent::ResetCombo_Implementation()
{
	ComboIndex = 0;
}

bool UCombatSystemComponent::IsShouldTriggerCombo_Implementation() const
{
	return bShouldTriggerCombo;
}

bool UCombatSystemComponent::IsRequestTriggerCombo_Implementation() const
{
	return bRequestTriggerCombo;
}

void UCombatSystemComponent::OpenComboWindow_Implementation()
{
	bWindowComboAttack = true;
}

bool UCombatSystemComponent::IsOpenComboWindow_Implementation() const
{
	return bWindowComboAttack;
}

void UCombatSystemComponent::CloseComboWindow_Implementation()
{
	bWindowComboAttack = false;
	bRequestTriggerCombo = false;
	bShouldTriggerCombo = false;
	bNextComboAbilityActivated = false;
}

void UCombatSystemComponent::ActivateComboAbility(const TSubclassOf<UGameplayAbility> InAbilityClass)
{
	bShouldTriggerCombo = false;

	if(IsUsingAbilityByClass(InAbilityClass))
	{
		bShouldTriggerCombo = bWindowComboAttack;
	}
	else
	{
		TryActivateAbilityByClass(InAbilityClass);
	}
	
}