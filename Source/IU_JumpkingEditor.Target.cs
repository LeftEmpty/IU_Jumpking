// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;
using System.Collections.Generic;

public class IU_JumpkingEditorTarget : TargetRules
{
	public IU_JumpkingEditorTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Editor;
		DefaultBuildSettings = BuildSettingsVersion.V5;
        IncludeOrderVersion = EngineIncludeOrderVersion.Latest;
        CppStandard = CppStandardVersion.Cpp20;
        ExtraModuleNames.Add("IU_Jumpking");
	}
}
