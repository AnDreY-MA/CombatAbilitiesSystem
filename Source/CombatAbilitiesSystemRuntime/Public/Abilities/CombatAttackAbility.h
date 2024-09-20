// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ComboAbility.h"
#include "Data/CombatActionData.h"
#include "CombatAttackAbility.generated.h"


UCLASS(ClassGroup="CombatAbilitiesSystem", Abstract)
class COMBATABILITIESSYSTEM_API UCombatAttackAbility : public UComboAbility
{
	GENERATED_BODY()

protected:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	virtual bool ApplyDamageToTargetByEvent_Implementation(const FGameplayTag& InEventTag, FGameplayEventData InEventData) override;


private:
	UPROPERTY(EditDefaultsOnly, Category = "Combat|Hit")
	FGameplayTag HitReactTag;

	UPROPERTY(EditDefaultsOnly, Category="Combat|Hit")
	float PauseHitMontage{0.05f};
	
	UPROPERTY()
	FCombatAnimationInfo AttackAnimation;

	UPROPERTY()
	TArray<AActor*> HitActors;

	void ResetMontage() const;
	
};