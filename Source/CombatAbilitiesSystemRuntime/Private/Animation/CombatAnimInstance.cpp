// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/CombatAnimInstance.h"

#include "AbilitySystemComponent.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(CombatAnimInstance)

void UCombatAnimInstance::NativeInitializeAnimation()
{
	if(const auto* OwnerActor{GetOwningActor()}; OwnerActor)
	{
		if(auto* AbilityComponent {OwnerActor->FindComponentByClass<UAbilitySystemComponent>()}; AbilityComponent)
		{
			InitializeWithAbilitySystem(AbilityComponent);
		}
	}
	
	Super::NativeInitializeAnimation();
}

void UCombatAnimInstance::InitializeWithAbilitySystem(UAbilitySystemComponent* InAbilityComponent)
{
	GameplayTagPropertyMap.Initialize(this, InAbilityComponent);
	ReceiveAbilitySystem(InAbilityComponent);
}
