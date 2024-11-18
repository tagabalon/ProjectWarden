#pragma once

#include "EdGraph/EdGraphSchema.h"

#include "Nodes/CommandNode.h"
#include "Commands/BaseCommand.h"
#include "MapEventGraphSchema_Actions.generated.h"

/** Action to add a node to the graph */
USTRUCT()
struct RPGSYSTEMEDITOR_API FMapEventGraphSchemaAction_NewNode : public FEdGraphSchemaAction
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY()
	class UClass* EventCommandClass;

	static FName StaticGetTypeId()
	{
		static FName Type("MapEventGraphSchema_Action_NewNode");
		return Type;
	}

	virtual FName GetTypeId() const override { return StaticGetTypeId(); }

	FMapEventGraphSchemaAction_NewNode()
		: FEdGraphSchemaAction()
		, EventCommandClass(nullptr)
	{
	}

	FMapEventGraphSchemaAction_NewNode(UClass* Node)
		: FEdGraphSchemaAction()
		, EventCommandClass(Node)
	{
	}

	FMapEventGraphSchemaAction_NewNode(const UBaseCommand* Command)
		: FEdGraphSchemaAction(Command->GetCategory(), Command->GetLabel(), FText::FromString("Tooltip"), 0, FText(), 0)
		, EventCommandClass(Command->GetClass())
	{
	}

	// FEdGraphSchemaAction
	virtual UEdGraphNode* PerformAction(class UEdGraph* ParentGraph, UEdGraphPin* FromPin, const FVector2D Location, bool bSelectNewNode = true) override;
	// --

	static UCommandNode* CreateNode(class UEdGraph* Graph, UEdGraphPin* FromPin, const UClass* EventCommandClass, const FVector2D Location, const bool bSelectNewNode = true);
	/*static UCommandNode* RecreateNode(UEdGraph* ParentGraph, UEdGraphNode* OldInstance, UFlowNode* FlowNode);
	static UCommandNode* ImportNode(class UEdGraph* ParentGraph, UEdGraphPin* FromPin, const UClass* NodeClass, const FGuid& NodeGuid, const FVector2D Location);*/
	
	/** parent node */
	UPROPERTY()
	TObjectPtr<UCommandNode> ParentNode;
};