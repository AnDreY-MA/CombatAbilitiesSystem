// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DamageAbility.h"
#include "RangedAttackAbility.generated.h"

class ACombatProjectile;

UCLASS(Abstract)
class COMBATABILITIESSYSTEM_API URangedAttackAbility : public UDamageAbility
{
	GENERATED_BODY()

protected:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	virtual void OnEventReceived(FGameplayTag EventTag, FGameplayEventData EventData) override;

	UFUNCTION()
	void HitProjectile(AActor* SelfActor, AActor* OtherActor, FVector NormalImpulse, const FHitResult& Hit);
	UFUNCTION()
	void OnProjectileSpawn(ACombatProjectile* SpawnedProjectile);
	
	virtual void Shoot();

private:
	
	UPROPERTY(EditDefaultsOnly, Category="Combat|Events")
	FGameplayTag ShootEvent;
	
	UPROPERTY(EditDefaultsOnly, Category="Combat|RangedAttack")
	FName NameSocketFromShoot;
	UPROPERTY(EditDefaultsOnly, Category="Combat|RangedAttack")
	TSubclassOf<ACombatProjectile> ProjectileClass;
	
};