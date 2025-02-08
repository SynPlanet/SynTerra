// Copyright (C) 2025 SynPlanet Holding.
using System;
using System.IO;
using UnrealBuildTool;
using UnrealBuildTool.Rules;

public class SynTerraEditor : ModuleRules
{
    public SynTerraEditor(ReadOnlyTargetRules Target) : base(Target)
    {
        PublicIncludePaths.AddRange(
            new string[] {
            }
        );


        PrivateIncludePaths.AddRange(
            new string[] {
            }
        );

        PublicDefinitions.AddRange(
            new string[] {
            }
        );


        PublicDependencyModuleNames.AddRange(
            new string[]
            {
                "Core",
                "SynTerraRuntime",
            }
        );


        PrivateDependencyModuleNames.AddRange(
            new string[]
            {
                "Projects",
                "InputCore",
                "EditorFramework",
                "UnrealEd",
                "ToolMenus",
                "CoreUObject",
                "Engine",
                "Slate",
                "SlateCore",
                "EditorStyle",
                "ApplicationCore",
            }
        );


        DynamicallyLoadedModuleNames.AddRange(
            new string[] {
            }
        );

        PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;
        CppStandard = CppStandardVersion.Cpp20;
    }
}
