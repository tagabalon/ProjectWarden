// Fill out your copyright notice in the Description page of Project Settings.

#include "Asset/AssetTypeActions_MapEvent.h"

#include "RPGSystemEditorModule.h"


#include "MapEvent.h"

#define LOCTEXT_NAMESPACE "AssetTypeActions_MapEvent"

FText FAssetTypeActions_MapEvent::GetName() const
{
	return LOCTEXT("AssetTypeActions_MapEventAsset", "Map Event");
}

FColor FAssetTypeActions_MapEvent::GetTypeColor() const
{
	return FColor::Cyan;
}

UClass* FAssetTypeActions_MapEvent::GetSupportedClass() const
{
	return UMapEvent::StaticClass();
}

void FAssetTypeActions_MapEvent::OpenAssetEditor(const TArray<UObject*>& inObjects, TSharedPtr<class IToolkitHost> editWithinLevelEditor)
{
	EToolkitMode::Type mode = editWithinLevelEditor.IsValid() ? EToolkitMode::WorldCentric : EToolkitMode::Standalone;
	for (UObject* object : inObjects) {
		if(UMapEvent* mapEvent = Cast<UMapEvent>(object))
		{
			const FRPGSystemEditorModule* RPGSystemModule = &FModuleManager::LoadModuleChecked<FRPGSystemEditorModule>("RPGSystemEditor");
			RPGSystemModule->CreateMapEventEditor(mode, editWithinLevelEditor, mapEvent);
		}
	}
}

uint32 FAssetTypeActions_MapEvent::GetCategories()
{
	return FRPGSystemEditorModule::MapEventAssetCategory;
}

#undef LOCTEXT_NAMESPACE