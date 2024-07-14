// Copyright Epic Games, Inc. All Rights Reserved.

#include "GameFeature/AddInputContextMapping.h"
#include "CombatAbilitiesSystemRuntime/Public/CombatAbilitiesSystemRuntimeModule.h"
#include "Engine/AssetManager.h"
#include "GameFeaturesSubsystemSettings.h"
#include "Components/GameFrameworkComponentManager.h"
#include "GameFramework/PlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"

#define LOCTEXT_NAMESPACE "CombatAbilitiesSystemRuntimeFeatures"

//////////////////////////////////////////////////////////////////////
// UGameFeatureAction_AddInputContextMapping

void UAddInputContextMapping::OnGameFeatureActivating()
{
	if (!ensure(ExtensionRequestHandles.IsEmpty()) ||
		!ensure(ControllersAddedTo.IsEmpty()))
	{
		Reset();
	}
	Super::OnGameFeatureActivating();
}

void UAddInputContextMapping::OnGameFeatureDeactivating(FGameFeatureDeactivatingContext& Context)
{
	Super::OnGameFeatureDeactivating(Context);
	Reset();
}

#if WITH_EDITORONLY_DATA
void UAddInputContextMapping::AddAdditionalAssetBundleData(FAssetBundleData& AssetBundleData)
{
	if (UAssetManager::IsValid())
	{
		AssetBundleData.AddBundleAsset(UGameFeaturesSubsystemSettings::LoadStateClient, InputMapping.ToSoftObjectPath());
		AssetBundleData.AddBundleAsset(UGameFeaturesSubsystemSettings::LoadStateServer, InputMapping.ToSoftObjectPath());
	}
}
#endif

#if WITH_EDITOR
EDataValidationResult UAddInputContextMapping::IsDataValid(TArray<FText>& ValidationErrors)
{
	EDataValidationResult Result = CombineDataValidationResults(Super::IsDataValid(ValidationErrors), EDataValidationResult::Valid);

	if (InputMapping.IsNull())
	{
		Result = EDataValidationResult::Invalid;
		ValidationErrors.Add(LOCTEXT("NullInputMapping", "Null InputMapping."));
	}

	return Result;
}
#endif

void UAddInputContextMapping::AddToWorld(const FWorldContext& WorldContext)
{
	UWorld* World = WorldContext.World();
	UGameInstance* GameInstance = WorldContext.OwningGameInstance;

	if ((GameInstance != nullptr) && (World != nullptr) && World->IsGameWorld())
	{
		if (UGameFrameworkComponentManager* ComponentMan = UGameInstance::GetSubsystem<UGameFrameworkComponentManager>(GameInstance))
		{
			if (!InputMapping.IsNull())
			{
				UGameFrameworkComponentManager::FExtensionHandlerDelegate AddAbilitiesDelegate = UGameFrameworkComponentManager::FExtensionHandlerDelegate::CreateUObject(
					this, &UAddInputContextMapping::HandleControllerExtension);
				TSharedPtr<FComponentRequestHandle> ExtensionRequestHandle = ComponentMan->AddExtensionHandler(APlayerController::StaticClass(), AddAbilitiesDelegate);

				ExtensionRequestHandles.Add(ExtensionRequestHandle);
			}
		}
	}
}

void UAddInputContextMapping::Reset()
{
	ExtensionRequestHandles.Empty();

	while (!ControllersAddedTo.IsEmpty())
	{
		TWeakObjectPtr<APlayerController> ControllerPtr = ControllersAddedTo.Top();
		if (ControllerPtr.IsValid())
		{
			RemoveInputMapping(ControllerPtr.Get());
		}
		else
		{
			ControllersAddedTo.Pop();
		}
	}
}

void UAddInputContextMapping::HandleControllerExtension(AActor* Actor, FName EventName)
{
	APlayerController* AsController = CastChecked<APlayerController>(Actor);

	if (EventName == UGameFrameworkComponentManager::NAME_ExtensionRemoved || EventName == UGameFrameworkComponentManager::NAME_ReceiverRemoved)
	{
		RemoveInputMapping(AsController);
	}
	else if (EventName == UGameFrameworkComponentManager::NAME_ExtensionAdded || EventName == UGameFrameworkComponentManager::NAME_GameActorReady)
	{
		AddInputMappingForPlayer(AsController->GetLocalPlayer());
	}
}

void UAddInputContextMapping::AddInputMappingForPlayer(UPlayer* Player)
{
	if (ULocalPlayer* LocalPlayer = Cast<ULocalPlayer>(Player))
	{
		if (UEnhancedInputLocalPlayerSubsystem* InputSystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
		{
			if (!InputMapping.IsNull())
			{
				InputSystem->AddMappingContext(InputMapping.LoadSynchronous(), Priority);
			}
		}
		else
		{
			UE_LOG(LogCombatAbilitySystem, Error, TEXT("Failed to find `UEnhancedInputLocalPlayerSubsystem` for local player. Input mappings will not be added. Make sure you're set to use the EnhancedInput system via config file."));
		}
	}
}

void UAddInputContextMapping::RemoveInputMapping(APlayerController* PlayerController)
{
	if (ULocalPlayer* LocalPlayer = PlayerController->GetLocalPlayer())
	{
		if (UEnhancedInputLocalPlayerSubsystem* InputSystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
		{
			InputSystem->RemoveMappingContext(InputMapping.Get());
		}
	}

	ControllersAddedTo.Remove(PlayerController);
}

#undef LOCTEXT_NAMESPACE