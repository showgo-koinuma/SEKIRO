// Fill out your copyright notice in the Description page of Project Settings.

using UnrealBuildTool;

public class SEKIRO : ModuleRules
{
	public SEKIRO(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "GameplayAbilities", "GameplayTags", "EnhancedInput" });

		PrivateDependencyModuleNames.AddRange(new string[] {  });
	}
}
