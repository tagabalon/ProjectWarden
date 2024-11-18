#pragma once

#include "Graph/Nodes/CommandNode.h"
#include "Graph/Slate/SGraphNodeCommand.h"
#include "Widgets/Input/SMultiLineEditableTextBox.h"

class UEdGraphNode;
class SMultiLineEditableTextBox;
class UShowText;

class RPGSYSTEMEDITOR_API SGraphNodeShowText : public SGraphNodeCommand
{
public:
	// Slate Arguments 
	SLATE_BEGIN_ARGS(SGraphNodeCommand) {}
	SLATE_END_ARGS()

	/** Slate Constructor*/
	void Construct(const FArguments& InArgs, UEdGraphNode* InNode);

	/// SGraphNode 
	/*virtual void UpdateGraphNode() override;
	virtual void AddPin(const TSharedRef<SGraphPin>& PinToAdd) override;*/
protected:
	virtual TSharedRef<SWidget> ShowContent();

	void OnNameCommitted(const FText& InText, ETextCommit::Type InCommitType);
	void OnTextCommitted(const FText& InText, ETextCommit::Type InCommitType);

private:
	TSharedRef<SWidget> ShowFace();

	UShowText* ShowTextCommand;
	TSharedPtr<SImage> ImageFace;
	TSharedPtr<SEditableTextBox> Name;
	TSharedPtr<SMultiLineEditableTextBox> Text;
};