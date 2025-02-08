// Copyright (C) 2025 SynPlanet Holding.
#include "SynTerraStyle.h"
#include "Styling/SlateStyleRegistry.h"
#include "Framework/Application/SlateApplication.h"
#include "Slate/SlateGameResources.h"
#include "Interfaces/IPluginManager.h"
#include "Styling/SlateStyleMacros.h"

#define RootToContentDir Style->RootToContentDir

TSharedPtr<FSlateStyleSet> FSynTerraStyle::StyleInstance = nullptr;

void FSynTerraStyle::Initialize()
{
	if (!StyleInstance.IsValid())
	{
		StyleInstance = Create();
		FSlateStyleRegistry::RegisterSlateStyle(*StyleInstance);
	}
}

void FSynTerraStyle::Shutdown()
{
	FSlateStyleRegistry::UnRegisterSlateStyle(*StyleInstance);
	ensure(StyleInstance.IsUnique());
	StyleInstance.Reset();
}

FName FSynTerraStyle::GetStyleSetName()
{
	static FName StyleSetName(TEXT("SynTerraStyle"));
	return StyleSetName;
}

TSharedRef<FSlateStyleSet> FSynTerraStyle::Create()
{
	const FVector2D Icon20x20( 20.0f, 20.0f );
	const FVector2D Icon128x128( 128.0f, 128.0f );

	TSharedRef<FSlateStyleSet> Style = MakeShareable( new FSlateStyleSet( "SynTerraStyle" ) );
	Style->SetContentRoot( IPluginManager::Get().FindPlugin( "SynTerra" )->GetBaseDir() / TEXT( "Resources" ) );

	Style->Set( "SynTerra.Common.OpenSynTerraWindow", new IMAGE_BRUSH_SVG( TEXT( "PlaceholderButtonIcon" ), Icon20x20 ) );
	Style->Set( "SynTerra.Common.Auth", new IMAGE_BRUSH( TEXT( "Icon128" ), Icon128x128 ) );

	return Style;
}

void FSynTerraStyle::ReloadTextures()
{
	if (FSlateApplication::IsInitialized())
	{
		FSlateApplication::Get().GetRenderer()->ReloadTextureResources();
	}
}

const ISlateStyle& FSynTerraStyle::Get()
{
	return *StyleInstance;
}
