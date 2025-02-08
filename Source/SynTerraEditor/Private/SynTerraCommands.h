// Copyright (C) 2025 SynPlanet Holding.
#pragma once
#include "CoreMinimal.h"
#include "Framework/Commands/Commands.h"


class FSynTerraCommands : public TCommands<FSynTerraCommands>
{
public:
	FSynTerraCommands();

	void RegisterCommands() override;

public:
	TSharedPtr<FUICommandInfo> OpenSynTerraWindow;
};
