// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class CephedekiCevher : ModuleRules
{
	public CephedekiCevher(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay" ,"Slate", "SlateCore" ,"UMG","AIModule"});
	}
}
