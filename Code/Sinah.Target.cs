// Fill out your copyright notice in the Description page of Project Settings.

using UnrealBuildTool;
using System.Collections.Generic;

public class SinahTarget : TargetRules
{
	public SinahTarget(TargetInfo Target)
	{
		Type = TargetType.Game;
		ExtraModuleNames.AddRange( new string[] { "Sinah" } );
		
		//Steam
		bUsesSteam = true;
	}
}
