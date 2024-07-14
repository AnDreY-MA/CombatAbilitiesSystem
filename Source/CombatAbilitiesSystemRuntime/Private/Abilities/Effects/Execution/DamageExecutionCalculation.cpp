// Fill out your copyright notice in the Description page of Project Settings.


#include "Abilities/Effects/Execution/DamageExecutionCalculation.h"

#include "CombatAbilitiesSystemRuntimeModule.h"
#include "Abilities/Attributes/CombatAttributeSet.h"

void UDamageExecutionCalculation::Execute_Implementation(
	const FGameplayEffectCustomExecutionParameters& ExecutionParams,
	FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	const UAbilitySystemComponent* TargetAbilityComponent{ExecutionParams.GetTargetAbilitySystemComponent()};
	const UAbilitySystemComponent* SourceAbilityComponent{ExecutionParams.GetSourceAbilitySystemComponent()};
	const AActor* TargetActor{TargetAbilityComponent->GetAvatarActor()};
	const AActor* SourceActor{SourceAbilityComponent->GetAvatarActor()};

	const FGameplayEffectSpec& Spec {ExecutionParams.GetOwningSpec()};
	const FGameplayTagContainer* SourceTags{Spec.CapturedSourceTags.GetAggregatedTags()};
	const FGameplayTagContainer* TargetTags{Spec.CapturedTargetTags.GetAggregatedTags()};

	FAggregatorEvaluateParameters EvaluateParameters;
	EvaluateParameters.SourceTags = SourceTags;
	EvaluateParameters.TargetTags = TargetTags;

	const float DamageDone{SourceAbilityComponent->GetNumericAttribute(UCombatAttributeSet::GetStrengthAttribute())};

	UE_LOG(LogCombatAbilitySystem, Display, TEXT("Damage Owner=%s, Done=%f, Causer=%s"), *TargetActor->GetName(), DamageDone, *SourceActor->GetName());

	const FGameplayModifierEvaluatedData EvaluatedData{FGameplayModifierEvaluatedData(UCombatAttributeSet::GetHealthAttribute(), EGameplayModOp::Additive, -DamageDone)};
	OutExecutionOutput.AddOutputModifier(EvaluatedData);

}