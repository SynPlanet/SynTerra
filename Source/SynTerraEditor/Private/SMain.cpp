// Copyright (C) 2025 SynPlanet Holding.
#include "SMain.h"
#include "SUserInfo.h"
#include "SCSMode.h"
#include "SNumberOfTiles.h"
#include "SMapViewport.h"
#include "Controller.h"

#define LOCTEXT_NAMESPACE "Main"

void SMain::Construct( const FArguments& InArgs )
{
	_Controller = MakeShared<Controller>();

	ChildSlot
		[
			SNew( SHorizontalBox )
				+ SHorizontalBox::Slot()
				.AutoWidth()
				.MaxWidth( 450.0f )
				[
					SNew( SVerticalBox )
						+ SVerticalBox::Slot()
						.AutoHeight()
						.Padding( 2.0f )
						[
							SNew( SUserInfo )
								.UserInfoAttr_Lambda( [&] { return _Controller->GetUserInfo(); } )
						]

						+ SVerticalBox::Slot()
						.AutoHeight()
						.Padding( 2.0f )
						[
							SNew( SCSMode )
								.CSModeAttr_Lambda( [&] { return _Controller->GetCSMode(); } )
								.OnCSModeChanged_Lambda( [&]( CSMode Mode ) { _Controller->SetCSMode( Mode ); } )
						]

						+ SVerticalBox::Slot()
						.AutoHeight()
						.Padding( 2.0f )
						[
							SNew( SNumberOfTiles )
								.NumberOfTiles_Lambda( [&] { return _Controller->GetNumberOfTiles(); } )
						]

						+ SVerticalBox::Slot()
						.AutoHeight()
						.Padding( 2.0f )
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
											SNew( SCheckBox )
												.IsChecked_Lambda(
													[&] { return _Controller->GetIsNeedSpawnTiles() ? ECheckBoxState::Checked : ECheckBoxState::Unchecked; } )
												.OnCheckStateChanged_Lambda(
													[&]( ECheckBoxState NewState ) {
														_Controller->SetIsNeedSpawnTiles( NewState == ECheckBoxState::Checked );
													} )
										]

										+ SHorizontalBox::Slot()
										.AutoWidth()
										.Padding( 4, 0, 0, 0 )
										.VAlign( VAlign_Center )
										[
											SNew( STextBlock )
												.Font( FAppStyle::Get().GetFontStyle( "NormalFontBold" ) )
												.Text( LOCTEXT( "IsNeedSpawnTiles.Title", "Automatically add tiles to scene" ) )
										]
								]
						]

						+ SVerticalBox::Slot()
						.AutoHeight()
						.Padding( 2.0f )
						[
							SNew( SHorizontalBox )
								+ SHorizontalBox::Slot()
								.AutoWidth()
								[
									SNew( SButton )
										.ButtonStyle( &FAppStyle::Get().GetWidgetStyle< FButtonStyle >( "PrimaryButton" ) )
										.Text( LOCTEXT( "GenerateTiles.Title", "Generate tiles (.uasset)" ) )
										.IsEnabled_Lambda(
											[&] {
												const auto& Nums = _Controller->GetNumberOfTiles();
												int CurrentLimit = _Controller->GetUserInfo().CurrenLimit;
												return (Nums.IsSet() && (CurrentLimit > 0 || CurrentLimit == -1));
											} )
										.OnClicked_Lambda(
											[&]() {
												_Controller->GenerateTiles();
												return FReply::Handled();
											} )
								]
						]

						+ SVerticalBox::Slot()
						.VAlign( VAlign_Center )
						.HAlign( HAlign_Center )
						.Padding( 5, 5, 5, 5 )
						.AutoHeight()
						[
							SNew( STextBlock )
								.Text_Lambda( [&] { return FText::FromString( _Controller->GetMessage() ); } )
						]
				]

				+ SHorizontalBox::Slot()
				.FillWidth( 1.0f )
				[
					SNew( SMapViewport )
						.Tiles_Lambda( [&] { return _Controller->GetMapTiles(); } )
						.OnCurrentMapGeoPosition_Lambda(
							[&]( double Lon, double Lat ) {
								_Controller->OnCheckActiveMapTiles( Lon, Lat );
							} )
				]
		];
}

#undef LOCTEXT_NAMESPACE
