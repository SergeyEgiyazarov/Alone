// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class Alone : ModuleRules
{
	public Alone(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PublicDependencyModuleNames.AddRange(new string[] { 
			"Core",
			"CoreUObject",
			"Engine",
			"InputCore",
			"Niagara",
			"PhysicsCore",
			"GameplayTasks",
			"NavigationSystem"
		});

		PrivateDependencyModuleNames.AddRange(new string[] {  });

		PublicIncludePaths.AddRange(new string[] { 
			"Alone/Public/Player", 
			"Alone/Public/Components", 
			"Alone/Public/Weapon",
			"Alone/Public/UI",
			"Alone/Public/Animations",
			"Alone/Public/Pickups",
			"Alone/Public/Weapon/Components",
			"Alone/Public/AI",
			"Alone/Public/AI/Tasks",
			"Alone/Public/AI/Services",
			"Alone/Public/AI/EQS"
		});

		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });
		
		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}
