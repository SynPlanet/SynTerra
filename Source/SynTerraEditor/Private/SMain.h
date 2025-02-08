// Copyright (C) 2025 SynPlanet Holding.
#pragma once
#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"

class SMain : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS( SMain ) {}
	SLATE_END_ARGS()

	virtual void Construct( const FArguments& InArgs );
	virtual ~SMain() override {}

private:
	TSharedPtr<class Controller> _Controller;
};
