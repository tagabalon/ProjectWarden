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
	FMapEventEditor();
	virtual ~FMapEventEditor() override;

	void InitMapEventEditor(const EToolkitMode::Type Mode, const TSharedPtr<class IToolkitHost>& InitToolkitHost, UObject* ObjectToEdit);
	UMapEvent* GetMapEvent() const { return MapEvent; }
	TSharedPtr<SGraphEditor> GetMapEventGraph() const { return GraphEditor; }
	void SetUISelectionState(const FName SelectionOwner);
	virtual void ClearSelectionStateFor(const FName SelectionOwner);

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

	static const FName DetailsTab;
	static const FName GraphTab;

protected:
	void CreateGraph();
	virtual void CreateToolbar();
	virtual void BindToolbarCommands();
	virtual void CreateWidgets();
	virtual void CreateGraphWidget();

	TObjectPtr<UMapEvent> MapEvent;

	TSharedPtr<SMapEventGraphEditor> GraphEditor;
	TSharedPtr<class IDetailsView> DetailsView;

private:
	TSharedRef<SDockTab> SpawnTab_Details(const FSpawnTabArgs& Args) const;
	TSharedRef<SDockTab> SpawnTab_Graph(const FSpawnTabArgs& Args) const;

	void DoPresaveAssetUpdate();
	void OnDeleteNodes();
	bool CanDeleteNodes();

	FName CurrentUISelection;
	TSharedPtr<FUICommandList> EditorCommands;
};
