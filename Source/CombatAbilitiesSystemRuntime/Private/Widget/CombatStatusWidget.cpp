// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/CombatStatusWidget.h"
#include <Interfaces/CombatUIComponentInterface.h>
#include "Components/CombatUIComponent.h"
#include "CombatAbilitiesSystemModule.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(CombatStatusWidget)

void UCombatStatusWidget::InitWidgetFromActor(const AActor* InActor)
{
	if (InActor->Implements<UCombatUIComponentInterface>())
	{
		UCombatUIComponent* UIComponent = ICombatUIComponentInterface::Execute_GetCombatUIComponent(InActor);
		BP_OnCombatUIComponentInitialize(UIComponent);
	}
	else
	{
		UE_LOG(LogCombatAbilitySystem, Warning, TEXT("%s doesn' implements UCombatUIComponentInterface"), *InActor->GetName());
	}
}