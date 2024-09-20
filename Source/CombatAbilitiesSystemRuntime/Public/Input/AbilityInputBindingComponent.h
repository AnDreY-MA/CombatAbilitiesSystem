// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "GameplayAbilitySpec.h"
#include "GameplayAbilitySpecHandle.h"
#include "Components/PlayerControlsComponent.h"
#include "AbilityInputBindingComponent.generated.h"

class UAbilitySystemComponent;

USTRUCT()
struct FAbilityInputBinding
{
	GENERATED_BODY()

	int32 InputID{0};
	uint32 OnPressedHandle{0};
	uint32 OnReleasedHandle{0};
	TArray<FGameplayAbilitySpecHandle> BoundAbilitiesStack;
	
};

UCLASS(ClassGroup="CombatAbilitiesSystem", meta=(BlueprintSpawnableComponent))
class COMBATABILITIESSYSTEM_API UAbilityInputBindingComponent : public UPlayerControlsComponent
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category="Abilities")
	void SetInputBinding(UInputAction* InInputAction, FGameplayAbilitySpecHandle AbilitySpecHandle);

	UFUNCTION(BlueprintCallable, Category="Abilities")
	void ClearInputBinding(FGameplayAbilitySpecHandle InAbilityHandle);

	UFUNCTION(BlueprintCallable, Category="Abilities")
	void ClearAbilityBindings(UInputAction* InInputAction);

	virtual void SetupPlayerControls_Implementation(UEnhancedInputComponent* InPlayerInputComponent) override;
	virtual void ReleaseInputComponent(AController* OldController) override;

private:
	void ResetBindings();
	void RunAbilitySystemSetup();
	void OnAbilityInputPressed(UInputAction* InInputAction);
	void OnAbilityInputReleased(UInputAction* InInputAction);

	void RemoveEntry(UInputAction* InInputAction);

	FGameplayAbilitySpec* FindAbilitySpec(FGameplayAbilitySpecHandle InHandle);
	void TryBindAbilityInput(UInputAction* InInputAction, FAbilityInputBinding& InAbilityInputBinding);
	
private:
	UPROPERTY(Transient)
	UAbilitySystemComponent* AbilityComponent;

	UPROPERTY(Transient)
	TMap<UInputAction*, FAbilityInputBinding> MappedAbilities;
	
};