// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DataAbilitySystemComponent.h"
#include "CombatComponentInterface.h"
#include "CombatSystemComponent.generated.h"


UCLASS(meta=(BlueprintSpawnableComponent))
class COMBATABILITIESSYSTEMRUNTIME_API UCombatSystemComponent : public UDataAbilitySystemComponent, public ICombatComponentInterface
{
	GENERATED_BODY()

public:

	explicit UCombatSystemComponent(const FObjectInitializer& InInitializer = FObjectInitializer::Get());

	virtual void AbilityLocalInputPressed(int32 InputID) override;
	
	// Begin ICombatComponentInterface
	virtual TArray<FCombatAnimationInfo> GetMontageAction_Implementation(const FGameplayTag& InTagName) const override;

	virtual FCombatAnimationInfo GetComboMontageAction_Implementation(const FGameplayTag& InTagName) override;

	virtual UGameplayAbility* GetCurrentActiveComboAbility_Implementation() const override;

	virtual void IncrementComboIndex_Implementation() override;

	virtual void RequestTriggerCombo_Implementation() override;

	virtual void ActivateNextCombo_Implementation() override;
	
	virtual void ResetCombo_Implementation() override;

	virtual bool IsOpenComboWindow_Implementation() const override;
	
	virtual bool IsActiveNextCombo_Implementation() const override;

	virtual bool IsShouldTriggerCombo_Implementation() const override;

	virtual bool IsRequestTriggerCombo_Implementation() const override;
	
	virtual void OpenComboWindow_Implementation() override;

	virtual void CloseComboWindow_Implementation() override;
	// End ICombatComponentInterface

private:
	void ActivateComboAbility(const TSubclassOf<UGameplayAbility> InAbilityClass);

private:
	UPROPERTY(EditDefaultsOnly, Category="Abilities|Anims")
	TObjectPtr<UDataTable> CombatActionTable;
	
	UPROPERTY()
	int32 ComboIndex;

	UPROPERTY()
	bool bWindowComboAttack;
	UPROPERTY()
	bool bRequestTriggerCombo;
	UPROPERTY()
	bool bNextComboAbilityActivated;
	UPROPERTY()
	bool bShouldTriggerCombo;
	
};