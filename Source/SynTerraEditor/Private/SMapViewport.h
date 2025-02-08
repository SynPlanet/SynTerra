// Copyright (C) 2025 SynPlanet Holding.
#pragma once
#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"

class FSceneViewport;
class SViewport;

class SMapViewport : public SCompoundWidget
{
	DECLARE_DELEGATE_TwoParams( FCurrentMapGeoPosition, double, double );
public:
	SLATE_BEGIN_ARGS( SMapViewport ) { }
		SLATE_EVENT( FCurrentMapGeoPosition, OnCurrentMapGeoPosition )
		SLATE_ATTRIBUTE( TArray<FBox2D>, Tiles )
	SLATE_END_ARGS()

	virtual void Construct( const FArguments& InArgs );
	virtual ~SMapViewport() override {}

	virtual void Tick( const FGeometry& AllottedGeometry, const double InCurrentTime, const float InDeltaTime ) override;


	TSharedPtr<FSceneViewport> GetViewport() const { return Viewport; }

	TSharedPtr<SScrollBar> GetVerticalScrollBar() const { return VerticalScrollBar; }
	TSharedPtr<SScrollBar> GetHorizontalScrollBar() const { return HorizontalScrollBar; }

public:
	void SetCurrentMapGeoPosition( double Lon, double Lat ) { OnCurrentMapGeoPosition.ExecuteIfBound( Lon, Lat ); }
	const TArray<FBox2D>& GetTiles() const { return Tiles.Get(); }

private:
	TSharedPtr<class FMapViewportClient> ViewportClient;
	TSharedPtr<FSceneViewport> Viewport;
	TSharedPtr<SViewport> ViewportWidget;

	TSharedPtr<SScrollBar> VerticalScrollBar;
	TSharedPtr<SScrollBar> HorizontalScrollBar;

private:
	TAttribute<TArray<FBox2D>> Tiles;
	FCurrentMapGeoPosition OnCurrentMapGeoPosition;

private:
	void HandleHorizontalScrollBarScrolled( float InScrollOffsetFraction );
	EVisibility HandleHorizontalScrollBarVisibility() const;
	void HandleVerticalScrollBarScrolled( float InScrollOffsetFraction );
	EVisibility HandleVerticalScrollBarVisibility() const;
};
