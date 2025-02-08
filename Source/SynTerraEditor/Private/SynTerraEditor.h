// Copyright (C) 2025 SynPlanet Holding.
#pragma once
#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"

DECLARE_LOG_CATEGORY_EXTERN( LogSynTerraEditor, Log, All );

class FSynTerraEditorModule : public IModuleInterface
{
public:
	void StartupModule() override;
	void ShutdownModule() override;

private:
	void OnRegisterMenus();
	void OnOpenSynTerraWindow();

private:
	TSharedPtr<class FUICommandList> Commands;

private:
	void SpawnLoginRegisterWindow();
	void SpawnMainWindow();

	void OnWindowClosed( const TSharedRef<SWindow>& Window );
	void OnLoginSuccessful();

private:
	TSharedPtr<SWindow> Window;
	bool IsLoginSuccessful = false;
};
