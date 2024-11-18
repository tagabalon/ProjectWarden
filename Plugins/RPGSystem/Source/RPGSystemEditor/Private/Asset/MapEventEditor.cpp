// Fill out your copyright notice in the Description page of Project Settings.

#include "Asset/MapEventEditor.h"

#include "Asset/MapEventEditorContext.h"
#include "Graph/MapEventGraph.h"
#include "Graph/MapEventGraphSchema.h"
#include "MapEventEditorCommands.h"
#include "Kismet2/BlueprintEditorUtils.h"

#include "MapEvent.h"

#include "EditorClassUtils.h"
#include "GraphEditor.h"

#define LOCTEXT_NAMESPACE "MapEventEditor"

const FName FMapEventEditor::DetailsTab(TEXT("Details"));
const FName FMapEventEditor::GraphTab(TEXT("Graph"));

FMapEventEditor::FMapEventEditor()
    :MapEvent(nullptr)
{
}

FMapEventEditor::~FMapEventEditor()
{
    GEditor->UnregisterForUndo(this);
}

void FMapEventEditor::InitMapEventEditor(const EToolkitMode::Type Mode, const TSharedPtr<class IToolkitHost>& InitToolkitHost, UObject* ObjectToEdit)
{
    MapEvent = CastChecked<UMapEvent>(ObjectToEdit);

    if (MapEvent->GetGraph() == nullptr)
    {
        UEdGraph* NewGraph = FBlueprintEditorUtils::CreateNewGraph(
            MapEvent,
            NAME_None,
            UMapEventGraph::StaticClass(),
            UMapEventGraphSchema::StaticClass()
        );
        check(NewGraph);

        MapEvent->SetGraph(NewGraph);
        MapEvent->GetGraph()->bAllowDeletion = false;

		const UEdGraphSchema* Schema = NewGraph->GetSchema();
		Schema->CreateDefaultNodesForGraph(*NewGraph);
    }

	/*UMapEventGraph* MapEventGraph = Cast<UMapEventGraph>(MapEvent->GetGraph());
	if (IsValid(MapEventGraph))
	{
		MapEventGraph->OnLoaded();
	}*/

    // Support undo/redo
    MapEvent->SetFlags(RF_Transactional);
    GEditor->RegisterForUndo(this);

    UMapEventGraphSchema::SubscribeToAssetChanges();

    //BindToolbarCommands();
    //CreateToolbar();

    CreateWidgets();

    const TSharedRef<FTabManager::FLayout> StandaloneDefaultLayout = FTabManager::NewLayout("MapEventEditor_Layout_v5.1")
        ->AddArea
        (
            FTabManager::NewPrimaryArea()->SetOrientation(Orient_Horizontal)
            ->Split
            (
                FTabManager::NewStack()
                ->SetSizeCoefficient(0.225f)
                ->AddTab(DetailsTab, ETabState::OpenedTab)
            )
            ->Split
            (
                FTabManager::NewSplitter()
                ->SetSizeCoefficient(0.65f)
                ->SetOrientation(Orient_Vertical)
                ->Split
                (
                    FTabManager::NewStack()
                    ->SetSizeCoefficient(0.8f)
                    ->SetHideTabWell(true)
                    ->AddTab(GraphTab, ETabState::OpenedTab)
                )
            )
        );

    constexpr bool bCreateDefaultStandaloneMenu = true;
    constexpr bool bCreateDefaultToolbar = true;
    InitAssetEditor(Mode, InitToolkitHost, TEXT("FlowEditorApp"), StandaloneDefaultLayout, bCreateDefaultStandaloneMenu, bCreateDefaultToolbar, ObjectToEdit, false);

    RegenerateMenusAndToolbars();
}

void FMapEventEditor::CreateToolbar()
{
	/*FName ParentToolbarName;
	const FName ToolBarName = GetToolMenuToolbarName(ParentToolbarName);

	UToolMenus* ToolMenus = UToolMenus::Get();
	UToolMenu* FoundMenu = ToolMenus->FindMenu(ToolBarName);
	if (!FoundMenu || !FoundMenu->IsRegistered())
	{
		FoundMenu = ToolMenus->RegisterMenu(ToolBarName, ParentToolbarName, EMultiBoxType::ToolBar);
	}

	if (FoundMenu)
	{
		AssetToolbar = MakeShareable(new FFlowAssetToolbar(SharedThis(this), FoundMenu));
	}*/
}

void FMapEventEditor::BindToolbarCommands()
{

}

void FMapEventEditor::CreateWidgets()
{
    // Details View
    {
        FDetailsViewArgs Args;
        Args.bHideSelectionTip = true;
        Args.bShowPropertyMatrixButton = false;
        Args.DefaultsOnlyVisibility = EEditDefaultsOnlyNodeVisibility::Hide;
        Args.NotifyHook = this;

        FPropertyEditorModule& PropertyModule = FModuleManager::LoadModuleChecked<FPropertyEditorModule>("PropertyEditor");
        DetailsView = PropertyModule.CreateDetailView(Args);
        //DetailsView->SetIsPropertyEditingEnabledDelegate(FIsPropertyEditingEnabled::CreateStatic(&FMapEventEditor::CanEdit));
        DetailsView->SetObject(MapEvent);
    }

    CreateGraphWidget();
}

void FMapEventEditor::CreateGraphWidget()
{
    SAssignNew(GraphEditor, SMapEventGraphEditor, SharedThis(this))
        .DetailsView(DetailsView);
}

// FGCObject
void FMapEventEditor::AddReferencedObjects(FReferenceCollector& Collector)
{
    Collector.AddReferencedObject(MapEvent);
}

// FEditorUndoClient
void FMapEventEditor::PostUndo(bool bSuccess)
{
    HandleUndoTransaction();
}
void FMapEventEditor::PostRedo(bool bSuccess)
{
    HandleUndoTransaction();
}

void FMapEventEditor::HandleUndoTransaction()
{
    SetUISelectionState(NAME_None);
    GraphEditor->NotifyGraphChanged();
    FSlateApplication::Get().DismissAllMenus();
}

void FMapEventEditor::NotifyPostChange(const FPropertyChangedEvent& PropertyChangedEvent, FProperty* PropertyThatChanged)
{
    if (PropertyChangedEvent.ChangeType != EPropertyChangeType::Interactive)
    {
        GraphEditor->NotifyGraphChanged();
    }
}

// IToolkit
FName FMapEventEditor::GetToolkitFName() const
{
    return FName("MapEventEditor");
}

FText FMapEventEditor::GetBaseToolkitName() const
{
    return LOCTEXT("AppLabel", "MapEvent Editor");
}

FString FMapEventEditor::GetWorldCentricTabPrefix() const
{
    return LOCTEXT("WorldCentricTabPrefix", "MapEvent").ToString();
}

FLinearColor FMapEventEditor::GetWorldCentricTabColorScale() const
{
    return FLinearColor(0.3f, 0.2f, 0.5f, 0.5f);
}

void FMapEventEditor::RegisterTabSpawners(const TSharedRef<class FTabManager>& InTabManager)
{
    WorkspaceMenuCategory = InTabManager->AddLocalWorkspaceMenuCategory(LOCTEXT("WorkspaceMenu_MapEventEditor", "Map Event Editor"));
    const auto WorkspaceMenuCategoryRef = WorkspaceMenuCategory.ToSharedRef();

    FAssetEditorToolkit::RegisterTabSpawners(InTabManager);

    InTabManager->RegisterTabSpawner(DetailsTab, FOnSpawnTab::CreateSP(this, &FMapEventEditor::SpawnTab_Details))
        .SetDisplayName(LOCTEXT("DetailsTab", "Details"))
        .SetGroup(WorkspaceMenuCategoryRef)
        .SetIcon(FSlateIcon(FAppStyle::GetAppStyleSetName(), "LevelEditor.Tabs.Details"));

    InTabManager->RegisterTabSpawner(GraphTab, FOnSpawnTab::CreateSP(this, &FMapEventEditor::SpawnTab_Graph))
        .SetDisplayName(LOCTEXT("GraphTab", "Graph"))
        .SetGroup(WorkspaceMenuCategoryRef)
        .SetIcon(FSlateIcon(FAppStyle::GetAppStyleSetName(), "GraphEditor.EventGraph_16x"));

	/*InTabManager->RegisterTabSpawner(PaletteTab, FOnSpawnTab::CreateSP(this, &FFlowAssetEditor::SpawnTab_Palette))
		.SetDisplayName(LOCTEXT("PaletteTab", "Palette"))
		.SetGroup(WorkspaceMenuCategoryRef)
		.SetIcon(FSlateIcon(FAppStyle::GetAppStyleSetName(), "Kismet.Tabs.Palette"));

	InTabManager->RegisterTabSpawner(RuntimeLogTab, FOnSpawnTab::CreateSP(this, &FFlowAssetEditor::SpawnTab_RuntimeLog))
		.SetDisplayName(LOCTEXT("RuntimeLog", "Runtime Log"))
		.SetGroup(WorkspaceMenuCategoryRef)
		.SetIcon(FSlateIcon(FAppStyle::GetAppStyleSetName(), "Kismet.Tabs.CompilerResults"));

	InTabManager->RegisterTabSpawner(SearchTab, FOnSpawnTab::CreateSP(this, &FFlowAssetEditor::SpawnTab_Search))
		.SetDisplayName(LOCTEXT("SearchTab", "Search"))
		.SetGroup(WorkspaceMenuCategoryRef)
		.SetIcon(FSlateIcon(FAppStyle::GetAppStyleSetName(), "Kismet.Tabs.FindResults"));

	InTabManager->RegisterTabSpawner(ValidationLogTab, FOnSpawnTab::CreateSP(this, &FFlowAssetEditor::SpawnTab_ValidationLog))
		.SetDisplayName(LOCTEXT("ValidationLog", "Validation Log"))
		.SetGroup(WorkspaceMenuCategoryRef)
		.SetIcon(FSlateIcon(FAppStyle::GetAppStyleSetName(), "Debug"));*/
}

void FMapEventEditor::UnregisterTabSpawners(const TSharedRef<class FTabManager>& InTabManager)
{
    FAssetEditorToolkit::UnregisterTabSpawners(InTabManager);

    InTabManager->UnregisterTabSpawner(DetailsTab);
    InTabManager->UnregisterTabSpawner(GraphTab);
	/*InTabManager->UnregisterTabSpawner(ValidationLogTab);
	InTabManager->UnregisterTabSpawner(PaletteTab);
	InTabManager->UnregisterTabSpawner(SearchTab);*/
}

// FAssetEditorToolkit
//void FMapEventEditor::InitToolMenuContext(FToolMenuContext& MenuContext)
//{
//    FAssetEditorToolkit::InitToolMenuContext(MenuContext);
//
//    UMapEventEditorContext* Context = NewObject<UMapEventEditorContext>();
//    Context->MapEventEditor = SharedThis(this);
//    MenuContext.AddObject(Context);
//}

//void FMapEventEditor::PostRegenerateMenusAndToolbars()
//{
//    // Provide a hyperlink to view our class
//    const TSharedRef<SHorizontalBox> MenuOverlayBox = SNew(SHorizontalBox)
//        + SHorizontalBox::Slot()
//        .AutoWidth()
//        .VAlign(VAlign_Center)
//        [
//            SNew(STextBlock)
//                .ColorAndOpacity(FSlateColor::UseSubduedForeground())
//                .ShadowOffset(FVector2D::UnitVector)
//                .Text(LOCTEXT("MapEventEditor_AssetType", "Asset Type: "))
//        ]
//        + SHorizontalBox::Slot()
//        .AutoWidth()
//        .VAlign(VAlign_Center)
//        .Padding(0.0f, 0.0f, 8.0f, 0.0f)
//        [
//            FEditorClassUtils::GetSourceLink(MapEvent->GetClass())
//        ];
//
//    SetMenuOverlay(MenuOverlayBox);
//}
//
//void FMapEventEditor::SaveAsset_Execute()
//{
//    DoPresaveAssetUpdate();
//
//    FAssetEditorToolkit::SaveAsset_Execute();
//}
//
// void FMapEventEditor::SaveAssetAs_Execute()
//{
//     DoPresaveAssetUpdate();
//
//     FAssetEditorToolkit::SaveAssetAs_Execute();
//}
//
//
// void FMapEventEditor::DoPresaveAssetUpdate()
// {
//     if (IsValid(MapEvent))
//     {
//         UMapEventGraph* MapEventGraph = Cast<UMapEventGraph>(MapEvent->GetGraph());
//         if (IsValid(MapEventGraph))
//         {
//             MapEventGraph->OnSave();
//         }
//     }
// }

 TSharedRef<SDockTab> FMapEventEditor::SpawnTab_Details(const FSpawnTabArgs& Args) const
 {
     check(Args.GetTabId() == DetailsTab);

     return SNew(SDockTab)
         .Label(LOCTEXT("MapEventDetailsTitle", "Details"))
         [
             DetailsView.ToSharedRef()
         ];
 }

 TSharedRef<SDockTab> FMapEventEditor::SpawnTab_Graph(const FSpawnTabArgs& Args) const
 {
     check(Args.GetTabId() == GraphTab);

     TSharedRef<SDockTab> SpawnedTab = SNew(SDockTab)
         .Label(LOCTEXT("MapEventGraphTitle", "Graph"));

     if (GraphEditor.IsValid())
     {
         SpawnedTab->SetContent(GraphEditor.ToSharedRef());
     }

     return SpawnedTab;
 }

 void FMapEventEditor::SetUISelectionState(const FName SelectionOwner)
 {
     if (SelectionOwner != CurrentUISelection)
     {
         ClearSelectionStateFor(CurrentUISelection);
         CurrentUISelection = SelectionOwner;
     }
 }

 void FMapEventEditor::ClearSelectionStateFor(const FName SelectionOwner)
 {
     if (SelectionOwner == GraphTab)
     {
         GraphEditor->ClearSelectionSet();
     }
 }

//void FMapEventEditor::InitEditor(const EToolkitMode::Type mode, const TSharedPtr<class IToolkitHost>& initToolkitHost, UObject* inObject)
//{
//    TArray<UObject*> objectsToEdit;
//    objectsToEdit.Add(inObject);
//
//    m_MapEvent = Cast<UMapEvent>(inObject);
//
//    m_Graph = FBlueprintEditorUtils::CreateNewGraph(m_MapEvent, NAME_None, UEdGraph::StaticClass(), UMapEventGraphSchema::StaticClass());
//
//    InitAssetEditor(
//        mode,
//        initToolkitHost,
//        TEXT("RPGSystem"),
//        FTabManager::FLayout::NullLayout,
//        true, // createDefaultStandaloneMenu 
//        true,  // createDefaultToolbar
//        objectsToEdit);
//
//    // Add our modes (just one for this example)
//    AddApplicationMode(TEXT("MapEventEditorAppMode"), MakeShareable(new FMapEventEditorAppMode(SharedThis(this))));
//
//    // Set the mode
//    SetCurrentMode(TEXT("MapEventEditorAppMode"));
//
//    UpdateGraphFromMapEvent();
//
//    m_GraphChangeListenerHandle = m_Graph->AddOnGraphChangedHandler(FOnGraphChanged::FDelegate::CreateSP(this, &FMapEventEditor::OnGraphChanged));
//
//}
//
//void FMapEventEditor::OnClose()
//{
//    UpdateMapEventFromGraph();
//    m_Graph->RemoveOnGraphChangedHandler(m_GraphChangeListenerHandle);
//    FAssetEditorToolkit::OnClose();
//}

//void FMapEventEditor::OnGraphChanged(const FEdGraphEditAction& editAction)
//{
//    UpdateMapEventFromGraph();
//}

//void FMapEventEditor::UpdateMapEventFromGraph()
//{
//    if (m_Graph == nullptr || m_MapEvent == nullptr)
//        return;

	/*UMapEventRuntimeGraph* runtimeGraph = NewObject<UMapEventRuntimeGraph>(m_MapEvent);
	m_MapEvent->Graph = runtimeGraph;*/

    //TArray<std::pair<FGuid, FGuid>> connections;
    //TMap<FGuid, UMapEventRuntimePin*> idToPinMap;

    //for (UEdGraphNode* uiNode : m_Graph->Nodes)
    //{
    //    UEventCommandNode* commandNode = Cast<UEventCommandNode>(uiNode);

    //    UMapEventRuntimeNode* runtimeNode = commandNode->GetRuntimeNode();
    //    if (runtimeNode == nullptr)
    //    {
    //        runtimeNode = NewObject<UMapEventRuntimeNode>(m_MapEvent->Graph);
    //        m_MapEvent->Graph->Nodes.Add(runtimeNode);
    //    }
    //    runtimeNode->Position = FVector2D(uiNode->NodePosX, uiNode->NodePosY);

    //    for (UEdGraphPin* uiPin : uiNode->Pins)
    //    {
    //        UMapEventRuntimePin* runtimePin = NewObject<UMapEventRuntimePin>(runtimeNode);
    //        runtimePin->PinName = uiPin->PinName;
    //        runtimePin->PinId = uiPin->PinId;

    //        // Only record the the output side of the connection since this is a directed graph
    //        if (uiPin->HasAnyConnections() && uiPin->Direction == EEdGraphPinDirection::EGPD_Output) {
    //            // Only 1 connection is allowed so just take the first one
    //            std::pair<FGuid, FGuid> connection = std::make_pair(uiPin->PinId, uiPin->LinkedTo[0]->PinId);
    //            connections.Add(connection);
    //        }

    //        idToPinMap.Add(uiPin->PinId, runtimePin);
    //        if (uiPin->Direction == EEdGraphPinDirection::EGPD_Input) {
    //            runtimeNode->EnterPin = runtimePin;
    //        }
    //        else {
    //            runtimeNode->ExitPins.Add(runtimePin);
    //        }
    //    }
    //}

    // Now make all the connections
	/*for (std::pair<FGuid, FGuid> connection : connections) {
		UMapEventRuntimePin* pin1 = idToPinMap[connection.first];
		UMapEventRuntimePin* pin2 = idToPinMap[connection.second];
		pin1->Connection = pin2;
	}
}*/

//void FMapEventEditor::UpdateGraphFromMapEvent()
//{
//    if (m_MapEvent == nullptr)
//    {
//        return;
//    }
//
//    // Create all the nodes/pins first
//    TArray<std::pair<FGuid, FGuid>> connections;
//    TMap<FGuid, UEdGraphPin*> idToPinMap;
//
//    //Start node
//    UEventCommandNode* previousNode = nullptr;
//
//    UEventCommandNode* currentNode = NewObject<UEventCommandNode>(m_Graph);
//    currentNode->Initialize(nullptr);
//
//    IEventCommandInterface* nextCommand = m_MapEvent->GetRootCommand();
//    do {
//        if (UEdGraphPin* enterPin = currentNode->GetInputPin())
//        {
//            connections.Add(std::make_pair(previousNode->GetOutputPin(0)->PinId, enterPin->PinId));
//            idToPinMap.Add(enterPin->PinId, enterPin);
//        }
//
//        UEventCommandNode* nextNode = nullptr;
//        if (UEdGraphPin* exitPin = currentNode->GetOutputPin(0))
//        {
//            if (nextCommand != nullptr)
//            {
//                nextNode = NewObject<UEventCommandNode>(m_Graph);
//                nextNode->Initialize(nextCommand, currentNode);
//                connections.Add(std::make_pair(exitPin->PinId, nextNode->GetInputPin()->PinId));
//            }
//            idToPinMap.Add(exitPin->PinId, exitPin);
//        }
//
//        m_Graph->AddNode(currentNode, true, true);
//
//        previousNode = currentNode;
//        currentNode = nextNode;
//    } while (currentNode != nullptr);
//
//
//    for (std::pair<FGuid, FGuid> connection : connections) {
//        UEdGraphPin* fromPin = idToPinMap[connection.first];
//        UEdGraphPin* toPin = idToPinMap[connection.second];
//        fromPin->LinkedTo.Add(toPin);
//        toPin->LinkedTo.Add(fromPin);
//    }
//}

#undef LOCTEXT_NAMESPACE