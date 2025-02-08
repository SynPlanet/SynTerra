// Copyright (C) 2025 SynPlanet Holding.
#pragma once
#include "CoreMinimal.h"
#include "InputCoreTypes.h"
#include "ViewportClient.h"

class FCanvas;
class SMapViewport;
class UTexture2D;

class FMapViewportClient : public FViewportClient
{
public:
	FMapViewportClient( TWeakPtr<SMapViewport> InMapViewport );
	~FMapViewportClient();

	virtual void Draw( FViewport* Viewport, FCanvas* Canvas ) override;
	virtual bool InputKey( const FInputKeyEventArgs& InEventArgs ) override;
	virtual bool InputAxis( FViewport* Viewport, FInputDeviceId DeviceId, FKey Key, float Delta, float DeltaTime, int32 NumSamples = 1, bool bGamepad = false ) override;
	virtual UWorld* GetWorld() const override { return nullptr; }
	virtual EMouseCursor::Type GetCursor( FViewport* Viewport, int32 X, int32 Y ) override;

	FVector2D GetViewportRatio() const;

private:
	void LoadWorldTexture();
	void LoadTilesTexture();

	void CalculateTextureDimensions( int32& OutWidth, int32& OutHeight ) const;

	void ZoomIn();
	void ZoomOut();

	void SetZoomLevel( double ZoomValue );
	double CalculateDisplayedZoomLevel() const;

	void UpdateScrollBars( const FVector2D& );

	FVector2D GetViewportScrollBarPositions();

	void DrawTileItem( FCanvas*, int32 XPos, int32 YPos, const FBox2D& Box);

private:
	static constexpr double MaxZoom = 2048.0;
	static constexpr double MinZoom = 1.0 / 16.0;
	static constexpr int ZoomFactorLogSteps = 8;
	static inline const double ZoomFactor = pow( 2.0, 1.0 / ZoomFactorLogSteps );

private:
	TWeakPtr<SMapViewport> _MapViewport;
	UTexture2D* _WorldTexture = nullptr;
	UTexture2D* _TilesTexture = nullptr;

	int32 _WorldTextureWidth = 0;
	int32 _WorldTextureHeight = 0;

	double _Zoom = 1.0f;

	enum class ZoomMode
	{
		Fill,
		Custom
	};

	ZoomMode _ZoomMode = ZoomMode::Fill;

	bool _IsLeftMouseButtonPressed = false;
};
