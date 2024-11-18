// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Graph/MapEventGraphEditor.h"

#include "EditorUndoClient.h"
#include "Misc/NotifyHook.h"
#include "GraphEditor.h"
#include "Toolkits/AssetEditorToolkit.h"
#include "Toolkits/IToolkitHost.h"
#include "UObject/GCObject.h"

class SMapEventGraphEditor;
class UMapEvent;

class FMapEventEditor : public FAssetEditorToolkit, public FEditorUndoClient, public FGCObject, public FNotifyHook
{
public:
	void InitMapEventEditor(const EToolkitMode::Type Mode, const TSharedPtr<class IToolkitHost>& InitToolkitHost, UObject* ObjectToEdit);

	static const FName DetailsTab;
	static const FName GraphTab;

protected:
	virtual void CreateToolbar();
	virtual void BindToolbarCommands();
	virtual void CreateWidgets();
	virtual void CreateGraphWidget();

	TObjectPtr<UMapEvent> MapEvent;

	TSharedPtr<SMapEventGraphEditor> GraphEditor;
	TSharedPtr<class IDetailsView> DetailsView;

private:
	FName CurrentUISelection;

public:
	FMapEventEditor();
	virtual ~FMapEventEditor() override;

	UMapEvent* GetMapEvent() const { return MapEvent; }
	TSharedPtr<SGraphEditor> GetMapEventGraph() const { return GraphEditor; }

	// FGCObject
	virtual void AddReferencedObjects(FReferenceCollector& Collector) override;
	virtual FString GetReferencerName() const override
	{
		return TEXT("FMapEventEditor");
	}

	// FEditorUndoClient
	virtual void PostUndo(bool bSuccess) override;
	virtual void PostRedo(bool bSuccess) override;

	virtual void HandleUndoTransaction();

	virtual void NotifyPostChange(const FPropertyChangedEvent& PropertyChangedEvent, FProperty* PropertyThatChanged) override;

	// IToolkit
	virtual FName GetToolkitFName() const override;
	virtual FText GetBaseToolkitName() const override;
	virtual FString GetWorldCentricTabPrefix() const override;
	virtual FLinearColor GetWorldCentricTabColorScale() const override;
	virtual void RegisterTabSpawners(const TSharedRef<class FTabManager>& InTabManager) override;
	virtual void UnregisterTabSpawners(const TSharedRef<class FTabManager>& InTabManager) override;

	// FAssetEditorToolkit
	/*virtual void InitToolMenuContext(FToolMenuContext& MenuContext) override;
	virtual void PostRegenerateMenusAndToolbars() override;
	virtual void SaveAsset_Execute() override;
	virtual void SaveAssetAs_Execute() override;*/

	bool IsTabFocused(const FTabId& TabId) const;
private:
	TSharedRef<SDockTab> SpawnTab_Details(const FSpawnTabArgs& Args) const;
	TSharedRef<SDockTab> SpawnTab_Graph(const FSpawnTabArgs& Args) const;

	void DoPresaveAssetUpdate();

public:
	void SetUISelectionState(const FName SelectionOwner);
	virtual void ClearSelectionStateFor(const FName SelectionOwner);
};
