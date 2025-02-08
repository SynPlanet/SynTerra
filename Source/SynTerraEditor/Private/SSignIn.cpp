// Copyright (C) 2025 SynPlanet Holding.
#include "SSignIn.h"
#include "Widgets/SBoxPanel.h"
#include "Widgets/Text/STextBlock.h"
#include "Widgets/Input/SHyperlink.h"
#include "SynTerraRuntime.h"
#include "ControllerUtils.h"

#define LOCTEXT_NAMESPACE "SignIn"

void SSignIn::Construct( const FArguments& InArgs )
{
	OnNavigate = InArgs._OnNavigate;
	OnSignInSuccessful = InArgs._OnSignInSuccessful;

	ChildSlot
		[
			SNew( SVerticalBox )
				+ SVerticalBox::Slot()
				.VAlign( VAlign_Center )
				.HAlign( HAlign_Fill )
				.Padding( 5 )
				.AutoHeight()
				[
					SNew( SEditableTextBox )
						.IsPassword_Lambda( [&] { return false; } )
						.RevertTextOnEscape( true )
						.HintText( LOCTEXT( "SignIn.EMail", "EMail" ) )
						.OnTextCommitted_Lambda(
							[&]( const FText& Val, ETextCommit::Type TextCommitType ) {
								Message = "";
								IsValidEmail = false;
								EMail = Val.ToString();
								if ( EMail.IsEmpty() ) {
									Message = "EMail is empty";
								}
								else if ( !ControllerUtils::IsValidEMail( EMail ) ) {
									Message = "EMail is not valid";
								}
								else {
									IsValidEmail = true;
								}
							} )
				]
				+ SVerticalBox::Slot()
				.VAlign( VAlign_Center )
				.HAlign( HAlign_Fill )
				.Padding( 5 )
				.AutoHeight()
				[
					SNew( SEditableTextBox )
						.IsPassword_Lambda( [&] { return true; } )
						.RevertTextOnEscape( true )
						.HintText( LOCTEXT( "SignIn.Password", "Password" ) )
						.OnTextCommitted_Lambda(
							[&]( const FText& Val, ETextCommit::Type TextCommitType ) {
								Message = "";
								IsValidPassword = false;
								Password = Val.ToString();
								if ( Password.IsEmpty() )
									Message = "Password is empty";
								else if ( Password.Len() < 6 )
									Message = "Password length less than 6 characters";
								else
									IsValidPassword = true;
							} )
				]

				+ SVerticalBox::Slot()
				.VAlign( VAlign_Center )
				.HAlign( HAlign_Fill )
				.Padding( 5 )
				.AutoHeight()
				[
					SNew( SButton )
						.HAlign( HAlign_Center )
						.VAlign( VAlign_Center )
						.ButtonStyle( &FAppStyle::Get().GetWidgetStyle< FButtonStyle >( "PrimaryButton" ) )
						.Text( FText::FromString( "Sign In" ) )
						.IsEnabled_Lambda(
							[&] {
								return IsValidEmail && IsValidPassword && Message.IsEmpty();
							} )
						.OnClicked_Lambda(
							[&]() {
								FString Resp = ControllerUtils::SendSignIn( EMail, Password );
								if ( Resp.IsEmpty() )
									OnSignInSuccessful.ExecuteIfBound();
								else
									Message = Resp;

								return FReply::Handled();
							} )
				]

				+ SVerticalBox::Slot()
				.VAlign( VAlign_Center )
				.HAlign( HAlign_Center )
				.Padding( 5, 5, 5, 5 )
				.AutoHeight()
				[
					SNew( STextBlock )
						.Text_Lambda( [&] { return FText::FromString( Message ); } )
				]

				+ SVerticalBox::Slot()
				.VAlign( VAlign_Center )
				.HAlign( HAlign_Center )
				.Padding( 5, 20, 5, 5 )
				.AutoHeight()
				[
					SNew( STextBlock )
						.Text( FText::FromString( TEXT( "Don't have an account?" ) ) )
				]

				+ SVerticalBox::Slot()
				.VAlign( VAlign_Center )
				.HAlign( HAlign_Center )
				.Padding( 5 )
				.AutoHeight()
				[
					SNew( SHyperlink )
						.OnNavigate_Lambda( [&] { OnNavigate.ExecuteIfBound(); } )
						.Text( FText::FromString( TEXT( "Sign Up" ) ) )
				]
		];
}

#undef LOCTEXT_NAMESPACE
