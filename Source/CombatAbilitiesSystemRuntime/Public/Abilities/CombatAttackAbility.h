// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/CombatAbilityBase.h"
#include "CombatAttackAbility.generated.h"


UCLASS()
class COMBATABILITIESSYSTEMRUNTIME_API UCombatAttackAbility : public UCombatAbilityBase
{
	GENERATED_BODY()

protected:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	virtual void OnEventReceived(FGameplayTag EventTag, FGameplayEventData EventData) override;


private:
	UPROPERTY(EditDefaultsOnly, Category="Combat")
	float PauseHitMontage{0.05f};

	UPROPERTY(EditDefaultsOnly, Category="Combat")
	TSubclassOf<UGameplayEffect> DamageEffectClass;

	UPROPERTY()
	TObjectPtr<UAnimMontage> AttackMontage;

	void ResetMontage() const;

	
};