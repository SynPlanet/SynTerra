// Copyright (C) 2025 SynPlanet Holding.
#include "SCSMode.h"

#define LOCTEXT_NAMESPACE "SCSMode"

void SCSMode::Construct( const FArguments& InArgs )
{
	CSModeAttr = InArgs._CSModeAttr;
	OnCSModeChanged = InArgs._OnCSModeChanged;

	ChildSlot
		[
			SNew( SVerticalBox )
				+ SVerticalBox::Slot()
				.AutoHeight()
				.Padding( 0.0f, 20.0f, 2.0f, 2.0f )
				[
					SNew( SBorder )
						.Padding( FMargin( 3.0f ) )
						.BorderImage( FAppStyle::GetBrush( "ToolPanel.GroupBorder" ) )
						[
							SNew( SVerticalBox )
								+ SVerticalBox::Slot()
								.AutoHeight()
								[
									SNew( SHorizontalBox )
										+ SHorizontalBox::Slot()
										.AutoWidth()
										.Padding( 4, 0, 0, 0 )
										.VAlign( VAlign_Center )
										[
											SNew( SCheckBox )
												.Style( FAppStyle::Get(), "RadioButton" )
												.IsChecked_Lambda(
													[&]() { return CSModeAttr.Get() == CSMode::Geodetic ? ECheckBoxState::Checked : ECheckBoxState::Unchecked; } )
												.OnCheckStateChanged_Lambda(
													[&]( ECheckBoxState NewState ) {
														if ( NewState == ECheckBoxState::Checked )
															OnCSModeChanged.ExecuteIfBound( CSMode::Geodetic );
													} )
										]
										+ SHorizontalBox::Slot()
										.AutoWidth()
										.Padding( 4, 0, 0, 0 )
										.VAlign( VAlign_Center )
										[
											SNew( STextBlock )
												.Font( FAppStyle::Get().GetFontStyle( "NormalFontBold" ) )
												.Text( LOCTEXT( "CSMode.Geodetic.Title", "Geodetic" ) )
										]

								]

								+ SVerticalBox::Slot()
								.AutoHeight()
								.Padding( 0.0f, 5.0f, 0.0f, 0.0f )
								[
									SNew( SHorizontalBox )
										+ SHorizontalBox::Slot()
										.AutoWidth()
										.Padding( 4, 0, 0, 0 )
										.VAlign( VAlign_Center )
										[
											SNew( SCheckBox )
												.Style( FAppStyle::Get(), "RadioButton" )
												.IsChecked_Lambda(
													[&]() { return CSModeAttr.Get() == CSMode::Local ? ECheckBoxState::Checked : ECheckBoxState::Unchecked; } )
												.OnCheckStateChanged_Lambda(
													[&]( ECheckBoxState NewState ) {
														if ( NewState == ECheckBoxState::Checked )
															OnCSModeChanged.ExecuteIfBound( CSMode::Local );
													} )
										]
										+ SHorizontalBox::Slot()
										.AutoWidth()
										.Padding( 4, 0, 0, 0 )
										.VAlign( VAlign_Center )
										[
											SNew( STextBlock )
												.Font( FAppStyle::Get().GetFontStyle( "NormalFontBold" ) )
												.Text( LOCTEXT( "CSMode.Local.Title", "Local" ) )
										]

								]
						]
				]
		];
}

#undef LOCTEXT_NAMESPACE
