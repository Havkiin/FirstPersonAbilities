// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class AreWeBackReloaded : ModuleRules
{
	public AreWeBackReloaded(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput", "Niagara" });
	}
}
