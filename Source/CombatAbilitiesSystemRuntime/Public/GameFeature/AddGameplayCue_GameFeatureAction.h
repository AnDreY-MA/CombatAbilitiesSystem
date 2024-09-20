// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFeature_WorldActionBase.h"
#include "AddGameplayCue_GameFeatureAction.generated.h"

/**
 * 
 */
UCLASS(ClassGroup="CombatAbilitiesSystem", DisplayName="Add Gameplay Cue Path")
class COMBATABILITIESSYSTEM_API UAddGameplayCue_GameFeatureAction final : public UGameFeatureAction
{
	GENERATED_BODY()

public:
	explicit UAddGameplayCue_GameFeatureAction(const FObjectInitializer& InInitializer = FObjectInitializer::Get());

#if WITH_EDITOR
	virtual EDataValidationResult IsDataValid(FDataValidationContext& Context) override;
#endif

	const TArray<FDirectoryPath>& GetDirectoryPathsToAdd() const { return DirectoryPaths;}
	
private:
	UPROPERTY(EditAnywhere, Category="GameplayCue", meta=(RelativeToGameContentDir, LongPackageName))
	TArray<FDirectoryPath> DirectoryPaths;
	
};