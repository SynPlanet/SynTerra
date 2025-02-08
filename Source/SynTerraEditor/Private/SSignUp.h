// Copyright (C) 2025 SynPlanet Holding.
#pragma once
#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"

class SSignUp : public SCompoundWidget
{
	DECLARE_DELEGATE( FOnNavigate )
public:
	SLATE_BEGIN_ARGS( SSignUp ) {}
		SLATE_EVENT( FOnNavigate, OnNavigate )
	SLATE_END_ARGS()

	virtual void Construct( const FArguments& InArgs );
	virtual ~SSignUp() override {}

private:
	FOnNavigate OnNavigate;

private:
	FString UserName;
	FString EMail;
	FString Password;
	FString ConfirmPassword;
	FString Message;
	bool IsValidUserName = false;
	bool IsValidEmail = false;
	bool IsValidPassword = false;

private:
	void CheckPassword();
	bool IsValidInput() const;
};
