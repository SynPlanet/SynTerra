// Copyright (C) 2025 SynPlanet Holding.
#include "SynTerraEditor.h"
#include "SynTerraStyle.h"
#include "SynTerraCommands.h"
#include "SLoginRegister.h"
#include "SMain.h"
#include "SynTerraRuntime.h"
#include "ControllerUtils.h"

#include "LevelEditor.h"
#include "ToolMenus.h"
#include "Logging/StructuredLog.h"
#include "HAL/PlatformApplicationMisc.h"
#include "Interfaces/IMainFrameModule.h"

#define LOCTEXT_NAMESPACE "SynTerraEditorModule"

DEFINE_LOG_CATEGORY( LogSynTerraEditor );

void FSynTerraEditorModule::StartupModule()
{
	IModuleInterface::StartupModule();

	FSynTerraStyle::Initialize();
	FSynTerraStyle::ReloadTextures();

	FSynTerraCommands::Register();

	Commands = MakeShareable( new FUICommandList );

	Commands->MapAction(
		FSynTerraCommands::Get().OpenSynTerraWindow,
		FExecuteAction::CreateRaw( this, &FSynTerraEditorModule::OnOpenSynTerraWindow ),
		FCanExecuteAction()
	);

	UToolMenus::RegisterStartupCallback(
		FSimpleMulticastDelegate::FDelegate::CreateRaw( this, &FSynTerraEditorModule::OnRegisterMenus ) );
}

void FSynTerraEditorModule::ShutdownModule()
{
	UToolMenus::UnRegisterStartupCallback( this );
	UToolMenus::UnregisterOwner( this );

	FSynTerraStyle::Shutdown();

	FSynTerraCommands::Unregister();

	IModuleInterface::ShutdownModule();
}


namespace
{
	FVector2D GetWindowPosition( FVector2D WindowSize )
	{
		FSlateRect WorkAreaRect = FSlateApplicationBase::Get().GetPreferredWorkArea();
		FVector2D DisplayTopLeft( WorkAreaRect.Left, WorkAreaRect.Top );
		FVector2D DisplaySize( WorkAreaRect.Right - WorkAreaRect.Left, WorkAreaRect.Bottom - WorkAreaRect.Top );

		float ScaleFactor = FPlatformApplicationMisc::GetDPIScaleFactorAtPoint( DisplayTopLeft.X, DisplayTopLeft.Y );
		WindowSize *= ScaleFactor;

		return (DisplayTopLeft + (DisplaySize - WindowSize) / 2.0f) / ScaleFactor;
	}

	void AddWindow( TSharedPtr<SWindow> Window )
	{
		IMainFrameModule& MainFrame = FModuleManager::LoadModuleChecked<IMainFrameModule>( "MainFrame" );
		TSharedPtr<SWindow> ParentWindow = MainFrame.GetParentWindow();
		FSlateApplication::Get().AddWindowAsNativeChild( Window.ToSharedRef(), ParentWindow.ToSharedRef() );
	}
}


void FSynTerraEditorModule::SpawnLoginRegisterWindow()
{
	FVector2D WindowSize( 400, 600 );
	FVector2D WindowPosition = GetWindowPosition( WindowSize );

	Window.Reset();

	Window = SNew( SWindow )
		.Title( LOCTEXT( "Synterra.Window.LoginRegister", "SynTerra" ) )
		.SizingRule( ESizingRule::UserSized )
		.AutoCenter( EAutoCenter::None )
		.ClientSize( WindowSize )
		.ScreenPosition( WindowPosition );


	TSharedRef<SWidget> Content =
		SNew( SLoginRegister )
		.OnLoginSuccessful_Lambda( [&] { OnLoginSuccessful(); } );

	Window->SetContent( Content );

	Window->SetOnWindowClosed(
		FOnWindowClosed::CreateRaw( this, &FSynTerraEditorModule::OnWindowClosed )
	);

	AddWindow( Window );
}

void FSynTerraEditorModule::SpawnMainWindow()
{
	FVector2D WindowSize( 1800, 800 );
	FVector2D WindowPosition = GetWindowPosition( WindowSize );

	Window.Reset();

	Window = SNew( SWindow )
		.Title( LOCTEXT( "Synterra.Window.Main", "SynTerra" ) )
		.SizingRule( ESizingRule::UserSized )
		.AutoCenter( EAutoCenter::None )
		.ClientSize( WindowSize )
		.ScreenPosition( WindowPosition );

	TSharedRef<SWidget> Content =
		SNew( SMain );

	Window->SetContent( Content );

	Window->SetOnWindowClosed(
		FOnWindowClosed::CreateRaw( this, &FSynTerraEditorModule::OnWindowClosed )
	);

	AddWindow( Window );
}

void FSynTerraEditorModule::OnWindowClosed( const TSharedRef<SWindow>& )
{
	Window.Reset();

	if ( IsLoginSuccessful )
	{
		SpawnMainWindow();
		IsLoginSuccessful = false;
	}
}

void FSynTerraEditorModule::OnLoginSuccessful()
{
	IsLoginSuccessful = true;
	Window->RequestDestroyWindow();
}


void FSynTerraEditorModule::OnOpenSynTerraWindow()
{
	if ( Window.IsValid() )
		return;

	if ( ControllerUtils::IsAuthorized() )
	{
		SpawnMainWindow();
	}
	else
	{
		SpawnLoginRegisterWindow();
	}
}


void FSynTerraEditorModule::OnRegisterMenus()
{
	// Owner will be used for cleanup in call to UToolMenus::UnregisterOwner
	FToolMenuOwnerScoped OwnerScoped( this );

	UToolMenu* Menu = UToolMenus::Get()->ExtendMenu( "LevelEditor.MainMenu.Tools" );
	FToolMenuSection& section = Menu->AddSection( TEXT( "SynTerra.Layout" ), LOCTEXT( "SynTerra.SectionLabel", "SynTerra" ) );
	section.AddMenuEntryWithCommandList( FSynTerraCommands::Get().OpenSynTerraWindow, Commands );


	FLevelEditorModule* LevelEditorModule = FModuleManager::GetModulePtr<FLevelEditorModule>( FName( TEXT( "LevelEditor" ) ) );
	TSharedPtr<FExtender> ToolbarExtender = MakeShared<FExtender>();

	ToolbarExtender->AddToolBarExtension(
		"Play",
		EExtensionHook::After,
		Commands,
		FToolBarExtensionDelegate::CreateLambda(
			[]( FToolBarBuilder& Builder )
			{
				Builder.BeginSection( "SynTerra" );
				Builder.AddToolBarButton( FSynTerraCommands::Get().OpenSynTerraWindow );
				Builder.EndSection();
			}
		)
	);

	LevelEditorModule->GetToolBarExtensibilityManager()->AddExtender( ToolbarExtender );
}


#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE( FSynTerraEditorModule, SynTerraEditor )
