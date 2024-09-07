// Fill out your copyright notice in the Description page of Project Settings.


#include "CombatSystemAssetManager.h"

#include "CombatAbilitiesSystemModule.h"

#include "GameplayTagsManager.h"

FCombatGameplayTags FCombatGameplayTags::CombatTags;

const FCombatGameplayTags& FCombatGameplayTags::Get()
{
	return CombatTags;
}

void FCombatGameplayTags::InitializeNativeGameplayTags()
{
	UGameplayTagsManager& TagsManager = UGameplayTagsManager::Get();
	CombatTags.CombatInput_Attack = TagsManager.AddNativeGameplayTag(
		FName("CombatSystem.Abilities.Attack"));
	CombatTags.CombatInput_Dodge = TagsManager.AddNativeGameplayTag(
		FName("CombatSystem.Abilities.Dodge"));
	CombatTags.CombatInput_Fire = TagsManager.AddNativeGameplayTag(
		FName("CombatSystem.Abilities.Fire"));
		
}

UCombatSystemAssetManager& UCombatSystemAssetManager::Get()
{
	if(auto* Singleton{Cast<UCombatSystemAssetManager>(GEngine->AssetManager)}; Singleton)
	{
		return *Singleton;
	}

	UE_LOG(LogCombatAbilitySystem, Fatal, TEXT("Invalid AssetManagerClassName in DefaultEngine"));

	return *NewObject<UCombatSystemAssetManager>();
}

void UCombatSystemAssetManager::StartInitialLoading()
{
	Super::StartInitialLoading();

	FCombatGameplayTags::InitializeNativeGameplayTags();
}
