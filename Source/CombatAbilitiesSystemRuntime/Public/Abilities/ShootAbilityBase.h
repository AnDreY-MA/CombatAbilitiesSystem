// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/CombatAbilityBase.h"
#include "ShootAbilityBase.generated.h"

class UCharacterMovementComponent;
/**
 * 
 */
UCLASS()
class COMBATABILITIESSYSTEMRUNTIME_API UShootAbilityBase : public UCombatAbilityBase
{
	GENERATED_BODY()

public:
	explicit UShootAbilityBase(const FObjectInitializer& InInitializer = FObjectInitializer::Get());

protected:
	
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	                             const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	virtual void InputReleased(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo) override;

private:
	UFUNCTION()
	void OnReleased(float TimeHeld);

private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Shooting", meta=(AllowPrivateAccess="true"))
	FName NameSocketFromShoot;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Shooting", meta=(AllowPrivateAccess="true"))
	TSubclassOf<AActor> ProjectileActorClass;

	UPROPERTY()
	float MovementSpeed;
	
	UPROPERTY(Transient)
	TObjectPtr<UCharacterMovementComponent> CharacterMovementComponent;
	
};