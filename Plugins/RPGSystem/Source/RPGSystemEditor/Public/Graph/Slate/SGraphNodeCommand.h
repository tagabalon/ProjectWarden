#pragma once

#include "CoreMinimal.h"
#include "SGraphNode.h"

#include "Graph/Nodes/CommandNode.h"

class UEdGraphNode;

class RPGSYSTEMEDITOR_API SGraphNodeCommand : public SGraphNode
{
public:
	// Slate Arguments 
	SLATE_BEGIN_ARGS(SGraphNodeCommand) {}
	SLATE_END_ARGS()

	/** Slate Constructor*/
	void Construct(const FArguments& InArgs, UEdGraphNode* InNode);

	/// SGraphNode 
	virtual void UpdateGraphNode() override;
	virtual void AddPin(const TSharedRef<SGraphPin>& PinToAdd) override;
	virtual void CreatePinWidgets() override;

protected:
	virtual TSharedRef<SWidget> ShowLabelAndValue(FText Label, const TSharedRef<SWidget>& Value, FText Tooltip);
	virtual TSharedRef<SWidget> ShowContent();

	TObjectPtr<UCommandNode> CommandNode;
};