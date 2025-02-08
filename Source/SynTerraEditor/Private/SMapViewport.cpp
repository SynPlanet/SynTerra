// Copyright (C) 2025 SynPlanet Holding.
#include "SMapViewport.h"
#include "FMapViewportClient.h"
#include "Framework/Application/SlateApplication.h"
#include "Widgets/SViewport.h"
#include "Slate/SceneViewport.h"

#define LOCTEXT_NAMESPACE "MapViewport"

void SMapViewport::Construct( const FArguments& InArgs )
{
	Tiles = InArgs._Tiles;
	OnCurrentMapGeoPosition = InArgs._OnCurrentMapGeoPosition;

	ChildSlot
		[
			SNew( SVerticalBox )
				+ SVerticalBox::Slot()
				.FillHeight( 1.0f )
				[
					SNew( SOverlay )
						+ SOverlay::Slot()
						.Padding( 5.0f, 0.0f )
						[
							SAssignNew( ViewportWidget, SViewport )
								.EnableGammaCorrection( false )
								.IsEnabled( FSlateApplication::Get().GetNormalExecutionAttribute() )
								.ShowEffectWhenDisabled( false )
								.EnableBlending( true )
						]
						+ SOverlay::Slot()
						.HAlign( HAlign_Right )
						[
							SAssignNew( VerticalScrollBar, SScrollBar )
								.Visibility( this, &SMapViewport::HandleVerticalScrollBarVisibility )
								.OnUserScrolled( this, &SMapViewport::HandleVerticalScrollBarScrolled )
						]
				]
				+ SVerticalBox::Slot()
				.AutoHeight()
				[
					SAssignNew( HorizontalScrollBar, SScrollBar )
						.Orientation( Orient_Horizontal )
						.Visibility( this, &SMapViewport::HandleHorizontalScrollBarVisibility )
						.OnUserScrolled( this, &SMapViewport::HandleHorizontalScrollBarScrolled )
				]
		];

	ViewportClient = MakeShareable( new FMapViewportClient( SharedThis( this ) ) );
	Viewport = MakeShareable( new FSceneViewport( ViewportClient.Get(), ViewportWidget ) );
	ViewportWidget->SetViewportInterface( Viewport.ToSharedRef() );
}


void SMapViewport::Tick( const FGeometry& AllottedGeometry, const double InCurrentTime, const float InDeltaTime )
{
	Viewport->Invalidate();
}


void SMapViewport::HandleHorizontalScrollBarScrolled( float InScrollOffsetFraction )
{
	const float Ratio = ViewportClient->GetViewportRatio().X;
	const float MaxOffset = (Ratio < 1.0f) ? 1.0f - Ratio : 0.0f;
	InScrollOffsetFraction = FMath::Clamp( InScrollOffsetFraction, 0.0f, MaxOffset );

	HorizontalScrollBar->SetState( InScrollOffsetFraction, Ratio );
}

EVisibility SMapViewport::HandleHorizontalScrollBarVisibility() const
{
	return (ViewportClient->GetViewportRatio().X < 1.0f)
		? EVisibility::Visible
		: EVisibility::Collapsed;
}

void SMapViewport::HandleVerticalScrollBarScrolled( float InScrollOffsetFraction )
{
	const float Ratio = ViewportClient->GetViewportRatio().X;
	const float MaxOffset = (Ratio < 1.0f) ? 1.0f - Ratio : 0.0f;
	InScrollOffsetFraction = FMath::Clamp( InScrollOffsetFraction, 0.0f, MaxOffset );

	VerticalScrollBar->SetState( InScrollOffsetFraction, Ratio );
}

EVisibility SMapViewport::HandleVerticalScrollBarVisibility() const
{
	return (ViewportClient->GetViewportRatio().Y < 1.0f)
		? EVisibility::Visible
		: EVisibility::Collapsed;
}

#undef LOCTEXT_NAMESPACE
