// Copyright (C) 2025 SynPlanet Holding.
#pragma once
#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"

class SNumberOfTiles : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS( SNumberOfTiles ) {}
		SLATE_ATTRIBUTE( TOptional<int>, NumberOfTiles )
	SLATE_END_ARGS()

	virtual void Construct( const FArguments& InArgs );
	virtual ~SNumberOfTiles() override {}

private:
	TAttribute<TOptional<int>> NumberOfTiles;
};
