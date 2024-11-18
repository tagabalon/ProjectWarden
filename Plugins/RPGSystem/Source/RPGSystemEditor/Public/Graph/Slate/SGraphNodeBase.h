#pragma once

#include "CoreMinimal.h"
#include "SGraphNode.h"

#include "Graph/Nodes/CommandNode.h"

class UEdGraphNode;

class RPGSYSTEMEDITOR_API SGraphNodeBase : public SGraphNode
{
public:
	// Slate Arguments 
	SLATE_BEGIN_ARGS(SGraphNodeBase) {}
	SLATE_END_ARGS()

	/** Slate Constructor*/
	void Construct(const FArguments& InArgs, UEdGraphNode* InNode);

	/// SGraphNode 
	virtual void UpdateGraphNode() override;
	virtual void AddPin(const TSharedRef<SGraphPin>& PinToAdd) override;

protected:
	TObjectPtr<UCommandNode> CommandNode;
};