// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFeature/GameFeature_WorldActionBase.h"
#include "AddInputContextMapping.generated.h"

class UInputMappingContext;
struct FComponentRequestHandle;
/**
 * 
 */
UCLASS()
class COMBATABILITIESSYSTEMRUNTIME_API UAddInputContextMapping : public UGameFeature_WorldActionBase
{
	GENERATED_BODY()

public:

	virtual void OnGameFeatureActivating() override;
	virtual void OnGameFeatureDeactivating(FGameFeatureDeactivatingContext& Context) override;

#if WITH_EDITORONLY_DATA
	virtual void AddAdditionalAssetBundleData(FAssetBundleData& AssetBundleData) override;
#endif

#if WITH_EDITOR
	virtual EDataValidationResult IsDataValid(TArray<FText>& ValidationErrors) override;
#endif
	

private:
	virtual void AddToWorld(const FWorldContext& InWorldContext) override;

	void Reset();
	void HandleControllerExtension(AActor* InActor, FName EventName);
	void AddInputMappingForPlayer(UPlayer* InPlayer);
	void RemoveInputMapping(APlayerController* InPlayerController);
	
private:
	UPROPERTY(EditAnywhere, Category="Input", meta=(AllowPrivateAccess="true"))
	TSoftObjectPtr<UInputMappingContext> InputMapping;
	UPROPERTY(EditAnywhere, Category="Input", meta=(AllowPrivateAccess="true"))
	int32 Priority{0};
	
	TArray<TSharedPtr<FComponentRequestHandle>> ExtensionRequestHandles;
	TArray<TWeakObjectPtr<APlayerController>> ControllersAddedTo;
};