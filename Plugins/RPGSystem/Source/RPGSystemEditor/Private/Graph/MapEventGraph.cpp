#include "Graph/MapEventGraph.h"

#include "Graph/MapEventGraphSchema.h"
#include "Graph/Nodes/CommandNode.h"

#include "Editor.h"
#include "Kismet2/BlueprintEditorUtils.h"

UMapEventGraph::UMapEventGraph()
{
	AddOnGraphChangedHandler(FOnGraphChanged::FDelegate::CreateUObject(this, &UMapEventGraph::OnMapEventGraphChanged));

}

bool UMapEventGraph::TryCreateNodes(UMapEvent* MapEvent)
{
	if (MapEvent == nullptr)
	{
		return false;
	}

	TArray<IEventCommandInterface*> MapCommands;
	MapEvent->GetCommands(MapCommands);
	if (MapCommands.Num() == 0)
	{
		return false;
	}

	if (!Nodes.IsEmpty() && MapEvent->GetGraph() == this)
	{
		//Remove Commands that already have a Node
		FilterUngeneratedCommands(MapCommands);

		if (MapCommands.Num() > 0)
		{
			GenerateCommands(MapCommands);
		}
	}

	MapEvent->SetGraph(this);
	MapEvent->GetGraph()->bAllowDeletion = false;
	StartNode = FindNodeOf(MapEvent->GetRootCommand());
	return true;
}

void UMapEventGraph::FilterUngeneratedCommands(TArray<IEventCommandInterface*>& MapCommands)
{
	for(int32 Index = MapCommands.Num() - 1; Index >= 0; Index--)	
	{
		if (FindNodeOf(Cast<UBaseCommand>(MapCommands[Index])))
		{
			MapCommands.RemoveAt(Index);
		}
	}
}

UCommandNode* UMapEventGraph::GetRootCommandNode() const
{
	return StartNode.Get();
}

void UMapEventGraph::GenerateCommands(TArray<IEventCommandInterface*>& MapCommands)
{

}

void UMapEventGraph::VerifyNodeLinks()
{

}

void UMapEventGraph::SetStartNode(UCommandNode* InStartNode)
{
	StartNode = InStartNode;
}

void UMapEventGraph::OnMapEventGraphChanged(const FEdGraphEditAction& EditAction)
{
	TArray<UCommandNode*> ChildrenNodes;
	GetNodesOfClass<UCommandNode>(ChildrenNodes);

	//Loading pins
	for (UCommandNode* Node : ChildrenNodes)
	{
		Node->LoadPins();
	}
	for (UCommandNode* Node : ChildrenNodes)
	{
		Node->RegenerateNodeConnections(this);
	}

	//if (ChildrenNodes.Num() > 1)
	//{
	//	for (UCommandNode* Node : ChildrenNodes)
	//	{
	//		if (UBaseCommand* Command = Node->GetCommandData())
	//		{
	//			if (UObject* Outer = Command->GetOuter())
	//			{
	//				if (UMapEvent* MapEvent = Cast<UMapEvent>(Outer))
	//				{
	//					MapEvent->Modify();
	//					//RootNode = FindNodeOf(MapEvent->GetRootCommand());
	//					break;
	//				}
	//			}
	//		}
	//	}
	//}

	/*Modify(false);
	for (UCommandNode* Node : ChildrenNodes)
	{
		Node->Modify();
		Node->ReconstructNode();
	}
	for (UCommandNode* Node : ChildrenNodes)
	{
		Node->RegenerateNodeConnections(this);
	}*/
}

UCommandNode* UMapEventGraph::FindNodeOf(UBaseCommand* Command)
{
	if (Command != nullptr)
	{
		TArray<UCommandNode*> ChildrenNodes;
		GetNodesOfClass<UCommandNode>(ChildrenNodes);
		for (UCommandNode* Node : ChildrenNodes)
		{
			if (Node->GetCommandData() == Command)
			{
				return Node;
			}
		}
	}
	return nullptr;
}


void UMapEventGraph::UpdateNodeOf(UBaseCommand* InCommand)
{
	TArray<UCommandNode*> ChildrenNodes;
	GetNodesOfClass<UCommandNode>(ChildrenNodes);
	for (UCommandNode* Node : ChildrenNodes)
	{
		if (Node->GetCommandData() == InCommand)
		{
			Node->ReconstructNode();
			break;
		}
	}
}

















//bool UMapEventGraph::TryBuildGraphFromMapEvent(const UMapEvent* InMapEvent)
//{
//	Nodes.Empty();
//	return false;
//}
//
//// UEdGraph
//void UMapEventGraph::NotifyGraphChanged()
//{
//	Super::NotifyGraphChanged();
//}
//// --
//
//UMapEvent* UMapEventGraph::GetMapEvent() const
//{
//	return GetTypedOuter<UMapEvent>();
//}
//
//void UMapEventGraph::Serialize(FArchive& Ar)
//{
//
//}
//
//void UMapEventGraph::CollectAllNodeInstances(TSet<UObject*>& NodeInstances)
//{
//
//}
//bool UMapEventGraph::CanRemoveNestedObject(UObject* TestObject) const
//{
//	return false;
//}
//
//void UMapEventGraph::OnNodeInstanceRemoved(UObject* NodeInstance)
//{
//
//}
//
//void UMapEventGraph::UpdateAsset(int32 UpdateFlags)
//{
//	if (IsLocked())
//	{
//		return;
//	}
//
//	for (UEdGraphNode* EdNode : Nodes)
//	{
//		if (UCommandNode* CommandNode = Cast<UCommandNode>(EdNode))
//		{
//			CommandNode->OnUpdateMapEvent(UpdateFlags);
//		}
//	}
//}
//
//void UMapEventGraph::OnLoaded()
//{
//	check(GEditor);
//
//	for (UEdGraphNode* EdNode : Nodes)
//	{
//		UCommandNode* CommandNode = Cast<UCommandNode>(EdNode);
//		if (IsValid(CommandNode))
//		{
//			UBaseCommand* CommandData = CommandNode->GetCommandData();
//			if (IsValid(CommandData))
//			{
//				CommandData->SetGraphNode(CommandNode);
//			}
//		}
//	}
//
//	Refresh();
//}
//
//void UMapEventGraph::OnSave()
//{
//	UpdateAsset();
//}
//
//void UMapEventGraph::Refresh()
//{
//	if (!GEditor || GEditor->PlayWorld)
//	{
//		return;
//	}
//
//	{
//		LockUpdates();
//
//		if (const UEdGraphSchema* EdSchema = GetSchema())
//		{
//			const UMapEventGraphSchema* MapEventGraphSchema = CastChecked<UMapEventGraphSchema>(EdSchema);
//			MapEventGraphSchema->GatherNodes();
//
//			for (UBaseCommand* Command : GetMapEvent()->GetCommands())
//			{
//				if (!IsValid(Command))
//				{
//					continue;
//				}
//
//				UCommandNode* const ExistingCommandNode = Cast<UCommandNode>(Command->GetGraphNode());
//				UCommandNode* RefreshedCommandNode = ExistingCommandNode;
//
//				const TSubclassOf<UEdGraphNode> ExpectGraphNodeClass = UMapEventGraphSchema::GetAssignedGraphNodeClass(Command->GetClass());
//				UClass* ExistingGraphNodeClass = IsValid(ExistingCommandNode) ? ExistingCommandNode->GetClass() : nullptr;
//				if (ExistingGraphNodeClass != ExpectGraphNodeClass)
//				{
//
//				}
//			}
//		}
//
//		UnlockUpdates();
//	}
//
//	TArray<UCommandNode*> CommandNodes;
//	GetNodesOfClass<UCommandNode>(CommandNodes);
//	for (UCommandNode* CommandNode : CommandNodes)
//	{
//		CommandNode->GetGraph()->NotifyGraphChanged();
//	}
//}