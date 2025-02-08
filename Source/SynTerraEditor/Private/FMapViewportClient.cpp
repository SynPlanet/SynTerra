// Copyright (C) 2025 SynPlanet Holding.
#include "FMapViewportClient.h"
#include "SMapViewport.h"
#include "Logging/StructuredLog.h"
#include "Slate/SceneViewport.h"
#include "Engine/Canvas.h"
#include "Engine/Texture2D.h"
#include "CanvasItem.h"
#include "TextureCompiler.h"

FMapViewportClient::FMapViewportClient( TWeakPtr<SMapViewport> InMapViewport )
	: _MapViewport( InMapViewport )
{
	LoadWorldTexture();
	LoadTilesTexture();
}

FMapViewportClient::~FMapViewportClient()
{
}


void FMapViewportClient::LoadWorldTexture()
{
	const FString WorldTexturePath = TEXT( "/Synterra/World.World" );

	if ( _WorldTexture )
		return;

	_WorldTexture = LoadObject<UTexture2D>( nullptr, *WorldTexturePath );

	if ( !_WorldTexture )
	{
		UE_LOGFMT( LogTemp, Error, "Failed to load WorldTexture: {1}", WorldTexturePath );
		return;
	}

	FTextureCompilingManager::Get().FinishCompilation( { _WorldTexture } );
	_WorldTexture->WaitForStreaming();

	_WorldTextureWidth = _WorldTexture->GetSizeX();
	_WorldTextureHeight = _WorldTexture->GetSizeY();
}

void FMapViewportClient::LoadTilesTexture()
{
	const FString TilesTexturePath = TEXT( "/Synterra/Tiles.Tiles" );

	if ( _TilesTexture )
		return;

	_TilesTexture = LoadObject<UTexture2D>( nullptr, *TilesTexturePath );

	if ( !_TilesTexture )
	{
		UE_LOGFMT( LogTemp, Error, "Failed to load TilesTexture: {1}", TilesTexturePath );
		return;
	}

	FTextureCompilingManager::Get().FinishCompilation( { _TilesTexture } );
	_TilesTexture->WaitForStreaming();
}


void FMapViewportClient::Draw( FViewport* Viewport, FCanvas* Canvas )
{
	if ( _WorldTexture == nullptr )
		return;

	Canvas->Clear( FLinearColor( 0.0f, 0.0f, 0.0f ) );

	_WorldTexture->SetForceMipLevelsToBeResident( 30.0f );
	_WorldTexture->WaitForStreaming();

	auto World_TextureResource = _WorldTexture->GetResource();
	if ( World_TextureResource == nullptr )
		return;

	FVector2D ViewportSize = FVector2D( _MapViewport.Pin()->GetViewport()->GetSizeXY() );
	FVector2D Ratio = GetViewportRatio();
	float YOffset = static_cast<float>((Ratio.Y > 1.0) ? ((ViewportSize.Y - (ViewportSize.Y / Ratio.Y)) * 0.5) : 0);
	float XOffset = static_cast<float>((Ratio.X > 1.0) ? ((ViewportSize.X - (ViewportSize.X / Ratio.X)) * 0.5) : 0);
	FVector2D ScrollBarPos = GetViewportScrollBarPositions();
	int32 YPos = (int32)FMath::Clamp( FMath::RoundToInt( YOffset - ScrollBarPos.Y ), TNumericLimits<int32>::Min(), TNumericLimits<int32>::Max() );
	int32 XPos = (int32)FMath::Clamp( FMath::RoundToInt( XOffset - ScrollBarPos.X ), TNumericLimits<int32>::Min(), TNumericLimits<int32>::Max() );

	int32 Width = 0, Height = 0;
	CalculateTextureDimensions( Width, Height );


	FCanvasTileItem World_TileItem( FVector2D( XPos, YPos ), World_TextureResource, FVector2D( Width, Height ), FLinearColor( 1.0f, 1.0f, 1.0f ) );
	World_TileItem.BlendMode = SE_BLEND_Opaque;

	Canvas->DrawItem( World_TileItem );


	if ( _TilesTexture )
	{
		if ( auto Tiles_TextureResource = _TilesTexture->GetResource(); Tiles_TextureResource )
		{
			FCanvasTileItem Tiles_TileItem( FVector2D( XPos, YPos ), Tiles_TextureResource, FVector2D( Width, Height ), FLinearColor( 1.0f, 1.0f, 1.0f ) );
			Tiles_TileItem.BlendMode = SE_BLEND_AlphaBlend;

			Canvas->DrawItem( Tiles_TileItem );
		}
	}
	

	for ( const auto& Box : _MapViewport.Pin()->GetTiles() )
	{
		DrawTileItem( Canvas, XPos, YPos, Box );
	}
}


void FMapViewportClient::DrawTileItem( FCanvas* Canvas, int32 XPos, int32 YPos, const FBox2D& Box )
{
	int32 Width = 0, Height = 0;
	CalculateTextureDimensions( Width, Height );

	const double Xpx = (double)Width / 360.0;
	const double Ypx = (double)Height / 180.0;
	const double X = (Box.Min.X + 180.0) * Xpx;
	const double Y = std::abs( Box.Max.Y - 90.0 ) * Ypx;

	int32 BorderSize = 0;
	float ScaledBorderSize = ((float)BorderSize - 1) * 0.5f;

	FCanvasTileItem TileItem(
		FVector2D( (float)XPos - ScaledBorderSize + X, (float)YPos - ScaledBorderSize + Y ),
		FVector2D( (Box.Max.X - Box.Min.X) * Xpx + BorderSize, (Box.Max.Y - Box.Min.Y) * Ypx + BorderSize ),
		FColor( 255, 128, 0 )
	);

	Canvas->DrawItem( TileItem );
}


bool FMapViewportClient::InputKey( const FInputKeyEventArgs& InEventArgs )
{
	if ( InEventArgs.Event == IE_Pressed )
	{
		if ( InEventArgs.Key == EKeys::MouseScrollUp ||
			 InEventArgs.Key == EKeys::MouseScrollDown )
		{
			TSharedPtr<SMapViewport> MapViewport = _MapViewport.Pin();
			TSharedPtr<FSceneViewport> SceneViewport = MapViewport->GetViewport();

			FIntPoint MousePos;
			SceneViewport->GetMousePos( MousePos );
			FVector2D MousePosition = { (float)MousePos[0], (float)MousePos[1] };

			const FVector2D NormMousePos = MousePosition / SceneViewport->GetSizeXY();

			FVector2D Ratio = GetViewportRatio();
			float HDistFromTop = MapViewport->GetHorizontalScrollBar()->DistanceFromTop();
			float VDistFromTop = MapViewport->GetVerticalScrollBar()->DistanceFromTop();

			FVector2D Position(
				HDistFromTop + NormMousePos.X * Ratio.X,
				VDistFromTop + NormMousePos.Y * Ratio.Y
			);

			if ( InEventArgs.Key == EKeys::MouseScrollUp )
			{
				ZoomIn();
			}
			else if ( InEventArgs.Key == EKeys::MouseScrollDown )
			{
				ZoomOut();
			}

			Position -= (NormMousePos * GetViewportRatio());

			UpdateScrollBars( Position );

			return true;
		}

		if ( InEventArgs.Key == EKeys::LeftMouseButton )
		{
			if ( !_IsLeftMouseButtonPressed )
			{
				_IsLeftMouseButtonPressed = true;
			}
		}
	}
	else if ( InEventArgs.Event == IE_Released)
	{
		if ( InEventArgs.Key == EKeys::LeftMouseButton )
		{
			if ( _IsLeftMouseButtonPressed )
			{
				_IsLeftMouseButtonPressed = false;


				TSharedPtr<SMapViewport> MapViewport = _MapViewport.Pin();
				TSharedPtr<FSceneViewport> SceneViewport = MapViewport->GetViewport();

				FIntPoint MousePos;
				SceneViewport->GetMousePos( MousePos );
				FVector2D MousePosition = { (float)MousePos[0], (float)MousePos[1] };

				const FVector2D NormMousePos = MousePosition / SceneViewport->GetSizeXY();

				FVector2D Ratio = GetViewportRatio();
				float HDistFromTop = MapViewport->GetHorizontalScrollBar()->DistanceFromTop();
				float VDistFromTop = MapViewport->GetVerticalScrollBar()->DistanceFromTop();

				if ( Ratio.X > 1.0 || Ratio.Y > 1.0 )
					return false;	// Skip

				FVector2D Position(
					HDistFromTop + NormMousePos.X * Ratio.X,
					VDistFromTop + NormMousePos.Y * Ratio.Y
				);

				int32 Width = 0, Height = 0;
				CalculateTextureDimensions( Width, Height );

				FVector2D MapPos( Position.X * Width, Position.Y * Height );

				FVector2D MapGeoPosition(
					(MapPos.X / ((double)Width / 360.0)) - 180.0,	// Lon
					90.0 - (MapPos.Y / ((double)Height / 180.0))	// Lat
				);


				_MapViewport.Pin()->SetCurrentMapGeoPosition( MapGeoPosition.X, MapGeoPosition.Y );
			}
		}
	}

	return false;
}


bool FMapViewportClient::InputAxis( FViewport* InViewport, FInputDeviceId DeviceId, FKey Key, float Delta, float DeltaTime, int32 NumSamples, bool bGamepad )
{
	if ( Key == EKeys::MouseX || Key == EKeys::MouseY )
	{
		if ( InViewport->KeyState( EKeys::LeftMouseButton  ) )
		{
			_IsLeftMouseButtonPressed = false;


			TSharedPtr<SMapViewport> Viewport = _MapViewport.Pin();

			int32 Width = 0, Height = 0;
			CalculateTextureDimensions( Width, Height );

			if ( Key == EKeys::MouseY )
			{
				float VDistFromBottom = Viewport->GetVerticalScrollBar()->DistanceFromBottom();
				float VRatio = GetViewportRatio().Y;
				float LocalDelta = (Delta / static_cast<float>(Height));

				Viewport->GetVerticalScrollBar()->SetState( FMath::Clamp( (1.f - VDistFromBottom - VRatio) + LocalDelta, 0.0f, 1.0f - VRatio ), VRatio );
			}
			else
			{
				float HDistFromBottom = Viewport->GetHorizontalScrollBar()->DistanceFromBottom();
				float HRatio = GetViewportRatio().X;
				float LocalDelta = (Delta / static_cast<float>(Width)) * -1.f;

				Viewport->GetHorizontalScrollBar()->SetState( FMath::Clamp( (1.f - HDistFromBottom - HRatio) + LocalDelta, 0.0f, 1.0f - HRatio ), HRatio );
			}

			return true;
		}
	}

	return false;
}


EMouseCursor::Type FMapViewportClient::GetCursor( FViewport* Viewport, int32 X, int32 Y )
{
	return (Viewport->KeyState( EKeys::LeftMouseButton ))
		? EMouseCursor::GrabHandClosed
		: EMouseCursor::Default;
}


FVector2D FMapViewportClient::GetViewportRatio() const
{
	int32 Width = 0, Height = 0;
	CalculateTextureDimensions( Width, Height );

	FVector2D ViewportSize = FVector2D( _MapViewport.Pin()->GetViewport()->GetSizeXY() );
	FVector2D Ratio = { ViewportSize.X / (float)Width, ViewportSize.Y / Height };

	return Ratio;
}


void FMapViewportClient::CalculateTextureDimensions( int32& OutWidth, int32& OutHeight ) const
{
	if ( _WorldTexture == nullptr )
		return;

	OutWidth = _WorldTexture->GetSizeX();
	OutHeight = _WorldTexture->GetSizeY();

	if ( _WorldTextureWidth == 0 || _WorldTextureHeight == 0 )
	{
		OutWidth = 0;
		OutHeight = 0;
		return;
	}

	if ( _ZoomMode == ZoomMode::Fill )
	{
		const int32 MaxWidth = FMath::Max( _MapViewport.Pin()->GetViewport()->GetSizeXY().X, 0);
		const int32 MaxHeight = FMath::Max( _MapViewport.Pin()->GetViewport()->GetSizeXY().Y, 0);

		if ( MaxWidth * _WorldTextureHeight < MaxHeight * _WorldTextureWidth )
		{
			OutWidth = MaxWidth;
			OutHeight = FMath::DivideAndRoundNearest( OutWidth * _WorldTextureHeight, _WorldTextureWidth );
		}
		else
		{
			OutHeight = MaxHeight;
			OutWidth = FMath::DivideAndRoundNearest( OutHeight * _WorldTextureWidth, _WorldTextureHeight );
		}
	}
	else
	{
		OutWidth = static_cast<int32>( _WorldTextureWidth * _Zoom );
		OutHeight = static_cast<int32>( _WorldTextureHeight * _Zoom );
	}
}


void FMapViewportClient::ZoomIn()
{
	const double CurrentZoom = CalculateDisplayedZoomLevel();
	SetZoomLevel( CurrentZoom * ZoomFactor );
}


void FMapViewportClient::ZoomOut()
{
	const double CurrentZoom = CalculateDisplayedZoomLevel();
	SetZoomLevel( CurrentZoom / ZoomFactor );
}


void FMapViewportClient::SetZoomLevel( double ZoomValue )
{
	double LogZoom = log2( ZoomValue );
	double QuantizationSteps = ZoomFactorLogSteps * 2.0;
	double LogZoomQuantized = (1.0 / QuantizationSteps) * (double)FMath::RoundToInt( QuantizationSteps * LogZoom );
	ZoomValue = pow( 2.0, LogZoomQuantized );
	ZoomValue = FMath::Clamp( ZoomValue, MinZoom, MaxZoom );

	_Zoom = ZoomValue;
	_ZoomMode = ZoomMode::Custom;
}


double FMapViewportClient::CalculateDisplayedZoomLevel() const
{
	if ( _ZoomMode == ZoomMode::Custom )
		return _Zoom;

	int32 Width = 0, Height = 0;
	CalculateTextureDimensions( Width, Height );

	if ( _WorldTextureHeight != 0 )
		return (double)Height / _WorldTextureHeight;

	if ( _WorldTextureWidth != 0 )
		return (double)Width / _WorldTextureWidth;

	return 0.0;
}


void FMapViewportClient::UpdateScrollBars( const FVector2D& Position )
{
	TSharedPtr<SMapViewport> Viewport = _MapViewport.Pin();

	if ( !Viewport.IsValid() ||
		 !Viewport->GetVerticalScrollBar().IsValid() ||
		 !Viewport->GetHorizontalScrollBar().IsValid() )
	{
		return;
	}

	FVector2D Ratio = GetViewportRatio();
	float VDistFromTop = Viewport->GetVerticalScrollBar()->DistanceFromTop();
	float VDistFromBottom = Viewport->GetVerticalScrollBar()->DistanceFromBottom();
	float HDistFromTop = Viewport->GetHorizontalScrollBar()->DistanceFromTop();
	float HDistFromBottom = Viewport->GetHorizontalScrollBar()->DistanceFromBottom();

	if ( float VRatio = Ratio.Y; VRatio < 1.0f )
	{
		if ( VDistFromBottom < 1.0f )
		{
			Viewport->GetVerticalScrollBar()->SetState( FMath::Clamp( Position.Y, 0.0f, 1.0f - VRatio ), VRatio );
		}
		else
		{
			Viewport->GetVerticalScrollBar()->SetState( 0.0f, VRatio );
		}
	}

	if ( float HRatio = Ratio.X; HRatio < 1.0f )
	{
		if ( HDistFromBottom < 1.0f )
		{
			Viewport->GetHorizontalScrollBar()->SetState( FMath::Clamp( Position.X, 0.0f, 1.0f - HRatio ), HRatio );
		}
		else
		{
			Viewport->GetHorizontalScrollBar()->SetState( 0.0f, HRatio );
		}
	}
}


FVector2D FMapViewportClient::GetViewportScrollBarPositions()
{
	TSharedPtr<SMapViewport> Viewport = _MapViewport.Pin();

	if ( !Viewport.IsValid() ||
		 !Viewport->GetVerticalScrollBar().IsValid() ||
		 !Viewport->GetHorizontalScrollBar().IsValid() )
	{
		return FVector2D::ZeroVector;
	}

	FVector2D Positions = FVector2D::ZeroVector;

	FVector2D Ratio = GetViewportRatio();
	float VDistFromTop = Viewport->GetVerticalScrollBar()->DistanceFromTop();
	float VDistFromBottom = Viewport->GetVerticalScrollBar()->DistanceFromBottom();
	float HDistFromTop = Viewport->GetHorizontalScrollBar()->DistanceFromTop();
	float HDistFromBottom = Viewport->GetHorizontalScrollBar()->DistanceFromBottom();

	int32 Width = 0, Height = 0;
	CalculateTextureDimensions( Width, Height );

	if ( (Viewport->GetVerticalScrollBar()->GetVisibility() == EVisibility::Visible) &&
		 VDistFromBottom < 1.0f )
	{
		Positions.Y = FMath::Clamp( (1.0f + VDistFromTop - VDistFromBottom - Ratio.Y) * 0.5f, 0.0f, 1.0f - Ratio.Y ) * (float)Height;
	}

	if ( (Viewport->GetHorizontalScrollBar()->GetVisibility() == EVisibility::Visible) &&
		 HDistFromBottom < 1.0f )
	{
		Positions.X = FMath::Clamp( (1.0f + HDistFromTop - HDistFromBottom - Ratio.X) * 0.5f, 0.0f, 1.0f - Ratio.X ) * (float)Width;
	}

	return Positions;
}

