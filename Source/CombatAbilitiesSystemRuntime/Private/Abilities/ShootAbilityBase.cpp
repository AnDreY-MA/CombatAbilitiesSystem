// Fill out your copyright notice in the Description page of Project Settings.


#include "Abilities/ShootAbilityBase.h"

#include "CombatAbilitiesSystemRuntimeModule.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Abilities/Tasks/AbilityTask_WaitInputRelease.h"
#include "GameFramework/MovementComponent.h"

UShootAbilityBase::UShootAbilityBase(const FObjectInitializer& InInitializer)
	: Super(InInitializer), ProjectileActorClass(nullptr), MovementSpeed(0)
{
}

void UShootAbilityBase::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	const AActor* AvatarActor{ActorInfo->AvatarActor.Get()};
	CharacterMovementComponent  = AvatarActor->FindComponentByClass<UCharacterMovementComponent>();
	MovementSpeed = CharacterMovementComponent->GetMaxSpeed();
	CharacterMovementComponent->MaxWalkSpeed = 0;

	/*auto* ReleaseTask {UAbilityTask_WaitInputRelease::WaitInputRelease(this, true)};
	ReleaseTask->OnRelease.AddDynamic(this, &UShootAbilityBase::OnReleased);
	ReleaseTask->ReadyForActivation();*/
	

}

void UShootAbilityBase::InputReleased(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo)
{
	OnReleased(0);
}

void UShootAbilityBase::OnReleased(float TimeHeld)
{
	auto* AvatarActor{CurrentActorInfo->AvatarActor.Get()};
	const auto* ComponentFromSpawn {AvatarActor->FindComponentByClass<USkeletalMeshComponent>()};
	const FVector SocketSpawnLocation {ComponentFromSpawn->GetSocketLocation(NameSocketFromShoot)};
	FTransform ProjectileTransform(FRotator(AvatarActor->GetActorRotation().Pitch, AvatarActor->GetActorRotation().Yaw, AvatarActor->GetActorRotation().Roll),
		FVector(SocketSpawnLocation.X + 50.0, SocketSpawnLocation.Y, SocketSpawnLocation.Z), FVector(1.0, 1.0, 1.0));
	ProjectileTransform.SetLocation(FVector(SocketSpawnLocation.X + 50.0, SocketSpawnLocation.Y, SocketSpawnLocation.Z));
	auto* ProjectileActor{GetWorld()->SpawnActorDeferred<AActor>(ProjectileActorClass, ProjectileTransform, AvatarActor, AvatarActor->GetInstigator())};
	ProjectileActor->FinishSpawning(ProjectileTransform);

	CharacterMovementComponent->MaxWalkSpeed = MovementSpeed;


	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, false, false);
}
