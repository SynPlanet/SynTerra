// Copyright (C) 2025 SynPlanet Holding.
#pragma once
#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"
#include "FCSMode.h"

class SCSMode : public SCompoundWidget
{
	DECLARE_DELEGATE_OneParam( FOnCSModeChanged, CSMode );
public:
	SLATE_BEGIN_ARGS( SCSMode ) {}
		SLATE_ATTRIBUTE( CSMode, CSModeAttr )
		SLATE_EVENT( FOnCSModeChanged, OnCSModeChanged )
	SLATE_END_ARGS()

	virtual void Construct( const FArguments& InArgs );
	virtual ~SCSMode() override { }

private:
	FOnCSModeChanged OnCSModeChanged;
private:
	TAttribute<CSMode> CSModeAttr;
};
