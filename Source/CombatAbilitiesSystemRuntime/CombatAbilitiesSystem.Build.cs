// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class CombatAbilitiesSystem : ModuleRules
{
	public CombatAbilitiesSystem(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;	
		
		PublicDependencyModuleNames.AddRange(
			new string[]
			{
				"Core", "Engine",
				"MotionWarping", "AIModule", "GameplayAbilities", "Niagara", "NiagaraCore", "UMG"
			}
			);
			
		
		PrivateDependencyModuleNames.AddRange(
			new string[]
			{
				"CoreUObject",
				"Engine",
				"Slate",
				"SlateCore",
				"GameFeatures",
				"GameplayTags", 
				"GameplayTasks",
				"EnhancedInput",
				"ModularGameplay"
			}
		);
        
	}
}
