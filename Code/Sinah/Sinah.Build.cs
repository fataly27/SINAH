// Fill out your copyright notice in the Description page of Project Settings.

using UnrealBuildTool;

public class Sinah : ModuleRules
{
	public Sinah(ReadOnlyTargetRules ROTargetRules) : base(ROTargetRules)
	{
		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "RHI", "RenderCore", "AIModule", "OggAsset" });

		// Uncomment if you are using Slate UI
		PrivateDependencyModuleNames.AddRange(new string[] { "UMG", "Slate", "SlateCore" });
		
		// Uncomment if you are using online features
		PublicDependencyModuleNames.AddRange(new string[] {"OnlineSubsystem", "OnlineSubsystemUtils"});
		DynamicallyLoadedModuleNames.Add("OnlineSubsystemSteam");
	}
}
