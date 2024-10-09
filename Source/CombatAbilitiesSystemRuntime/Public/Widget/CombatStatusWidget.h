// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CombatStatusWidget.generated.h"

class UCombatUIComponent;

UCLASS(Abstract)
class COMBATABILITIESSYSTEM_API UCombatStatusWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	void InitWidgetFromActor(const AActor* InActor);

protected:
	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "On CombatUI Initialized"))
	void BP_OnCombatUIComponentInitialize(UCombatUIComponent* InCombatUIComponent);
};