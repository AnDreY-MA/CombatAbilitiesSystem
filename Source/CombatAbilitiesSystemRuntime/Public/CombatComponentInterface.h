// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Data/CombatActionData.h"
#include "UObject/Interface.h"
#include "CombatComponentInterface.generated.h"

class UGameplayAbility;
// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UCombatComponentInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class COMBATABILITIESSYSTEMRUNTIME_API ICombatComponentInterface	
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="Combat Component")
	TArray<FCombatAnimationInfo> GetMontageAction(const FGameplayTag& InTagName) const;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="Combat Component")
	FCombatAnimationInfo GetComboMontageAction(const FGameplayTag& InTagName);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="Combat Component")
	void IncrementComboIndex();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="Combat Component")
	UGameplayAbility* GetCurrentActiveComboAbility() const;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="Combat Component")
	void ActivateNextCombo();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="Combat Component")
	bool IsActiveNextCombo() const;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="Combat Component")
	void ResetCombo();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="Combat Component")
	void RequestTriggerCombo();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="Combat Component")
	bool IsOpenComboWindow() const;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="Combat Component")
	bool IsShouldTriggerCombo() const;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="Combat Component")
	bool IsRequestTriggerCombo() const;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="Combat Component")
	void OpenComboWindow();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="Combat Component")
	void CloseComboWindow();
	
};