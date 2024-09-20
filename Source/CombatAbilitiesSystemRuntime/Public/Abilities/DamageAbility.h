// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/CombatAbilityBase.h"
#include "Data/DamageData.h"
#include "DamageAbility.generated.h"

/**
 * 
 */
UCLASS(ClassGroup="CombatAbilitiesSystem", Abstract)
class COMBATABILITIESSYSTEM_API UDamageAbility : public UCombatAbilityBase
{
	GENERATED_BODY()

public:
	virtual void OnEventReceived_Implementation(FGameplayTag InEventTag, FGameplayEventData InEventData) override;


protected:
	FDamageEffectData MakeDamageData() const;


	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "CombatAbility|DamageAbility")
	bool ApplyDamageToTargetByEvent(const FGameplayTag& InEventTag, FGameplayEventData InEventData);
	
private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Combat|Damage", meta = (AllowPrivateAccess = "true"))
	FGameplayTag DamageHitTag;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Combat|Damage", meta=(AllowPrivateAccess="true"))
	TSubclassOf<UGameplayEffect> DamageEffectClass;
	
};