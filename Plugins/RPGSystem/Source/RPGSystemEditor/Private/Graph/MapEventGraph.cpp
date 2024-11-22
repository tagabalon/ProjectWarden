#include "Graph/MapEventGraph.h"

#include "Graph/MapEventGraphSchema.h"
#include "Graph/Nodes/CommandNode.h"

#include "Editor.h"
#include "Kismet2/BlueprintEditorUtils.h"

UMapEventGraph::UMapEventGraph()
{
	AddOnGraphChangedHandler(FOnGraphChanged::FDelegate::CreateUObject(this, &UMapEventGraph::OnMapEventGraphChanged));

}

//UCommandNode* UMapEventGraph::CreateCommandNode(const UClass* EventCommandClass)
//{
//	UCommandNode* NewCommandNode = CreateDefaultSubobject<UCommandNode>(FName(EventCommandClass->GetName()));
//	NewCommandNode->CreateNewGuid();
//	AddNode(NewCommandNode, false, false);
//
//	return NewCommandNode;
//}

void UMapEventGraph::SetStartNode(UCommandNode* InStartNode)
{
	StartNode = InStartNode;
}

void UMapEventGraph::OnMapEventGraphChanged(const FEdGraphEditAction& EditAction)
{

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