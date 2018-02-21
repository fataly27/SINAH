// Fill out your copyright notice in the Description page of Project Settings.

using UnrealBuildTool;
using System.Collections.Generic;

public class SinahEditorTarget : TargetRules
{
	public SinahEditorTarget(TargetInfo Target)
	{
		Type = TargetType.Editor;
		ExtraModuleNames.AddRange( new string[] { "Sinah" } );
	}
}
