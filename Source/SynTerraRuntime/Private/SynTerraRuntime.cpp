// Copyright (C) 2025 SynPlanet Holding.
#include "SynTerraRuntime.h"
#include "Logging/StructuredLog.h"
#include "Interfaces/IPluginManager.h"

THIRD_PARTY_INCLUDES_START
#include <cpl_error.h>
#include <stp_utils.h>
THIRD_PARTY_INCLUDES_END

#define LOCTEXT_NAMESPACE "SynTerraRuntimeModule"

DEFINE_LOG_CATEGORY( LogSynTerra );

void FSynTerraRuntimeModule::StartupModule()
{
	CPLSetErrorHandler(
		[]( CPLErr, int, const char* message ) {
			UE_LOGFMT( LogSynTerra, Error, "{1}", message );
		}
	);

	FString PluginBaseDir = IPluginManager::Get().FindPlugin( "SynTerra" )->GetBaseDir();
	FString GDALDataDir = FPaths::ConvertRelativePathToFull(
		FPaths::Combine( PluginBaseDir, TEXT( "Binaries" ), TEXT( "Win64" ), TEXT( "share" ), TEXT( "gdal" ) ) );
	FString PROJDataDir = FPaths::ConvertRelativePathToFull(
		FPaths::Combine( PluginBaseDir, TEXT( "Binaries" ), TEXT( "Win64" ), TEXT( "share" ), TEXT( "proj" ) ) );

	set_option_GDAL( "GDAL_DATA", TCHAR_TO_UTF8( *GDALDataDir ) );
	set_search_path_PROJ( TCHAR_TO_UTF8( *PROJDataDir ) );

	init_GDAL();

	FString VersionGDAL( get_version_GDAL() );
	UE_LOGFMT( LogSynTerra, Display, "{1}", VersionGDAL );

	FString PathResources = PluginBaseDir + TEXT( "/Resources" );
	set_path_resources( TCHAR_TO_UTF8( *PathResources ) );

	init_NetSys();
}

void FSynTerraRuntimeModule::ShutdownModule()
{
	destroy_GDAL();
	destroy_NetSys();
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE( FSynTerraRuntimeModule, SynTerraRuntime )
