// Copyright (C) 2025 SynPlanet Holding.
#pragma once
#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"

DECLARE_LOG_CATEGORY_EXTERN( LogSynTerra, Log, All );

class FSynTerraRuntimeModule : public IModuleInterface
{
public:
	void StartupModule() override;
	void ShutdownModule() override;
};
