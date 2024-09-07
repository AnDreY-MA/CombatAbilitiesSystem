#pragma once

#include "DamageData.generated.h"

class UGameplayEffect;
class UAbilitySystemComponent;

USTRUCT(BlueprintType)
struct COMBATABILITIESSYSTEM_API FDamageEffectData
{
	GENERATED_BODY()

	FDamageEffectData() = default;
	
	UPROPERTY(BlueprintReadWrite)
	TSubclassOf<UGameplayEffect> DamageEffectClass = nullptr;
	UPROPERTY(BlueprintReadWrite)
	TWeakObjectPtr<UAbilitySystemComponent> SourceAbilityComponent{nullptr};
	UPROPERTY(BlueprintReadWrite)
	TWeakObjectPtr<UAbilitySystemComponent> TargetAbilityComponent{nullptr};
	
};