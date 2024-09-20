// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/Notifies/ComboWindowAnimNotifyState.h"

#include "AbilitySystemComponent.h"
#include "CombatAbilitiesSystemModule.h"
#include "Interfaces/CombatComponentInterface.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(ComboWindowAnimNotifyState)

UComboWindowAnimNotifyState::UComboWindowAnimNotifyState(const FObjectInitializer& InInitializer) :
	Super(InInitializer), bEndCombo(false)
{
}

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
		if(!ICombatComponentInterface::Execute_IsActiveNextCombo(CombatComponent) || bEndCombo)
		{
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
		UE_LOG(LogCombatAbilitySystem, Warning, TEXT("CombatComponent is Null"));
		return;
	}
	
	const bool bOpenWindowCombo {ICombatComponentInterface::Execute_IsOpenComboWindow(CombatComponent)};
	const bool bShouldTriggerCombo {ICombatComponentInterface::Execute_IsShouldTriggerCombo(CombatComponent)};
	const bool bRequestTriggerCombo{ICombatComponentInterface::Execute_IsRequestTriggerCombo(CombatComponent)};
	
	if(bOpenWindowCombo && bShouldTriggerCombo && bRequestTriggerCombo && !bEndCombo)
	{

		if(ICombatComponentInterface::Execute_IsActiveNextCombo(CombatComponent))
		{
			return;
		}

		const UGameplayAbility* ComboAbility {ICombatComponentInterface::Execute_GetCurrentActiveComboAbility(CombatComponent)};
		if(!ComboAbility)
		{
			UE_LOG(LogCombatAbilitySystem, Warning, TEXT("ComboAbility is Null"));
			return;
		}

		auto* AbilityComponent {MeshComp->GetOwner()->GetComponentByClass<UAbilitySystemComponent>()};
		if(const bool bSuccess {AbilityComponent->TryActivateAbilityByClass(ComboAbility->GetClass())}; bSuccess)
		{
			ICombatComponentInterface::Execute_ActivateNextCombo(CombatComponent);
		}
		else
		{
			UE_LOG(LogCombatAbilitySystem, Verbose, TEXT("CombotNotifyTick Ability %s didn't activate"), *ComboAbility->GetClass()->GetName());
		}
	}
}