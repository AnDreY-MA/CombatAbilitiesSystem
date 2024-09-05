// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/CombatAbilityBase.h"
#include "Data/DamageData.h"
#include "DamageAbility.generated.h"

/**
 * 
 */
UCLASS(Abstract)
class COMBATABILITIESSYSTEMRUNTIME_API UDamageAbility : public UCombatAbilityBase
{
	GENERATED_BODY()

protected:
	FDamageEffectData MakeDamageData() const;
	
private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Combat|Damage", meta=(AllowPrivateAccess="true"))
	TSubclassOf<UGameplayEffect> DamageEffectClass;
	
};