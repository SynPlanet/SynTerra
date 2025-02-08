// Copyright (C) 2025 SynPlanet Holding.
#include "Controller.h"
#include "SynTerraRuntime.h"
#include "Logging/StructuredLog.h"
#include "Engine/Engine.h"
#include "Engine/StaticMesh.h"
#include "Components/StaticMeshComponent.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Engine/Texture2D.h"
#include "AssetRegistry/AssetRegistryModule.h"
#include "Engine/StaticMeshActor.h"
#include "IImageWrapperModule.h"
#include "IImageWrapper.h"
#include "UObject/SavePackage.h"
#include "EditorAssetLibrary.h"
#include "Factories/TextureFactory.h"
#include "IAssetTools.h"
#include "AssetToolsModule.h"
#include "RawMesh.h"

THIRD_PARTY_INCLUDES_START
#include "stp_utils.h"
THIRD_PARTY_INCLUDES_END


namespace
{
	FBox2D ToBox2D( const Box_2d& Box )
	{
		return FBox2D(
			{ Box.x_min, Box.y_min },
			{ Box.x_max, Box.y_max }
		);
	}
}


Controller::Controller()
{
	reset_active_tiles();
	_MapTiles.Empty();
}

UserInfo Controller::GetUserInfo()
{
	UserInfo res;
	if ( const auto& user_data = get_user_data(); user_data )
	{
		res.UserName = FString( user_data->user_name );
		res.EMail = FString( user_data->email );
		res.CurrenLimit = user_data->current_limit;
	}
	return res;
}


void Controller::GenerateTiles()
{
	_ProgressTasks.Empty();
	_ShouldCancel = false;
	int CSModeCast = static_cast<int>(_CSMode);

	generate_tiles(
		(void*)this,
		[]( void*, int level, const char* msg ) {
			FString Msg( msg );
			if ( level == 0 ) {
				UE_LOGFMT( LogSynTerra, Display, "{Msg}", Msg );
			} else if ( level == 1 ) {
				UE_LOGFMT( LogSynTerra, Warning, "{Msg}", Msg );
			}
		},

		[]( void* ui, int CommonCount ) {
			auto c = (Controller*)ui;
			c->_ProgressTasks.Emplace(
				new FScopedSlowTask( CommonCount, NSLOCTEXT( "SynTerra", "OnGenerateTiles", "Generating tiles..." ), true ) );
			c->_ProgressTasks.Last()->MakeDialog( true );
		},

		[]( void* ui, int CommonCount, int CurrentCount ) {
			auto c = (Controller*)ui;
			FText Msg = FText::Format(
				NSLOCTEXT( "SynTerra", "OnGenerateTiles", "Generating tiles... {0} / {1}" ),
				FText::AsNumber( CurrentCount ), FText::AsNumber( CommonCount )
			);

			TSharedPtr<FScopedSlowTask>& ProgressTask = c->_ProgressTasks.Last();
			ProgressTask->EnterProgressFrame( 1, Msg );
			c->_ShouldCancel |= ProgressTask->ShouldCancel();
		},

		_ShouldCancel,
		CSModeCast,

		[]( void* ui, const char* file ) {
			auto c = (Controller*)ui;
			FString File( file );
			if ( c->ImportTexture( File ) == nullptr ) {
				UE_LOGFMT( LogSynTerra, Warning, "Can't import texture. File: {1}", File );
			}
		},

		[]( void* ui, const char* file ) {
			auto c = (Controller*)ui;
			FString File( file );
			if ( c->ImportTexture( File ) == nullptr ) {
				UE_LOGFMT( LogSynTerra, Warning, "Can't import texture. File: {1}", File );
			}
		},

		[]( void* ui, const Mesh* mesh, const char* tile_name ) {
			auto c = (Controller*)ui;
			FRawMesh RawMesh;
			for ( int v = 0; v < mesh->vsize; ++v ) {
				const auto& p = mesh->vertices[v];
				RawMesh.VertexPositions.Add( FVector3f( p.x, p.y, p.z ) );
			}
			for ( int i = 0; i < mesh->isize; ++i ) {
				RawMesh.WedgeIndices.Add( mesh->indices[i] );
			}
			for ( int t = 0; t < mesh->tsize; ++t ) {
				const auto& uv = mesh->uvs[t];
				RawMesh.WedgeTexCoords[0].Add( FVector2f( uv.x, uv.y ) );
			}
			for ( int n = 0; n < mesh->nsize; ++n ) {
				const auto& nrml = mesh->normals[n];
				RawMesh.WedgeTangentZ.Add( FVector3f( nrml.x, nrml.y, nrml.z ) );
			}
			for ( int it = 0; it < (mesh->isize / 3); ++it ) {
				RawMesh.FaceMaterialIndices.Add( 0 );
				RawMesh.FaceSmoothingMasks.Add( 0 );
			}
			free_mesh( mesh );

			FString TileName( tile_name );
			if ( c->BuildStaticMesh( TileName, &RawMesh ) == false ) {
				UE_LOGFMT( LogSynTerra, Warning, "Can't build static mesh. Tile: {1}", TileName );
			}
		},

		[]( void* ui, const char* tile_name ) {
			auto c = (Controller*)ui;
			UE_LOGFMT( LogSynTerra, Display, "Check is need spawn tiles." );
			if ( c->_IsNeedSpawnTiles ) {
				FString TileName( tile_name );
				c->OnSpawnTile( TileName );
			}
		}
	);

	_ProgressTasks.Empty();
	_ShouldCancel = false;
}


void Controller::OnCheckActiveMapTiles( double Lon, double Lat )
{
	Box_2d* BoxA = nullptr;
	int size = 0;
	check_active_tiles( Lon, Lat, &BoxA, size );
	_MapTiles.Empty();
	for ( int i = 0; i < size; ++i ) {
		_MapTiles.Add( ToBox2D( BoxA[i] ) );
	}
	free_array( BoxA );
}


static const FString CONTENT_PATH = "/Game/Synterra";

UTexture2D* Controller::ImportTexture( const FString& File )
{
	if ( !FPaths::FileExists( File ) )
		return nullptr;

	const FString AssetPath = CONTENT_PATH + "/Textures/";
	const FString AssetName = FPaths::GetBaseFilename( File );
	const FString FullAssetPath = AssetPath + AssetName;

	if ( UEditorAssetLibrary::DoesAssetExist( FullAssetPath ) )
		return Cast<UTexture2D>( UEditorAssetLibrary::LoadAsset( *FullAssetPath ) );

	UTextureFactory* TextureFactory = NewObject<UTextureFactory>();
	TextureFactory->AddToRoot();

	FAssetToolsModule& AssetToolsModule = FAssetToolsModule::GetModule();
	IAssetTools& AssetTools = AssetToolsModule.Get();

	TArray<FString> FilesToImport;
	FilesToImport.Add( File );
	AssetTools.ImportAssets( FilesToImport, AssetPath, TextureFactory, true );

	TextureFactory->RemoveFromRoot();

	UObject* ImportedAsset = UEditorAssetLibrary::LoadAsset( *FullAssetPath );

	if ( !ImportedAsset )
		return nullptr;

	UTexture2D* ImportedTexture = Cast<UTexture2D>( ImportedAsset );

	if ( !ImportedTexture )
		return nullptr;

	ImportedTexture->AddressX = TextureAddress::TA_Clamp;
	ImportedTexture->AddressY = TextureAddress::TA_Clamp;
	ImportedTexture->UpdateResource();

	FAssetRegistryModule::AssetCreated( ImportedTexture );
	ImportedTexture->MarkPackageDirty();

	const FString PackageFileName = FPackageName::LongPackageNameToFilename( FullAssetPath, FPackageName::GetAssetPackageExtension() );

	FSavePackageArgs SavePackageArgs;
	SavePackageArgs.TopLevelFlags = RF_Public | RF_Standalone;

	bool Saved = UPackage::SavePackage(
		ImportedTexture->GetOutermost(),
		ImportedTexture,
		*PackageFileName,
		MoveTemp( SavePackageArgs )
	);

	if ( !Saved )
		return nullptr;

	FAssetRegistryModule::AssetCreated( ImportedTexture );
	ImportedTexture->MarkPackageDirty();
	ImportedTexture->PostEditChange();

	return ImportedTexture;
}


bool Controller::BuildStaticMesh( const FString& AssetName, FRawMesh* InRawMesh )
{
	if ( InRawMesh == nullptr )
		return false;

	FString AssetName2 = "SM_" + AssetName;
	FString PackageName = CONTENT_PATH + "/StaticMeshs/" + AssetName2;

	UPackage* Package = CreatePackage( *PackageName );
	UStaticMesh* StaticMesh = NewObject<UStaticMesh>( Package, *AssetName2, RF_Public | RF_Standalone );
	StaticMesh->Modify();

	FStaticMeshSourceModel& SourceModel = StaticMesh->AddSourceModel();
	SourceModel.BuildSettings.bRecomputeNormals = false;
	SourceModel.BuildSettings.bComputeWeightedNormals = false;
	SourceModel.BuildSettings.bGenerateLightmapUVs = false;
	SourceModel.BuildSettings.bRecomputeTangents = true;

	SourceModel.RawMeshBulkData->SaveRawMesh( *InRawMesh );

	StaticMesh->Build( true );
	StaticMesh->PostEditChange();
	StaticMesh->MarkPackageDirty();

	FString PackageFileName = FPackageName::LongPackageNameToFilename( PackageName, FPackageName::GetAssetPackageExtension() );

	FSavePackageArgs SavePackageArgs;
	SavePackageArgs.TopLevelFlags = RF_Public | RF_Standalone;

	bool Saved = UPackage::SavePackage(
		Package, StaticMesh,
		*PackageFileName,
		MoveTemp( SavePackageArgs )
	);
	return Saved;
}

UWorld* GetWorld()
{
	FWorldContext* WorldContext = GEngine->GetWorldContextFromGameViewport( GEngine->GameViewport );
	if ( !WorldContext )
		return nullptr;
	UWorld* World = GEngine->GetWorldFromContextObjectChecked( WorldContext->World() );
	if ( !World )
		return nullptr;
	return World;
}

UStaticMeshComponent* Controller::SpawnStaticMesh( const FString& AssetName )
{
	UWorld* World = GetWorld();

	if ( !World )
		return nullptr;

	FString AssetPath = CONTENT_PATH + "/StaticMeshs/" + AssetName + "." + AssetName;

	UStaticMesh* StaticMesh = Cast<UStaticMesh>( StaticLoadObject( UStaticMesh::StaticClass(), nullptr, *AssetPath ) );

	if ( !StaticMesh )
		return nullptr;

	const FVector Location( 0, 0, 0 );		// Default
	FTransform Transform;
	Transform.SetLocation( Location );

	AStaticMeshActor* StaticMeshActor = World->SpawnActor<AStaticMeshActor>( AStaticMeshActor::StaticClass(), Transform );

	if ( !StaticMeshActor )
		return nullptr;

	StaticMeshActor->SetActorLabel( AssetName );		// Label to Outliner

	UStaticMeshComponent* StaticMeshComponent = StaticMeshActor->GetStaticMeshComponent();

	if ( !StaticMeshComponent )
		return nullptr;

	StaticMeshComponent->SetMobility( EComponentMobility::Movable );
	StaticMeshComponent->SetStaticMesh( StaticMesh );
	StaticMeshComponent->SetMobility( EComponentMobility::Static );
	StaticMeshComponent->RegisterComponent();
	return StaticMeshComponent;
}

void Controller::OnSpawnTile( const FString& TileName )
{
	UStaticMeshComponent* StaticMeshComponent = SpawnStaticMesh( "SM_" + TileName );

	if ( StaticMeshComponent == nullptr )
		return;

	const FString SurfaceTexturePath = CONTENT_PATH + "/Textures/" + "T_SF_" + TileName + "." + "T_SF_" + TileName;
	UTexture2D* SurfaceTexture = LoadObject<UTexture2D>( nullptr, *SurfaceTexturePath );

	const FString ClassifierTexturePath = CONTENT_PATH + "/Textures/" + "T_CS_" + TileName + "." + "T_CS_" + TileName;
	UTexture2D* ClassifierTexture = LoadObject<UTexture2D>( nullptr, *ClassifierTexturePath );

	const FString MaterialPath = "/Synterra/Materials/MI_Classification.MI_Classification";

	ApplyClassificationToMaterial(
		StaticMeshComponent,
		MaterialPath,
		SurfaceTexture,
		ClassifierTexture,
		{},		// Textures
		{}		// Colors
	);
}

void Controller::ApplyClassificationToMaterial(
	UStaticMeshComponent* MeshComponent,
	const FString& MaterialPath,
	UTexture2D* SurfaceTexture,
	UTexture2D* ClassifierTexture,
	const TArray<UTexture2D*>& Textures,
	const TArray<FLinearColor>& Colors )
{
	UMaterialInterface* Material = LoadObject<UMaterialInterface>( nullptr, *MaterialPath );

	if ( !Material )
		return;

	UMaterialInstanceDynamic* MaterialInstance = UMaterialInstanceDynamic::Create( Material, MeshComponent );

	if ( !MaterialInstance )
		return;

	if ( SurfaceTexture )
		MaterialInstance->SetTextureParameterValue( FName( "BaseTexture" ), SurfaceTexture );

	if ( ClassifierTexture )
		MaterialInstance->SetTextureParameterValue( FName( "ClassificationMap" ), ClassifierTexture );


	MeshComponent->SetMaterial( 0, MaterialInstance );
}

