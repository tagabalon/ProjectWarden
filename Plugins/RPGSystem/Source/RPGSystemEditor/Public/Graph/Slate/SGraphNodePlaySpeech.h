#pragma once

#include "Graph/Nodes/CommandNode.h"
#include "Graph/Slate/SGraphNodeCommand.h"
//#include "Widgets/Input/SMultiLineEditableTextBox.h"

class UEdGraphNode;
class SMultiLineEditableTextBox;
class UPlaySpeech;

class RPGSYSTEMEDITOR_API SGraphNodePlaySpeech : public SGraphNodeCommand
{
public:
	// Slate Arguments 
	SLATE_BEGIN_ARGS(SGraphNodePlaySpeech) {}
	SLATE_END_ARGS()

	/** Slate Constructor*/
	void Construct(const FArguments& InArgs, UEdGraphNode* InNode);

	/// SGraphNode 
	/*virtual void UpdateGraphNode() override;
	virtual void AddPin(const TSharedRef<SGraphPin>& PinToAdd) override;*/
protected:
	virtual TSharedRef<SWidget> ShowContent();

	/*void OnNameCommitted(const FText& InText, ETextCommit::Type InCommitType);
	void OnTextCommitted(const FText& InText, ETextCommit::Type InCommitType);*/

private:
	UPlaySpeech* PlaySpeechCommand;
	/*TSharedRef<SWidget> ShowFace();

	TSharedPtr<SImage> ImageFace;
	TSharedPtr<SEditableTextBox> Name;
	TSharedPtr<SMultiLineEditableTextBox> Text;*/
};