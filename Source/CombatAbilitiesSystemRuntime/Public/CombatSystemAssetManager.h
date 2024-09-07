// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Engine/AssetManager.h"
#include "CombatSystemAssetManager.generated.h"

struct FCombatGameplayTags
{
public:
	static const FCombatGameplayTags& Get();

	static void InitializeNativeGameplayTags();

	// Input Tags
	FGameplayTag CombatInput_Attack;
	FGameplayTag CombatInput_Dodge;
	FGameplayTag CombatInput_Fire;
	
private:
	static FCombatGameplayTags CombatTags;
	
};

UCLASS()
class COMBATABILITIESSYSTEM_API UCombatSystemAssetManager : public UAssetManager
{
	GENERATED_BODY()

public:
	static UCombatSystemAssetManager& Get();

protected:
	virtual void StartInitialLoading() override;
	
};
