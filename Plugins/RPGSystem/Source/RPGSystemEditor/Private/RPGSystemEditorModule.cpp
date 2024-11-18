// Copyright Epic Games, Inc. All Rights Reserved.

#include "RPGSystemEditorModule.h"

#include "Asset/AssetTypeActions_MapEvent.h"
#include "Asset/MapEventEditor.h"
#include "MapEventEditorStyle.h"

#include "MapEvent.h"

#include "AssetToolsModule.h"

#define LOCTEXT_NAMESPACE "RPGSystemEditorModule"

EAssetTypeCategories::Type FRPGSystemEditorModule::MapEventAssetCategory = static_cast<EAssetTypeCategories::Type>(0);

void FRPGSystemEditorModule::StartupModule()
{
	FMapEventEditorStyle::Initialize();

	RegisterAssets();

	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module

	/*IAssetTools& assetToolsModule = IAssetTools::Get();
	EAssetTypeCategories::Type assetType = assetToolsModule.RegisterAdvancedAssetCategory(FName(TEXT("RPGSystem")), LOCTEXT("RPGSystem", "RPG System"));
	TSharedPtr<FMapEventAction> mapEventAction = MakeShareable(new FMapEventAction(assetType));
	assetToolsModule.RegisterAssetTypeActions(mapEventAction.ToSharedRef());

	m_StyleSet = MakeShareable(new FSlateStyleSet(TEXT("RPGSystemStyle")));
	TSharedPtr<IPlugin> plugin = IPluginManager::Get().FindPlugin("RPGSystem");
	FString contentDir = plugin->GetContentDir();
	m_StyleSet->SetContentRoot(contentDir);

	FSlateImageBrush* thumbnailBrush = new FSlateImageBrush(m_StyleSet->RootToContentDir(TEXT("MapEventIcon"), TEXT(".png")), FVector2D(128.0f, 128.0f));
	FSlateImageBrush* iconBrush = new FSlateImageBrush(m_StyleSet->RootToContentDir(TEXT("CustomAssetIcon"), TEXT(".png")), FVector2D(128.0f, 128.0f));
	FSlateImageBrush* nodeAddPinIcon = new FSlateImageBrush(m_StyleSet->RootToContentDir(TEXT("NodeAddPinIcon"), TEXT(".png")), FVector2D(128.0f, 128.0f));
	FSlateImageBrush* nodeDeletePinIcon = new FSlateImageBrush(m_StyleSet->RootToContentDir(TEXT("NodeDeletePinIcon"), TEXT(".png")), FVector2D(128.0f, 128.0f));
	FSlateImageBrush* nodeDeleteNodeIcon = new FSlateImageBrush(m_StyleSet->RootToContentDir(TEXT("NodeDeleteNodeIcon"), TEXT(".png")), FVector2D(128.0f, 128.0f));
	m_StyleSet->Set(TEXT("ClassThumbnail.MapEvent"), thumbnailBrush);
	m_StyleSet->Set(TEXT("ClassIcon.MapEvent"), thumbnailBrush);
	m_StyleSet->Set(TEXT("RPGSystem.NodeAddPinIcon"), nodeAddPinIcon);
	m_StyleSet->Set(TEXT("RPGSystem.NodeDeletePinIcon"), nodeDeletePinIcon);
	m_StyleSet->Set(TEXT("RPGSystem.NodeDeleteNodeIcon"), nodeDeleteNodeIcon);
	FSlateStyleRegistry::RegisterSlateStyle(*m_StyleSet);

	m_PinFactory = MakeShareable(new FMapEventPinFactory());
	FEdGraphUtilities::RegisterVisualPinFactory(m_PinFactory);*/

}

void FRPGSystemEditorModule::ShutdownModule()
{
	FMapEventEditorStyle::Shutdown();

	UnregisterAssets();
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.

	//FSlateStyleRegistry::UnRegisterSlateStyle(*m_StyleSet);
	//FEdGraphUtilities::UnregisterVisualPinFactory(m_PinFactory);
	//m_PinFactory = nullptr;
}

void FRPGSystemEditorModule::RegisterAssets()
{
	IAssetTools& AssetTools = FModuleManager::LoadModuleChecked<FAssetToolsModule>("AssetTools").Get();

	MapEventAssetCategory = AssetTools.RegisterAdvancedAssetCategory(FName(TEXT("RPGSystem")), LOCTEXT("RPGSystem", "RPG System"));

	const TSharedRef<IAssetTypeActions> MapEventActions = MakeShareable(new FAssetTypeActions_MapEvent());
	RegisteredAssetActions.Add(MapEventActions);
	AssetTools.RegisterAssetTypeActions(MapEventActions);

}
void FRPGSystemEditorModule::UnregisterAssets()
{
	RegisteredAssetActions.Empty();
}

TSharedRef<FMapEventEditor> FRPGSystemEditorModule::CreateMapEventEditor(const EToolkitMode::Type Mode, const TSharedPtr<IToolkitHost>& InitToolkitHost, UMapEvent* MapEvent)
{
	TSharedRef<FMapEventEditor> NewMapEventEditor(new FMapEventEditor());
	NewMapEventEditor->InitMapEventEditor(Mode, InitToolkitHost, MapEvent);
	return NewMapEventEditor;
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FRPGSystemEditorModule, RPGSystemEditor)