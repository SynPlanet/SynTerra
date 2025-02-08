// Copyright (C) 2025 SynPlanet Holding.
#pragma once
#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"

class SSignIn : public SCompoundWidget
{
	DECLARE_DELEGATE( FOnNavigate )
	DECLARE_DELEGATE( FOnSignInSuccessful )
public:
	SLATE_BEGIN_ARGS( SSignIn ) {}
		SLATE_EVENT( FOnNavigate, OnNavigate )
		SLATE_EVENT( FOnSignInSuccessful, OnSignInSuccessful )
	SLATE_END_ARGS()

	virtual void Construct( const FArguments& InArgs );
	virtual ~SSignIn() override {}

private:
	FOnNavigate OnNavigate;
	FOnSignInSuccessful OnSignInSuccessful;

private:
	FString EMail;
	FString Password;
	FString Message;
	bool IsValidEmail = false;
	bool IsValidPassword = false;
};
