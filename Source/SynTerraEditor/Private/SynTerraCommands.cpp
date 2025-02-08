// Copyright (C) 2025 SynPlanet Holding.
#include "SynTerraCommands.h"
#include "SynTerraStyle.h"

#define LOCTEXT_NAMESPACE "SynTerraCommands"

FSynTerraCommands::FSynTerraCommands()
	: TCommands<FSynTerraCommands>(
		"SynTerra.Common",
		LOCTEXT( "Common", "Common" ),
		NAME_None,
		FSynTerraStyle::GetStyleSetName() )
{
}


void FSynTerraCommands::RegisterCommands()
{
	UI_COMMAND(
		OpenSynTerraWindow,
		"SynTerra",
		"Open the SynTerra window",
		EUserInterfaceActionType::Button,
		FInputChord()
	);
}

#undef LOCTEXT_NAMESPACE
