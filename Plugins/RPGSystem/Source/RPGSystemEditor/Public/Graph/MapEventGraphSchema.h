#pragma once

#include "EdGraph/EdGraphSchema.h"
#include "Commands/BaseCommand.h"

#include "EdGraph/EdGraph.h"
#include "Templates/SubclassOf.h"

#include "MapEventGraphSchema.generated.h"

class UCommandNode;
class UMapEvent;

UCLASS()
class RPGSYSTEMEDITOR_API UMapEventGraphSchema : public UEdGraphSchema
{
	GENERATED_UCLASS_BODY()

	friend class UMapEventGraph;

private:
	static TArray<UClass*> NativeCommandNodes;
	static TMap<TSubclassOf<UBaseCommand>, TSubclassOf<UEdGraphNode>> GraphNodesByCommandNodes;

public:
    virtual const FPinConnectionResponse CanCreateConnection(const UEdGraphPin* a, const UEdGraphPin* b) const override;

	// EdGraphSchema
	virtual void GetGraphContextActions(FGraphContextMenuBuilder& ContextMenuBuilder) const override;
	virtual void CreateDefaultNodesForGraph(UEdGraph& Graph) const override;

	static void SubscribeToAssetChanges();
	static const UMapEvent* GetEditedAssetOrClassDefault(const UEdGraph* Graph);
	static TSubclassOf<UEdGraphNode> GetAssignedGraphNodeClass(const TSubclassOf<UBaseCommand>& EventCommandClass);


protected:
	static UCommandNode* CreateCommand(UEdGraph& Graph, const UMapEvent* AssetClassDefaults, const TSubclassOf<UBaseCommand>& EventCommandClass, const FVector2D& Offset);
	static void GatherNodes();

private:
	static TArray<UBaseCommand*> GetCommandList();

	static bool bAreNodesGathered;
};

//USTRUCT()
//struct FNewCommandNodeAction : public FEdGraphSchemaAction {
//	GENERATED_BODY();
//
//public:
//	FNewCommandNodeAction() {}
//	FNewCommandNodeAction(FText inNodeCategory, FText inMenuDesc, FText inToolTip, const int32 inGrouping, UClass* commandType)
//	    : FEdGraphSchemaAction(inNodeCategory, inMenuDesc, inToolTip, inGrouping)
//	{
//		m_CommandType = commandType;
//	}
//
//	virtual UEdGraphNode* PerformAction(UEdGraph* parentGraph, UEdGraphPin* fromPin, const FVector2D location, bool bSelectNewNode = true) override;
//
//private:
//	TSubclassOf<UBaseCommand> m_CommandType;
//};