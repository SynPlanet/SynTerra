// Copyright (C) 2025 SynPlanet Holding.
#pragma once
#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"

class SLoginRegister : public SCompoundWidget
{
	DECLARE_DELEGATE( FOnLoginSuccessful )
public:
	SLATE_BEGIN_ARGS( SLoginRegister ) {}
		SLATE_EVENT( FOnLoginSuccessful, OnLoginSuccessful )
	SLATE_END_ARGS()

	virtual void Construct( const FArguments& InArgs );
	virtual ~SLoginRegister() override {}

private:
	FOnLoginSuccessful OnLoginSuccessful;

private:
	bool IsVisibleRegister = false;
};
