// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "AttributeSet.h"
#include "CombatAttributeSet.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnHealthChanged, float, NewValue, float, OldValue, float, Magintude);

#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

UCLASS(Blueprintable, BlueprintType)
class COMBATABILITIESSYSTEMRUNTIME_API UCombatAttributeSet : public UAttributeSet
{
	GENERATED_BODY()

public:
	ATTRIBUTE_ACCESSORS(UCombatAttributeSet, Health);
	
	ATTRIBUTE_ACCESSORS(UCombatAttributeSet, Strength);
	
	ATTRIBUTE_ACCESSORS(UCombatAttributeSet, Damage);

	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;

	virtual void PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data) override;

private:
	UPROPERTY(BlueprintReadOnly, Category="Health", meta=(AllowPrivateAccess="true"))
	FGameplayAttributeData Health;

	UPROPERTY(BlueprintReadOnly, Category="Strength", meta=(AllowPrivateAccess="true"))
	FGameplayAttributeData Strength;

	UPROPERTY(BlueprintReadOnly, Category="Strength", meta=(AllowPrivateAccess="true"))
	FGameplayAttributeData Damage;
	
};