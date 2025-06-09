// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class IU_Jumpking : ModuleRules
{
	public IU_Jumpking(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] {
			"Core",
			"CoreUObject",
			"Engine",
			"InputCore",
			"HeadMountedDisplay",
			"EnhancedInput",
			"Niagara",
			"UMG"
		});
	}
}
