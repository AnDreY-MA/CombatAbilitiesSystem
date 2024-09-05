// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Data/DamageData.h"
#include "GameFramework/Actor.h"
#include "CombatProjectile.generated.h"

class UProjectileMovementComponent;
class USphereComponent;
class UGameplayEffect;

UCLASS()
class COMBATABILITIESSYSTEMRUNTIME_API ACombatProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	explicit ACombatProjectile(const FObjectInitializer& InInitializer = FObjectInitializer::Get());

	void SetupProjectile(const FDamageEffectData& InDamageEffectData, const FVector& InDirection = FVector());

protected:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void OnProjectileHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
	
	virtual void BeginPlay() override;
	
private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Projectile", meta=(AllowPrivateAccess="true"))
	TObjectPtr<USphereComponent> SphereCollision;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Projectile", meta=(AllowPrivateAccess="true"))
	TObjectPtr<UProjectileMovementComponent> ProjectileMovement;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Projectile", meta=(AllowPrivateAccess="true"))
	FDamageEffectData DamageEffectData;
	
};