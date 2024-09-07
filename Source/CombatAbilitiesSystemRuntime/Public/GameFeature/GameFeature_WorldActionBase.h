// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFeatureAction.h"
#include "GameFeature_WorldActionBase.generated.h"

/**
 * 
 */
UCLASS()
class COMBATABILITIESSYSTEM_API UGameFeature_WorldActionBase : public UGameFeatureAction
{
	GENERATED_BODY()

public:
	virtual void OnGameFeatureActivating() override;
	virtual void OnGameFeatureDeactivating(FGameFeatureDeactivatingContext& Context) override;

private:
	void HandleGameInstanceStart(UGameInstance* InGameInstance);
	virtual void AddToWorld(const FWorldContext& InWorldContext) PURE_VIRTUAL(UGameFeature_WorldActionBase::AddToWorld, );
	FDelegateHandle GameInstanceStartHandle;
};
