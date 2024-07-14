// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/CombatSystemComponent.h"

#include "CombatAbilitiesSystemRuntimeModule.h"
#include "Abilities/CombatAttackAbility.h"
#include "Data/CombatActionData.h"

UCombatSystemComponent::UCombatSystemComponent()
{
	MeleeAttackAbilityClass = UCombatAttackAbility::StaticClass();
}

void UCombatSystemComponent::AbilityLocalInputPressed(int32 InputID)
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
	
	ABILITYLIST_SCOPE_LOCK()

	for(FGameplayAbilitySpec Spec : ActivatableAbilities.Items)
	{
		if(Spec.InputID == InputID && Spec.Ability)
		{
			Spec.InputPressed = true;

			if(Spec.Ability->IsA(UCombatAttackAbility::StaticClass()))
			{
				ActivateComboAbility(Spec.Ability.GetClass());
				UE_LOG(LogCombatAbilitySystem, Warning, TEXT("Combo"));
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

UAnimMontage* UCombatSystemComponent::GetCurrentMontage_Implementation()
{
	FCombatActionData ActionData = *CombatActionTable->FindRow<FCombatActionData>("Sword", "CombatContext");
	
	if(ComboIndex >= ActionData.Animations.Num())
	{
		ComboIndex = 0;
	}

	return ActionData.Animations.IsValidIndex(ComboIndex) ? ActionData.Animations[ComboIndex] : nullptr;
}

UAnimMontage* UCombatSystemComponent::GetDodgeMontage_Implementation()
{
	return DodgeMontage;
}

UGameplayAbility* UCombatSystemComponent::GetCurrentActiveComboAbility_Implementation() const
{
	TArray<UGameplayAbility*> Abilities = GetActiveAbilitiesByClass(MeleeAttackAbilityClass);

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