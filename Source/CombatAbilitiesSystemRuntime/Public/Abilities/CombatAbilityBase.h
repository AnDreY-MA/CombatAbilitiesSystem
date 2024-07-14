// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "CombatAbilityBase.generated.h"


UCLASS()
class COMBATABILITIESSYSTEMRUNTIME_API UCombatAbilityBase : public UGameplayAbility
{
	GENERATED_BODY()

protected:

	UFUNCTION()
	virtual void OnMontageCompleted(FGameplayTag EventTag, FGameplayEventData EventData);
	
	UFUNCTION()
	virtual void OnMontageCancelled(FGameplayTag EventTag, FGameplayEventData EventData);
	
	UFUNCTION()
	virtual void OnEventReceived(FGameplayTag EventTag, FGameplayEventData EventData);

	UFUNCTION(BlueprintCallable)
	void PlayMontageWaitEvent(UAnimMontage* InMontage, const FGameplayTagContainer& InTagsEvents);

	FORCENOINLINE FGameplayTagContainer GetEventsTags() const { return WaitMontageEvents;}
	
private:
	UPROPERTY(EditDefaultsOnly, Category="Combat|Events")
	FGameplayTagContainer WaitMontageEvents;
	
};