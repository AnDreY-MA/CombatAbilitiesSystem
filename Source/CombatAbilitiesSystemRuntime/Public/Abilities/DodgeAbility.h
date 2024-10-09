 // Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/CombatAbilityBase.h"
#include "DodgeAbility.generated.h"


UCLASS(ClassGroup="CombatAbilitiesSystem", Abstract)
class COMBATABILITIESSYSTEM_API UDodgeAbility : public UCombatAbilityBase
{
	GENERATED_BODY()

protected:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	
	UFUNCTION(BlueprintNativeEvent)
	void ApplyDodge();

};
