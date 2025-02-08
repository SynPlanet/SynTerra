// Copyright (C) 2025 SynPlanet Holding.
#include "SSignUp.h"
#include "Widgets/SBoxPanel.h"
#include "Widgets/Text/STextBlock.h"
#include "Widgets/Input/SHyperlink.h"
#include "SynTerraRuntime.h"
#include "ControllerUtils.h"

#define LOCTEXT_NAMESPACE "SignUp"

void SSignUp::Construct( const FArguments& InArgs )
{
	OnNavigate = InArgs._OnNavigate;

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
						.HintText( LOCTEXT( "SignIn.UserName", "User name" ) )
						.OnTextCommitted_Lambda(
							[&]( const FText& Val, ETextCommit::Type TextCommitType ) {
								Message = "";
								IsValidUserName = false;
								UserName = Val.ToString();
								if ( UserName.IsEmpty() ) {
									Message = "User name is empty";
								}
								else {
									IsValidUserName = true;
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
								Password = Val.ToString();
								CheckPassword();
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
						.HintText( LOCTEXT( "SignIn.ConfirmPassword", "Confirm password" ) )
						.OnTextCommitted_Lambda(
							[&]( const FText& Val, ETextCommit::Type TextCommitType ) {
								ConfirmPassword = Val.ToString();
								CheckPassword();
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
						.Text( FText::FromString( "Create account" ) )
						.IsEnabled_Lambda( [&] { return IsValidInput(); } )
						.OnClicked_Lambda(
							[&]() {
								FString Resp = ControllerUtils::SendSignUp( UserName, EMail, Password );
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
						.Text( FText::FromString( TEXT( "Already have an account?" ) ) )
				]
				+ SVerticalBox::Slot()
				.VAlign( VAlign_Center )
				.HAlign( HAlign_Center )
				.Padding( 5 )
				.AutoHeight()
				[
					SNew( SHyperlink )
						.OnNavigate_Lambda( [&] { OnNavigate.ExecuteIfBound(); } )
						.Text( FText::FromString( TEXT( "Sign In" ) ) )
				]
		];
}

void SSignUp::CheckPassword()
{
	Message = "";
	IsValidPassword = false;
	if ( Password.IsEmpty() )
		Message = "Password is empty";
	else if ( Password.Len() < 6 )
		Message = "Password length less than 6 characters";
	else if ( Password != ConfirmPassword )
		Message = "Passwords don't match";
	else
		IsValidPassword = true;
}

bool SSignUp::IsValidInput() const
{
	return IsValidUserName && IsValidEmail && IsValidPassword &&
		Message.IsEmpty();
}

#undef LOCTEXT_NAMESPACE
