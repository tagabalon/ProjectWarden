// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "AssetTypeCategories.h"
#include "IAssetTypeActions.h"
#include "Modules/ModuleInterface.h"
#include "Toolkits/IToolkit.h"

class FMapEventEditor;
class UMapEvent;

/**
 * This is the module definition for the editor mode. You can implement custom functionality
 * as your plugin module starts up and shuts down. See IModuleInterface for more extensibility options.
 */
class RPGSYSTEMEDITOR_API FRPGSystemEditorModule : public IModuleInterface
{
public:
	static EAssetTypeCategories::Type MapEventAssetCategory;

public:
	static TSharedRef<FMapEventEditor> CreateMapEventEditor(const EToolkitMode::Type Mode, const TSharedPtr<IToolkitHost>& InitToolkitHost, UMapEvent* MapEvent);

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;

private:
	void RegisterAssets();
	void UnregisterAssets();

	TArray<TSharedRef<IAssetTypeActions>> RegisteredAssetActions;

	/*TSharedPtr<FSlateStyleSet> m_StyleSet = nullptr;
	TSharedPtr<struct FMapEventPinFactory> m_PinFactory = nullptr;*/
};
