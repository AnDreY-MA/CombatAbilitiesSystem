// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Components/ActorComponent.h"
#include "CombatTraceComponent.generated.h"

DECLARE_STATS_GROUP(TEXT("CombatTraceComponent"), STATGROUP_CombatTraceComponent, STATCAT_Advanced);

UENUM(BlueprintType)
enum class ECombatCollisionShapeType : uint8
{
	Sphere = 0,
	Capsule,
	Box
};

UENUM(BlueprintType)
enum class ECombatCollisionStyleType : uint8
{
	Line = 0,
	Sweep
};

UCLASS(ClassGroup="CombatAbilitiesSystem", meta=(BlueprintSpawnableComponent) )
class COMBATABILITIESSYSTEM_API UCombatTraceComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	explicit UCombatTraceComponent(const FObjectInitializer& InInitializer = FObjectInitializer::Get());

    UFUNCTION(BlueprintCallable, Category="TraceComponent")
	void ActivateTrace();
	UFUNCTION(BlueprintCallable, Category="TraceComponent")
	void DeactivateTrace();

	UFUNCTION(BlueprintCallable, Category="TraceComponent")
	void ChangeTracingComponent(USceneComponent* InSceneComponent);

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditAnywhere, Category="Trace")
	FName NameSocketStart{FName()};

	UPROPERTY(EditAnywhere, Category="Trace")
	FName NameSocketEnd{FName()};

	UPROPERTY(EditDefaultsOnly, Category="Trace")
	TEnumAsByte<ECollisionChannel> TraceChannel{ECC_Visibility};

	UPROPERTY(EditDefaultsOnly, Category="Trace")
	float TraceInterval{0.5f};

	UPROPERTY(EditDefaultsOnly, Category="Trace")
	ECombatCollisionStyleType TraceStyleType{ECombatCollisionStyleType::Line};

	UPROPERTY(EditDefaultsOnly, Category="Trace|Sweep", meta=(EditCondition="TraceStyleType==ECombatCollisionStyleType::Sweep", EditConditionHides))
	ECombatCollisionShapeType ShapeType{ECombatCollisionShapeType::Capsule};

	UPROPERTY(EditDefaultsOnly, Category="Trace|Sweep", meta=(EditCondition="TraceStyleType==ECombatCollisionStyleType::Sweep, ShapeType==ECombatCollisionShapeType::Capsule", EditConditionHides))
	float CapsuleRadius;
	
	UPROPERTY(EditDefaultsOnly, Category="Trace|Sweep", meta=(EditCondition="TraceStyleType==ECombatCollisionStyleType::Sweep, ShapeType==ECombatCollisionShapeType::Capsule", EditConditionHides))
	float CapsuleHalfHeight;
	
	UPROPERTY(EditDefaultsOnly, Category="Trace|Event")
	FGameplayTag EventHitTag{FGameplayTag()};

#pragma region Debug

	UPROPERTY(EditDefaultsOnly, Category="Trace|Debug", meta=(DevelopmentOnly))
	bool bShouldLog{true};
	
	UPROPERTY(EditDefaultsOnly, Category="Trace|Debug", meta=(DevelopmentOnly))
	bool bShouldDebug{false};
	
	UPROPERTY(EditDefaultsOnly, Category="Trace|Debug", meta=(EditCondition="bShouldDebug", EditConditionHides, DevelopmentOnly))
	float LifeTimeDebugLine{0.5};
	
	UPROPERTY(EditDefaultsOnly, Category="Trace|Debug", meta=(EditCondition="bShouldDebug", EditConditionHides, DevelopmentOnly))
	FColor ColorDebug{FColor::Red};
#pragma endregion 
	
	UPROPERTY()
	TWeakObjectPtr<USceneComponent> TracingComponent;

	UPROPERTY()
	FTimerHandle TraceTimerHandle;

	FTraceDelegate TraceDelegate;

	void RunAsyncTaskStartTrace();
	void DoTrace() const;
	void OnTraceDelegate(const FTraceHandle& InTraceHandle, FTraceDatum& InTraceDatum);

#if !UE_BUILD_SHIPPING || WITH_EDITORONLY_DATA
	void TryDrawDebug(const FVector& InStart, const FVector& InEnd, const FQuat& InRot) const;
#endif
	
};