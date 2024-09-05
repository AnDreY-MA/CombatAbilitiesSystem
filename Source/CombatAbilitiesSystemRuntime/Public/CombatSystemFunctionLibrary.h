// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "CombatSystemFunctionLibrary.generated.h"

struct FDamageEffectData;
/**
 * 
 */
UCLASS()
class COMBATABILITIESSYSTEMRUNTIME_API UCombatSystemFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "CombatAbilitiesSystem")
	static void ApplyDamage(const FDamageEffectData& InDamageEffect);
	
};
