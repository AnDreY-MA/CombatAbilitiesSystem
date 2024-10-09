// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CombatUIComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAttributeChanged, const float, InValue);


UCLASS( ClassGroup=("CombatAbilitiesSystem"), meta = (BlueprintSpawnableComponent))
class COMBATABILITIESSYSTEM_API UCombatUIComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	explicit UCombatUIComponent();

	UPROPERTY(BlueprintAssignable)
	FOnAttributeChanged OnHealthChanged;

protected:
	virtual void BeginPlay() override;

};