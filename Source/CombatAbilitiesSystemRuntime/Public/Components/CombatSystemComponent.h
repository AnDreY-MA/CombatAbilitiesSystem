// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DataAbilitySystemComponent.h"
#include "CombatComponentInterface.h"
#include "CombatSystemComponent.generated.h"

/**
 * 
 */
UCLASS(meta=(BlueprintSpawnableComponent))
class COMBATABILITIESSYSTEMRUNTIME_API UCombatSystemComponent : public UDataAbilitySystemComponent, public ICombatComponentInterface
{
	GENERATED_BODY()

public:

	explicit UCombatSystemComponent();

	virtual void AbilityLocalInputPressed(int32 InputID) override;
	
	// Begin ICombatComponentInterface
	virtual UAnimMontage* GetCurrentMontage_Implementation() override;

	virtual UAnimMontage* GetDodgeMontage_Implementation() override;

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
	TObjectPtr<UDataTable> CombatActionTable{nullptr};

	UPROPERTY(EditDefaultsOnly, Category="Abilities|Anims")
	TObjectPtr<UAnimMontage> DodgeMontage{nullptr};

	UPROPERTY()
	int32 ComboIndex{0};

	UPROPERTY()
	bool bWindowComboAttack{false};
	UPROPERTY()
	bool bRequestTriggerCombo{false};
	UPROPERTY()
	bool bNextComboAbilityActivated{false};
	UPROPERTY()
	bool bShouldTriggerCombo{false};

	UPROPERTY()
	TSubclassOf<UGameplayAbility> MeleeAttackAbilityClass;
};