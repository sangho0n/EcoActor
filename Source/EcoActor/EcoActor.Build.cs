// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class EcoActor : ModuleRules
{
	public EcoActor(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay", "UMG", "NavigationSystem" ,"AIModule", "GameplayTasks" });
		
		PrivateDependencyModuleNames.AddRange(new string[] {
		"Slate",
		"SlateCore"
		});
	}
}
