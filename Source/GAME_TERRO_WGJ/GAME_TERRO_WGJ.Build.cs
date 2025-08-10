// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class GAME_TERRO_WGJ : ModuleRules
{
	public GAME_TERRO_WGJ(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] {
            "Core",
            "CoreUObject",
            "Engine",
            "InputCore",
            "HeadMountedDisplay",
            "AnimationCore",
            "AnimGraphRuntime",
            "UMG",         
			"Slate",       
			"SlateCore"    
		});
	}
}
