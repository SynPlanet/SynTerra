// Copyright (C) 2025 SynPlanet Holding.
#pragma
#include "CoreMinimal.h"
#include "SynTerraEditor/Private/FUserInfo.h"
#include "SynTerraEditor/Private/FCSMode.h"

class UTexture2D;
struct FRawMesh;
class UStaticMeshComponent;

class SYNTERRARUNTIME_API Controller
{
public:
	Controller();

	void GenerateTiles();
	void OnCheckActiveMapTiles( double Lon, double Lat );

private:
	CSMode _CSMode = CSMode::Geodetic;
	bool _IsNeedSpawnTiles = true;
	TArray<FBox2D> _MapTiles;
	FString _Message;

	TArray<TSharedPtr<struct FScopedSlowTask>> _ProgressTasks;
	bool _ShouldCancel = false;

public:
	UserInfo GetUserInfo();
	CSMode GetCSMode() const { return _CSMode; }
	void SetCSMode( CSMode Mode ) { _CSMode = Mode; }
	TOptional<int> GetNumberOfTiles() const { return _MapTiles.Num() > 0 ? _MapTiles.Num() : TOptional<int>(); }
	bool GetIsNeedSpawnTiles() const { return _IsNeedSpawnTiles; }
	void SetIsNeedSpawnTiles( bool IsNeedSpawnTiles ) { _IsNeedSpawnTiles = IsNeedSpawnTiles; }
	TArray<FBox2D> GetMapTiles() const { return _MapTiles; }
	FString GetMessage() const { return _Message; }

private:
	UTexture2D* ImportTexture( const FString& File );
	bool BuildStaticMesh( const FString& AssetName, FRawMesh* InRawMesh );
	UStaticMeshComponent* SpawnStaticMesh( const FString& AssetName );
	void OnSpawnTile( const FString& TileName );
	void ApplyClassificationToMaterial(
		UStaticMeshComponent* MeshComponent,
		const FString& MaterialPath,
		UTexture2D* SurfaceTexture,
		UTexture2D* ClassifierTexture,
		const TArray<UTexture2D*>& Textures,
		const TArray<FLinearColor>& Colors );
};

