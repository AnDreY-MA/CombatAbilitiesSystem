// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/ComboWindowAnimNotifyState.h"

#include "AbilitySystemComponent.h"
#include "CombatAbilitiesSystemRuntimeModule.h"
#include "CombatComponentInterface.h"

void UComboWindowAnimNotifyState::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
                                              float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);

	if(!MeshComp) return;
	if(!MeshComp->GetOwner()) return;;
	
	CombatComponent = MeshComp->GetOwner()->FindComponentByInterface<UCombatComponentInterface>();
	if(CombatComponent)
	{
		ICombatComponentInterface::Execute_OpenComboWindow(CombatComponent);
	}
}

void UComboWindowAnimNotifyState::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);

	if(CombatComponent)
	{
		if(!ICombatComponentInterface::Execute_IsActiveNextCombo(CombatComponent) && bEndCombo)
		{
			UE_LOG(LogAbilitySystemComponent, Display, TEXT("RESET COMBO"));

			ICombatComponentInterface::Execute_ResetCombo(CombatComponent);
		}
		ICombatComponentInterface::Execute_CloseComboWindow(CombatComponent);
	}
	
}

void UComboWindowAnimNotifyState::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	float FrameDeltaTime, const FAnimNotifyEventReference& EventReference)
{
	if(!CombatComponent)
	{
		UE_LOG(LogAbilitySystemComponent, Warning, TEXT("CombatComponent is Null"));
		return;
	}
	
	const bool bOpenWindowCombo {ICombatComponentInterface::Execute_IsOpenComboWindow(CombatComponent)};
	const bool bShouldTriggerCombo {ICombatComponentInterface::Execute_IsShouldTriggerCombo(CombatComponent)};
	const bool bRequestTriggerCombo{ICombatComponentInterface::Execute_IsRequestTriggerCombo(CombatComponent)};
	
	if(bOpenWindowCombo && bShouldTriggerCombo && bRequestTriggerCombo && !bEndCombo)
	{
		UE_LOG(LogAbilitySystemComponent, Warning, TEXT("Request Combo"));

		if(ICombatComponentInterface::Execute_IsActiveNextCombo(CombatComponent))
		{
			UE_LOG(LogAbilitySystemComponent, Warning, TEXT("Active Next Combo"));
			return;
		}

		const UGameplayAbility* ComboAbility {ICombatComponentInterface::Execute_GetCurrentActiveComboAbility(CombatComponent)};
		if(!ComboAbility)
		{
			UE_LOG(LogAbilitySystemComponent, Warning, TEXT("ComboAbility is Null"));
			return;
		}

		UE_LOG(LogAbilitySystemComponent, Warning, TEXT("Combo Ability %s"), *ComboAbility->GetName());
		
		auto* AbilityComponent {MeshComp->GetOwner()->GetComponentByClass<UAbilitySystemComponent>()};
		const bool bSuccess {AbilityComponent->TryActivateAbilityByClass(ComboAbility->GetClass())};
		if(bSuccess)
		{
			ICombatComponentInterface::Execute_ActivateNextCombo(CombatComponent);
		}
		else
		{
			UE_LOG(LogCombatAbilitySystem, Verbose, TEXT("CombotNotifyTick Ability %s didn't activate"), *ComboAbility->GetClass()->GetName());
		}
	}
}