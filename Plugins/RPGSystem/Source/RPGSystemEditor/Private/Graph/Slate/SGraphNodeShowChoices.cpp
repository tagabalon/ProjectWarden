#include "Graph/Slate/SGraphNodeShowChoices.h"

#include "Commands/ShowChoices.h"
#include "MapEvent.h"

void SGraphNodeShowChoices::Construct(const FArguments& InArgs, UEdGraphNode* InNode)
{
	check(InNode);
	GraphNode = InNode;
	SetCursor(EMouseCursor::Default);

	CommandNode = Cast<UCommandNode>(GraphNode);
	check(CommandNode);

	ShowChoicesCommand = Cast<UShowChoices>(CommandNode->GetCommandData());

	UpdateGraphNode();
}


TSharedRef<SWidget> SGraphNodeShowChoices::ShowContent()
{
	return SNew(SBorder)
		.HAlign(HAlign_Fill)
		.VAlign(VAlign_Fill);
}