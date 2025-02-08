// Copyright (C) 2025 SynPlanet Holding.
#include "SUserInfo.h"
#include "Widgets/Input/SNumericEntryBox.h"

#define LOCTEXT_NAMESPACE "UserInfo"

void SUserInfo::Construct( const FArguments& InArgs )
{
	UserInfoAttr = InArgs._UserInfoAttr;

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
										.Text( LOCTEXT( "UserInfo.UserName.Title:", "User name:" ) )
								]

								+ SHorizontalBox::Slot()
								.AutoWidth()
								.Padding( 4, 0, 0, 0 )
								.VAlign( VAlign_Center )
								[
									SNew( STextBlock )
										.Font( FAppStyle::Get().GetFontStyle( "SmallFontBold" ) )
										.Text_Lambda(
											[&] {
												const auto& UserName = UserInfoAttr.Get().UserName;
												return UserName.IsEmpty() ? FText::FromString( "" ) :
													FText::FromString( UserName );
											} )
								]
						]
				]

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
										.Text( LOCTEXT( "UserInfo.EMail.Title:", "EMail:" ) )
								]

								+ SHorizontalBox::Slot()
								.AutoWidth()
								.Padding( 4, 0, 0, 0 )
								.VAlign( VAlign_Center )
								[
									SNew( STextBlock )
										.Font( FAppStyle::Get().GetFontStyle( "SmallFontBold" ) )
										.Text_Lambda(
											[&] {
												const auto& EMail = UserInfoAttr.Get().EMail;
												return  EMail.IsEmpty() ? FText::FromString( "" ) :
													FText::FromString( EMail );
											} )
								]
						]
				]

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
										.Text( LOCTEXT( "UserInfo.Title.CurrenLimit:", "Current limit:" ) )
								]

								+ SHorizontalBox::Slot()
								.Padding( 10, 0, 0, 0 )
								.FillWidth( 1.0f )
								.VAlign( VAlign_Center )
								[
									SNew( SNumericEntryBox<int> )
										.IsEnabled( false )
										.Value_Lambda(
											[&] {
												const auto& CurrentLimit = UserInfoAttr.Get().CurrenLimit;
												return CurrentLimit == -1 ? TOptional<int>() : CurrentLimit;
											}
										)
								]
						]
				]
		];
}

#undef LOCTEXT_NAMESPACE
