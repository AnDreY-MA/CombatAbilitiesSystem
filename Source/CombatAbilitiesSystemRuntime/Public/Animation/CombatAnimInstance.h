// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectTypes.h"
#include "Animation/AnimInstance.h"
#include "CombatAnimInstance.generated.h"

/**
 * 
 */
UCLASS(Abstract)
class COMBATABILITIESSYSTEMRUNTIME_API UCombatAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	virtual void NativeInitializeAnimation() override;

	void InitializeWithAbilitySystem(UAbilitySystemComponent* InAbilityComponent);

	UFUNCTION(BlueprintImplementableEvent)
	void ReceiveAbilitySystem(UAbilitySystemComponent* InAbilitySystemComponent);
	
private:
	UPROPERTY(EditDefaultsOnly, Category="Gameplay")
	FGameplayTagBlueprintPropertyMap GameplayTagPropertyMap;

};