// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/CombatWarpingComponent.h"

#include "Interfaces/TargetWarpInterface.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(CombatWarpingComponent)


UCombatWarpingComponent::UCombatWarpingComponent(const FObjectInitializer& InInitializer)
	: Super(InInitializer), BoneName(FName("root")), LengthWarp(200.f), RadiusSphere(50.f), DebugTraceType(EDrawDebugTrace::None)
{

}

void UCombatWarpingComponent::UpdateWarpTarget()
{
	AActor* AvatarOwner{GetOwner()};
	const FVector Start{AvatarOwner->GetActorLocation()};
	const FVector End{Start + AvatarOwner->GetActorForwardVector() * LengthWarp};
	FHitResult HitResult;

	const TArray<AActor*> ActorsIgnore = {AvatarOwner};
	if(UKismetSystemLibrary::SphereTraceSingleForObjects(GetWorld(), Start, End, RadiusSphere, TraceObjectTypes, false, ActorsIgnore, DebugTraceType, HitResult, true))
	{
		TryWarpToTarget(HitResult.GetActor());
	}

}

void UCombatWarpingComponent::ResetWarpTarget()
{
	RemoveWarpTarget(WarpTargetName);
	
}

void UCombatWarpingComponent::TryWarpToTarget(AActor* InTarget)
{
	const FRotator TargetRotation = UKismetMathLibrary::FindLookAtRotation(GetOwner()->GetActorLocation(), InTarget->GetActorLocation() - InTarget->GetActorForwardVector());

	FMotionWarpingTarget WarpingTarget{ FMotionWarpingTarget() };
	WarpingTarget.Name = WarpTargetName;
	WarpingTarget.Rotation = FRotator(0.0, 0.0, TargetRotation.Roll);
	WarpingTarget.BoneName = BoneName;

	if(InTarget->Implements<UTargetWarpInterface>())
	{
		USceneComponent* SnapComponent {nullptr};
		ITargetWarpInterface::Execute_GetDirection(InTarget, GetOwner()->GetActorLocation(), SnapComponent);

		WarpingTarget.Location = SnapComponent->GetComponentLocation();
	}
	
	AddOrUpdateWarpTarget(WarpingTarget);

}