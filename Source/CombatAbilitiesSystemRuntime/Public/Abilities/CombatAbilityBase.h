// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "CombatAbilityBase.generated.h"


UCLASS(Abstract)
class COMBATABILITIESSYSTEMRUNTIME_API UCombatAbilityBase : public UGameplayAbility
{
	GENERATED_BODY()

public:
	explicit UCombatAbilityBase(const FObjectInitializer& InInitializer = FObjectInitializer::Get());

protected:

	UFUNCTION()
	virtual void OnMontageCompleted(FGameplayTag EventTag, FGameplayEventData EventData);
	
	UFUNCTION()
	virtual void OnMontageCancelled(FGameplayTag EventTag, FGameplayEventData EventData);
	
	UFUNCTION()
	virtual void OnEventReceived(FGameplayTag EventTag, FGameplayEventData EventData);
		
	UFUNCTION(BlueprintCallable, Category="CombatAbility")
    void PlayMontageWaitEvents(UAnimMontage* InMontage, const FGameplayTagContainer& InEventTags, const float InRateMontage = 1.f,
    		const FName& InStartSection = NAME_None, const bool InbStopWhenAbilityEnds = true);

	UFUNCTION(BlueprintCallable, Category="CombatAbility")
	void PlayMontageWaitEventsDefault(UAnimMontage* InMontage, const float InRateMontage = 1.f,
		const FName& InStartSection = NAME_None, const bool InbStopWhenAbilityEnds = true);

	UFUNCTION(BlueprintCallable, Category="Abilities|Combat")
	void ActivateGameplayCue(const FGameplayTag& InCueTag, FGameplayCueParameters InParameters);
	
private:
	UPROPERTY(EditDefaultsOnly, Category="Combat|Events")
	FGameplayTagContainer WaitMontageEvents;
	
};