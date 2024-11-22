#include "Graph/MapEventGraphSchema_Actions.h"

#include "Graph/MapEventGraph.h"
#include "Graph/MapEventGraphEditor.h"
#include "Graph/MapEventGraphSchema.h"
#include "Graph/Nodes/CommandNode.h"

#include "MapEvent.h"
#include "Commands/BaseCommand.h"

#include "EdGraph/EdGraph.h"
#include "Editor.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(MapEventGraphSchema_Actions)

#define LOCTEXT_NAMESPACE "MapEventGraphSchema_Actions"

UEdGraphNode* FMapEventGraphSchemaAction_NewNode::PerformAction(class UEdGraph* ParentGraph, UEdGraphPin* FromPin, const FVector2D Location, bool bSelectNewNode /* = true */)
{
	// prevent adding new nodes while playing
	if (GEditor->PlayWorld != nullptr)
	{
		return nullptr;
	}

	if (EventCommandClass != nullptr)
	{
		const FScopedTransaction Transaction(LOCTEXT("AddCommand", "Add command"));
		ParentGraph->Modify();
		if (FromPin)
		{
			FromPin->Modify();
		}

		FGraphNodeCreator<UCommandNode> NodeCreator(*ParentGraph);
		UCommandNode* CommandNode = NodeCreator.CreateNode();
		NodeCreator.Finalize();

		CommandNode->SetFlags(RF_Transactional);

		const UEdGraphSchema* Schema = ParentGraph->GetSchema();
		Schema->SetNodeMetaData(CommandNode, FNodeMetadata::DefaultGraphNode);

		UBaseCommand* FromCommand = nullptr;
		if (FromPin != nullptr)
		{
			FromPin->Modify();
			if (UCommandNode* FromNode = Cast<UCommandNode>(FromPin->GetOwningNode()))
			{
				FromCommand = FromNode->GetCommandData();
			}
		}

		if (UMapEvent* MapEvent = ParentGraph->GetTypedOuter<UMapEvent>())
		{
			UBaseCommand* NewCommand = MapEvent->CreateCommand(EventCommandClass, CommandNode, FromCommand);
			CommandNode->SetCommandData(NewCommand);
		}

		CommandNode->NodePosX = Location.X;
		CommandNode->NodePosY = Location.Y;
		CommandNode->AllocateDefaultPins();
		CommandNode->AutowireNewNode(FromPin);



		//UCommandNode* NewCommandNode = UCommandNode::StaticClass()->GetDefaultObject();

		//EventCommandClass->SetFlags(RF_Transactional);

		////Setup the node within the graph 
		//PlaceNodeInGraph(ParentGraph, Location, FromPin);

		return CommandNode;


		//return CreateNode(ParentGraph, FromPin, EventCommandClass, Location, bSelectNewNode);
	}

	return nullptr;
}

//UCommandNode* FMapEventGraphSchemaAction_NewNode::CreateNode(class UEdGraph* Graph, UEdGraphPin* FromPin, const UClass* EventCommandClass, const FVector2D Location, const bool bSelectNewNode/* = true*/)
//{
//    check(EventCommandClass);
//
//    const FScopedTransaction Transaction(LOCTEXT("AddNode", "Add Node"));
//
//	if (UMapEventGraph* MapEventGraph = CastChecked<UMapEventGraph>(Graph))
//	{
//		MapEventGraph->Modify();
//
//		UCommandNode* NewCommandNode = MapEventGraph->CreateCommandNode(EventCommandClass);
//
//	}
//
//
//	/*UBaseCommand* Command = MapEvent->CreateCommand(EventCommandClass, NewCommandNode);
//	NewCommandNode->SetCommandData(Command);
//
//	if (FromPin != nullptr)
//	{
//		UCommandNode* FromNode = Cast<UCommandNode>(FromPin->GetOwningNode());
//		UBaseCommand* FromCommand = FromNode->GetCommandData();
//		FromCommand->SetNextCommand(Command);
//	}*/
//
//    NewCommandNode->AllocateDefaultPins();
//    NewCommandNode->AutowireNewNode(FromPin);
//
//    NewCommandNode->NodePosX = Location.X;
//    NewCommandNode->NodePosY = Location.Y;
//
//    NewCommandNode->PostPlacedNewNode();
//
//    Graph->NotifyGraphChanged();
//
//    //MapEvent->PostEditChange();
//
//    return NewCommandNode;
//}