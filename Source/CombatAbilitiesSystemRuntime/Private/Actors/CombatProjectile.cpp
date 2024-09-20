// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/CombatProjectile.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystemGlobals.h"
#include "CombatSystemFunctionLibrary.h"
#include "Components/SphereComponent.h"
#include "Engine/DamageEvents.h"
#include "GameFramework/ProjectileMovementComponent.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(CombatProjectile)


ACombatProjectile::ACombatProjectile(const FObjectInitializer& InInitializer) :
	Super(InInitializer)
{
	PrimaryActorTick.bCanEverTick = false;

	SphereCollision = CreateDefaultSubobject<USphereComponent>("Collision");
	SetRootComponent(SphereCollision);

	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>("ProjectileMovement");
	ProjectileMovement->SetUpdatedComponent(SphereCollision);
}

void ACombatProjectile::BeginPlay()
{
	Super::BeginPlay();

	SphereCollision->OnComponentHit.AddDynamic(this, &ACombatProjectile::OnProjectileHit);
	SphereCollision->IgnoreActorWhenMoving(this, true);
	SphereCollision->IgnoreActorWhenMoving(GetOwner(), true);
	
}

void ACombatProjectile::SetupProjectile(const FDamageEffectData& InHitEffect, const FVector& InDirection)
{
	DamageEffectData = InHitEffect;
}

void ACombatProjectile::OnProjectileHit_Implementation(UPrimitiveComponent* HitComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if(auto* AbilityComponent {UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(OtherActor)}; AbilityComponent)
	{
		DamageEffectData.TargetAbilityComponent = AbilityComponent;
		UCombatSystemFunctionLibrary::ApplyDamage(DamageEffectData);
	}
	
	const FDamageEvent DamageEvent;
	OtherActor->TakeDamage(1.f, DamageEvent, GetOwner()->GetInstigatorController(), GetOwner());
	

}