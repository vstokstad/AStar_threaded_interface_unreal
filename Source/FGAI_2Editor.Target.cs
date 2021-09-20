// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;
using System.Collections.Generic;

public class FGAI_2EditorTarget : TargetRules
{
	public FGAI_2EditorTarget( TargetInfo Target) : base(Target)
	{
		Type = TargetType.Editor;
		DefaultBuildSettings = BuildSettingsVersion.V2;


		ExtraModuleNames.AddRange( new string[] { "FGAI_2", "FGAI_2Editor" } );
	}
}
