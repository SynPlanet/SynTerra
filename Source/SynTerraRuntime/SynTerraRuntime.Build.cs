// Copyright (C) 2025 SynPlanet Holding.
using System;
using System.IO;
using UnrealBuildTool;
using UnrealBuildTool.Rules;

public class SynTerraRuntime : ModuleRules
{
    public SynTerraRuntime(ReadOnlyTargetRules Target) : base(Target)
    {
        PublicIncludePaths.AddRange(
            new string[] {
            }
        );


        PrivateIncludePaths.AddRange(
            new string[]
            {
                Path.Combine(ModuleDirectory, "../ThirdParty/include")
            }
        );


        // LIBs
        var libs = Directory.GetFiles(Path.Combine(ModuleDirectory, "../ThirdParty/lib"), "*.lib", SearchOption.AllDirectories);
        foreach (string lib in libs)
        {
            PublicAdditionalLibraries.Add(lib);
        }

        // DLLs
        string binary_dir = Path.Combine(PluginDirectory, "Binaries", "Win64");
        var dlls = Directory.GetFiles(Path.Combine(ModuleDirectory, "../ThirdParty/bin"), "*.dll", SearchOption.AllDirectories);
        foreach (string dll in dlls)
        {
            RuntimeDependencies.Add(Path.Combine(binary_dir, Path.GetFileName(dll)), dll, StagedFileType.NonUFS);
        }

        // GDAL_DATA
        string gdal_data_dir = Path.Combine(PluginDirectory, "Binaries", "Win64/share", "gdal");
        if (!Directory.Exists(gdal_data_dir))
        {
            Directory.CreateDirectory(gdal_data_dir);
        }

        var gdal_data_files = Directory.GetFiles(Path.Combine(ModuleDirectory, "../ThirdParty/share", "gdal"), "*", SearchOption.AllDirectories);
        foreach (string file in gdal_data_files)
        {
            RuntimeDependencies.Add(Path.Combine(gdal_data_dir, Path.GetFileName(file)), file, StagedFileType.NonUFS);
        }

        // PROJ_LIB
        string proj_data_dir = Path.Combine(PluginDirectory, "Binaries", "Win64/share", "proj");
        if (!Directory.Exists(proj_data_dir))
        {
            Directory.CreateDirectory(proj_data_dir);
        }

        var proj_data_files = Directory.GetFiles(Path.Combine(ModuleDirectory, "../ThirdParty/share", "proj"), "*", SearchOption.AllDirectories);
        foreach (string file in proj_data_files)
        {
            RuntimeDependencies.Add(Path.Combine(proj_data_dir, Path.GetFileName(file)), file, StagedFileType.NonUFS);
        }


        PublicDefinitions.AddRange(
            new string[]
            {
                "GLM_FORCE_XYZW_ONLY",
                "GLM_FORCE_EXPLICIT_CTOR",
                "GLM_FORCE_SIZE_T_LENGTH",

                "BOOST_ALL_NO_LIB",
                "BOOST_UUID_FORCE_AUTO_LINK"
            }
        );


        PublicDependencyModuleNames.AddRange(
            new string[]
            {
                "Core",
                "Projects",
                "CoreUObject",
                "Engine",
            }
        );


        PrivateDependencyModuleNames.AddRange(
            new string[]
            {
                "UnrealEd",
                "RawMesh",
                "AssetRegistry",
                "AssetTools",
                "ImageWrapper",
                "EditorScriptingUtilities",
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
