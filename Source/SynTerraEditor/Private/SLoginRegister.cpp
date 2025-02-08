// Copyright (C) 2025 SynPlanet Holding.
#include "SLoginRegister.h"
#include "SSignIn.h"
#include "SSignUp.h"
#include "SynTerraStyle.h"
#include "Widgets/Layout/SScaleBox.h"

#define LOCTEXT_NAMESPACE "LoginRegister"

void SLoginRegister::Construct( const FArguments& InArgs )
{
	OnLoginSuccessful = InArgs._OnLoginSuccessful;

	ChildSlot
		[
			SNew( SBorder )
				.Padding( 30 )
				.BorderImage( FAppStyle::GetBrush( "ToolPanel.GroupBorder" ) )
				[
					SNew( SScrollBox )
						+ SScrollBox::Slot()
						.VAlign( VAlign_Top )
						.HAlign( HAlign_Center )
						.Padding( 20, 0, 20, 5 )
						[
							SNew( SScaleBox )
								.Stretch( EStretch::ScaleToFit )
								.HAlign( HAlign_Center )
								.VAlign( VAlign_Top )
								[
									SNew( SImage )
										.Image( FSynTerraStyle::Get().GetBrush( TEXT( "SynTerra.Common.Auth" ) ) )
								]
						]
						+ SScrollBox::Slot()
						.VAlign( VAlign_Center )
						.HAlign( HAlign_Fill )
						.Padding( 20, 0, 20, 5 )
						[
							SNew( SSignIn )
								.Visibility_Lambda( [&] { return !IsVisibleRegister ? EVisibility::Visible : EVisibility::Collapsed; } )
								.OnNavigate_Lambda( [&] { IsVisibleRegister = true; } )
								.OnSignInSuccessful_Lambda( [&] { OnLoginSuccessful.ExecuteIfBound(); } )
						]
						+ SScrollBox::Slot()
						.VAlign( VAlign_Center )
						.HAlign( HAlign_Fill )
						.Padding( 20, 0, 20, 5 )
						[
							SNew( SSignUp )
								.Visibility_Lambda( [&] { return IsVisibleRegister ? EVisibility::Visible : EVisibility::Collapsed; } )
								.OnNavigate_Lambda( [&] { IsVisibleRegister = false; } )
						]
				]
		];
}

#undef LOCTEXT_NAMESPACE
