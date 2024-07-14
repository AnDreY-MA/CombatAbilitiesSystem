// Copyright Epic Games, Inc. All Rights Reserved.

#include "GameFeature/GameFeature_WorldActionBase.h"
#include "GameFeaturesSubsystemSettings.h"
#include "Engine/World.h" // for FWorldDelegates::OnStartGameInstance
#include "Engine/Engine.h" // for FWorldContext

void UGameFeature_WorldActionBase::OnGameFeatureActivating()
{
	GameInstanceStartHandle = FWorldDelegates::OnStartGameInstance.AddUObject(this, &UGameFeature_WorldActionBase::HandleGameInstanceStart);

	// Add to any worlds with associated game instances that have already been initialized
	for (const FWorldContext& WorldContext : GEngine->GetWorldContexts())
	{
		AddToWorld(WorldContext);
	}
}

void UGameFeature_WorldActionBase::OnGameFeatureDeactivating(FGameFeatureDeactivatingContext& Context)
{
	FWorldDelegates::OnStartGameInstance.Remove(GameInstanceStartHandle);
}

void UGameFeature_WorldActionBase::HandleGameInstanceStart(UGameInstance* GameInstance)
{
	if (FWorldContext* WorldContext = GameInstance->GetWorldContext())
	{
		AddToWorld(*WorldContext);
	}
}