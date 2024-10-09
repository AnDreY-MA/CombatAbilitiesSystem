// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/CombatTraceComponent.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "CollisionDebugDrawingPublic.h"
#include "CombatAbilitiesSystemModule.h"
#include "Kismet/KismetSystemLibrary.h"


DECLARE_CYCLE_STAT(TEXT("DoTrace"), STAT_DoTraceTest, STATGROUP_CombatTraceComponent);
DECLARE_CYCLE_STAT(TEXT("RunStartTrace"), STAT_RunStartTrace, STATGROUP_CombatTraceComponent);

namespace
{
	
}

UCombatTraceComponent::UCombatTraceComponent(const FObjectInitializer& InInitializer)
	: Super(InInitializer), TraceType(ECombatTraceType::SINGLE)
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UCombatTraceComponent::BeginPlay()
{	
	Super::BeginPlay();
	
	TraceDelegate.BindUObject(this, &UCombatTraceComponent::OnTraceDelegate);
}

void UCombatTraceComponent::RunAsyncTaskStartTrace()
{
	SCOPE_CYCLE_COUNTER(STAT_RunStartTrace);

	GetWorld()->GetTimerManager().SetTimer(TraceTimerHandle, this, &UCombatTraceComponent::DoTrace, TraceInterval, true);
}


void UCombatTraceComponent::ActivateTrace()
{
	check(TracingComponent != nullptr);

	RunAsyncTaskStartTrace();
	
}

void UCombatTraceComponent::DeactivateTrace()
{
	GetWorld()->GetTimerManager().ClearTimer(TraceTimerHandle);
	
}

void UCombatTraceComponent::ChangeTracingComponent(USceneComponent* InSceneComponent)
{
	if(InSceneComponent)
	{
		TracingComponent = InSceneComponent;
	}
	
}

void UCombatTraceComponent::DoTrace() const
{
	SCOPE_CYCLE_COUNTER(STAT_DoTraceTest);

	auto* World {GetWorld()};
	const FVector Start{TracingComponent->GetSocketLocation(NameSocketStart)};
	const FVector End{TracingComponent->GetSocketLocation(NameSocketEnd)};
	const FQuat Rot = TracingComponent->GetSocketQuaternion(NameSocketStart);

	FCollisionQueryParams CollisionQueryParams;
	CollisionQueryParams.AddIgnoredActor(GetOwner());

#if !UE_BUILD_SHIPPING || WITH_EDITOR
	TryDrawDebug(Start, End, Rot);
#endif

	
	if(TraceStyleType == ECombatCollisionStyleType::Line)
	{
		EAsyncTraceType Type = TraceType == ECombatTraceType::SINGLE ? EAsyncTraceType::Single : EAsyncTraceType::Multi;
		World->AsyncLineTraceByChannel(Type, Start, End, TraceChannel, CollisionQueryParams, FCollisionResponseParams::DefaultResponseParam, &TraceDelegate);
	}
	else if(TraceStyleType == ECombatCollisionStyleType::Sweep)
	{
		DoTraceShape(Start, End, Rot, CollisionQueryParams);
	}
	
}

void UCombatTraceComponent::DoTraceShape(const FVector& Start, const FVector& End, const FQuat& Rot, FCollisionQueryParams& CollisionQueryParams) const
{
	FCollisionShape Shape = GetShape();
	
	EAsyncTraceType Type = TraceType == ECombatTraceType::SINGLE ? EAsyncTraceType::Single : EAsyncTraceType::Multi;
	GetWorld()->AsyncSweepByChannel(Type, Start, End, Rot, TraceChannel, Shape, CollisionQueryParams, FCollisionResponseParams::DefaultResponseParam, &TraceDelegate);

}

void UCombatTraceComponent::OnTraceDelegate(const FTraceHandle& InTraceHandle, FTraceDatum& InTraceDatum)
{
	if(!InTraceDatum.OutHits.Num()) return;
	
	for(auto HitResult : InTraceDatum.OutHits)
	{
		auto* HitActor {HitResult.GetActor()};
		if(!HitActor) continue;
			
		FGameplayEventData Payload;
		Payload.Instigator = GetOwner();
		Payload.Target = HitActor;
		Payload.TargetData = UAbilitySystemBlueprintLibrary::AbilityTargetDataFromActor(HitActor);
		UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(GetOwner(), EventHitTag, Payload);
	}
	
}

#if !UE_BUILD_SHIPPING || WITH_EDITOR
void UCombatTraceComponent::TryDrawDebug(const FVector& InStart, const FVector& InEnd, const FQuat& InRot) const
{
	if (!bShouldDebug) return;

	if(TraceStyleType == ECombatCollisionStyleType::Line)
	{
		DrawDebugLine(GetWorld(), InStart, InEnd, ColorDebug, false, LifeTimeDebug);
	}
	else if(TraceStyleType == ECombatCollisionStyleType::Sweep)
	{
		FCollisionShape Shape = GetShape();

		if(ShapeData.ShapeType == ECombatCollisionShapeType::Capsule)
		{
			DrawCapsuleSweeps(GetWorld(), InStart, InEnd, Shape.GetCapsuleHalfHeight(), Shape.GetCapsuleRadius(), InRot, {}, LifeTimeDebug);
		}
		else if(ShapeData.ShapeType == ECombatCollisionShapeType::Sphere)
		{
			DrawSphereSweeps(GetWorld(), InStart, InEnd, Shape.GetSphereRadius(), {}, LifeTimeDebug);
		}
		else if (ShapeData.ShapeType == ECombatCollisionShapeType::Box)
		{
			DrawBoxSweeps(GetWorld(), InStart, InEnd, Shape.GetExtent(), InRot, {}, LifeTimeDebug);
		}
	}
}
# endif