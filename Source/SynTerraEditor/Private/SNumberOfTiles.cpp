// Copyright (C) 2025 SynPlanet Holding.
#include "SNumberOfTiles.h"
#include "Widgets/SBoxPanel.h"
#include "Widgets/Input/SNumericEntryBox.h"
#include "Styling/AppStyle.h"

#define LOCTEXT_NAMESPACE "NumberOfTiles"


void SNumberOfTiles::Construct( const FArguments& InArgs )
{
	NumberOfTiles = InArgs._NumberOfTiles;

	ChildSlot
		[
			SNew( SVerticalBox )
				+ SVerticalBox::Slot()
				.AutoHeight()
				[
					SNew( SBorder )
						.Padding( FMargin( 3.0f ) )
						.BorderImage( FAppStyle::GetBrush( "ToolPanel.GroupBorder" ) )
						[
							SNew( SHorizontalBox )
								+ SHorizontalBox::Slot()
								.AutoWidth()
								.Padding( 4, 0, 0, 0 )
								.VAlign( VAlign_Center )
								[
									SNew( STextBlock )
										.Font( FAppStyle::Get().GetFontStyle( "NormalFontBold" ) )
										.Text( LOCTEXT( "NumberOfTiles.Title", "Number of tiles:" ) )
								]

								+ SHorizontalBox::Slot()
								.Padding( 10, 0, 0, 0 )
								.FillWidth( 1.0f )
								.VAlign( VAlign_Center )
								[
									SNew( SNumericEntryBox<int> )
										.IsEnabled( false )
										.Value_Lambda( [&] { return NumberOfTiles.Get(); } )
								]
						]
				]
		];
}

#undef LOCTEXT_NAMESPACE
