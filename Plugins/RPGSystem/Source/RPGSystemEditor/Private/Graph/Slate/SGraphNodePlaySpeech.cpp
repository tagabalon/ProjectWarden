#include "Graph/Slate/SGraphNodePlaySpeech.h"

#include "Commands/PlaySpeech.h"
#include "MapEvent.h"

void SGraphNodePlaySpeech::Construct(const FArguments& InArgs, UEdGraphNode* InNode)
{
	check(InNode);
	GraphNode = InNode;
	SetCursor(EMouseCursor::Default);

	CommandNode = Cast<UCommandNode>(GraphNode);
	check(CommandNode);

	PlaySpeechCommand = Cast<UPlaySpeech>(CommandNode->GetCommandData());

	UpdateGraphNode();
}


TSharedRef<SWidget> SGraphNodePlaySpeech::ShowContent()
{
	return SNew(SBorder)
		.HAlign(HAlign_Fill)
		.VAlign(VAlign_Fill);
}