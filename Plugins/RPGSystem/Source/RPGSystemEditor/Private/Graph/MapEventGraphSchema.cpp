#include "Graph/MapEventGraphSchema.h"

#include "Graph/MapEventGraph.h"
#include "Graph/Nodes/CommandNode.h"
#include "Graph/MapEventGraphSchema_Actions.h"
//#include "EventCommandNode.h"
#include "Commands/ShowMessage.h"

#include "Commands/Start.h"
#include "MapEvent.h"

#include "AssetRegistry/AssetRegistryModule.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(MapEventGraphSchema)

#define LOCTEXT_NAMESPACE "MapEventGraphSchema"

TArray<UClass*> UMapEventGraphSchema::NativeCommandNodes;
TMap<TSubclassOf<UBaseCommand>, TSubclassOf<UEdGraphNode>> UMapEventGraphSchema::GraphNodesByCommandNodes;
bool UMapEventGraphSchema::bAreNodesGathered = false;

//void UMapEventGraphSchema::GetGraphContextActions(FGraphContextMenuBuilder& contextMenuBuilder) const {
//    TSharedPtr<FNewCommandNodeAction> showMessageAction(
//        new FNewCommandNodeAction(
//            FText::FromString(TEXT("Messages")),
//            FText::FromString(TEXT("Show Text")),
//            FText::FromString(TEXT("Opens the Message Window and displays text.")),
//            0,
//            UShowMessage::StaticClass()
//        )
//    );
//
//    contextMenuBuilder.AddAction(showMessageAction);
//}

UMapEventGraphSchema::UMapEventGraphSchema(const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer)
{
}

void UMapEventGraphSchema::SubscribeToAssetChanges()
{
    const FAssetRegistryModule& AssetRegistry = FModuleManager::LoadModuleChecked<FAssetRegistryModule>(AssetRegistryConstants::ModuleName);
    AssetRegistry.Get().OnFilesLoaded().AddStatic(&UMapEventGraphSchema::GatherNodes);
}

void UMapEventGraphSchema::GetGraphContextActions(FGraphContextMenuBuilder& ContextMenuBuilder) const
{
    UEdGraph* Graph = const_cast<UEdGraph*>(ContextMenuBuilder.CurrentGraph);
    UClass* GraphNodeClass = UCommandNode::StaticClass();

    const UMapEvent* EditedMapEvent = GetEditedAssetOrClassDefault(ContextMenuBuilder.CurrentGraph);

    TArray<UBaseCommand*> CommandList = GetCommandList();

    for (UBaseCommand* Command : CommandList)
    {
        UCommandNode* CommandNode = NewObject<UCommandNode>(Graph, GraphNodeClass);
        CommandNode->EventCommandClass = Command->GetClass();

        TSharedPtr<FMapEventGraphSchemaAction_NewNode> NewNodeAction = TSharedPtr<FMapEventGraphSchemaAction_NewNode>(
            new FMapEventGraphSchemaAction_NewNode(Command));

        if(ContextMenuBuilder.SelectedObjects.Num() > 0)
            NewNodeAction->ParentNode = Cast<UCommandNode>(ContextMenuBuilder.SelectedObjects[0]);
        ContextMenuBuilder.AddAction(NewNodeAction);
    }
}

void UMapEventGraphSchema::CreateDefaultNodesForGraph(UEdGraph& Graph) const
{
    const UMapEvent* AssetClassDefaults = GetEditedAssetOrClassDefault(&Graph);
    static const FVector2D NodeOffsetIncrement = FVector2D(0, 128);
    FVector2D NodeOffset = FVector2D::ZeroVector;

    // Start node
    CreateCommand(Graph, AssetClassDefaults, UStart::StaticClass(), NodeOffset);
}

UCommandNode* UMapEventGraphSchema::CreateCommand(UEdGraph& Graph, const UMapEvent* AssetClassDefaults, const TSubclassOf<UBaseCommand>& EventCommandClass, const FVector2D& Offset)
{
    UCommandNode* NewCommandNode = FMapEventGraphSchemaAction_NewNode::CreateNode(&Graph, nullptr, EventCommandClass, Offset);


    

    SetNodeMetaData(NewCommandNode, FNodeMetadata::DefaultGraphNode);

    return NewCommandNode;
}

TSubclassOf<UEdGraphNode> UMapEventGraphSchema::GetAssignedGraphNodeClass(const TSubclassOf<UBaseCommand>& EventCommandClass)
{
    TArray<TSubclassOf<UBaseCommand>> FoundParentClasses;
    UClass* ReturnClass = nullptr;

    for (const TPair<TSubclassOf<UBaseCommand>, TSubclassOf<UEdGraphNode>>& GraphNodeByCommandNode : GraphNodesByCommandNodes)
    {
        if (EventCommandClass == GraphNodeByCommandNode.Key)
        {
            return GraphNodeByCommandNode.Value;
        }

        if (EventCommandClass->IsChildOf(GraphNodeByCommandNode.Key))
        {
            FoundParentClasses.Add(GraphNodeByCommandNode.Key);
        }
    }
	if (FoundParentClasses.Num() == 1)
	{
		ReturnClass = GraphNodesByCommandNodes.FindRef(FoundParentClasses[0]);
	}

	return IsValid(ReturnClass) ? ReturnClass : UCommandNode::StaticClass();
}

void UMapEventGraphSchema::GatherNodes()
{
    // prevent asset crunching during PIE
    if (GEditor && GEditor->PlayWorld)
    {
        return;
    }

    // prevent adding assets while compiling blueprints
    //  (because adding assets can cause blueprint compiles to be queued as a side-effect (via GetPlaceableNodeOrAddOnBlueprint))
    if (GCompilingBlueprint)
    {
        return;
    }

    TArray<UClass*> CommandNodes;
    GetDerivedClasses(UBaseCommand::StaticClass(), NativeCommandNodes);

    TArray<UClass*> GraphNodes;
    GetDerivedClasses(UCommandNode::StaticClass(), GraphNodes);

    for (UClass* GraphNodeClass : GraphNodes)
    {
        const UCommandNode* GraphNodeDefault = GraphNodeClass->GetDefaultObject<UCommandNode>();
        for (UClass* AssignedClass : GraphNodeDefault->AssignedCommandClasses)
        {
            if (AssignedClass->IsChildOf(UBaseCommand::StaticClass()))
            {
                GraphNodesByCommandNodes.Emplace(AssignedClass, GraphNodeClass);
            }
        }
    }

    bAreNodesGathered = true;
}

const FPinConnectionResponse UMapEventGraphSchema::CanCreateConnection(const UEdGraphPin* a, const UEdGraphPin* b) const {
    // Neither can be nullptr
    if (a == nullptr || b == nullptr) {
        return FPinConnectionResponse(CONNECT_RESPONSE_DISALLOW, TEXT("Need 2 pins"));
    }

    // Make sure the directions are different (no inputs to inputs or outputs to outputs)
    if (a->Direction == b->Direction) {
        return FPinConnectionResponse(CONNECT_RESPONSE_DISALLOW, TEXT("Inputs can only connect to outputs"));
    }

    // Else allow it but break all connections
    return FPinConnectionResponse(CONNECT_RESPONSE_BREAK_OTHERS_AB, TEXT(""));
}
//
//UEdGraphNode* FNewCommandNodeAction::PerformAction(UEdGraph* parentGraph, UEdGraphPin* fromPin, const FVector2D location, bool bSelectNewNode) {
//
//    if (m_CommandType != nullptr)
//    {
//        UEventCommandNode* result = NewObject<UEventCommandNode>(parentGraph);
//
//		    UEventCommandNode* previousNode = nullptr;
//		 if (fromPin != nullptr)
//		 {
//			 previousNode = Cast<UEventCommandNode>(fromPin->GetOwningNode());
//		 }
//
//         UBaseCommand* baseCommand = m_CommandType->GetDefaultObject<UBaseCommand>();
//		 result->Initialize(baseCommand, previousNode);
//
//		 result->NodePosX = location.X;
//		 result->NodePosY = location.Y;
//
//		 parentGraph->Modify();
//		 parentGraph->AddNode(result, true, true);
//
//		 if (previousNode != nullptr)
//		 {
//			 if (const UEdGraphSchema* schema = result->GetSchema())
//			 {
//				 schema->TryCreateConnection(fromPin, result->GetInputPin());
//			 }
//		 }
//
//        return result;
//    }
//
//    return nullptr;
//}

const UMapEvent* UMapEventGraphSchema::GetEditedAssetOrClassDefault(const UEdGraph* EdGraph)
{
    if (const UMapEventGraph* MapEventGraph = Cast<UMapEventGraph>(EdGraph))
    {
        if (UMapEvent* MapEvent = MapEventGraph->GetMapEvent())
        {
            return MapEvent;
        }
    }

    const UClass* AssetClass = UMapEvent::StaticClass();
    return AssetClass->GetDefaultObject<UMapEvent>();
}

TArray<UBaseCommand*> UMapEventGraphSchema::GetCommandList()
{
    if (!bAreNodesGathered)
    {
        GatherNodes();
    }

    TArray<UBaseCommand*> CommandList;

    for (UClass* NativeCommandNode : NativeCommandNodes)
    {
        UBaseCommand* CommandDefaults = NativeCommandNode->GetDefaultObject<UBaseCommand>();
        if (CommandDefaults->GetInputPins() > 0)
        {
            CommandList.Emplace(CommandDefaults);
        }
    }

    return CommandList;
}