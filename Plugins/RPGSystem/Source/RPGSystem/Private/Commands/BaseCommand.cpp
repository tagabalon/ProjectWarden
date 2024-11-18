#include "Commands/BaseCommand.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(BaseCommand)

UBaseCommand::UBaseCommand(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
	, GraphNode(nullptr)
{
}

void UBaseCommand::Execute(APlayerController* Player)
{
	if (NextCommand == this)
		return;

	if (NextCommand != nullptr)
	{
		NextCommand->Execute(Player);
	}
}

#if WITH_EDITOR
void UBaseCommand::SetGraphNode(UEdGraphNode* NewGraphNode)
{
	GraphNode = NewGraphNode;
}
#endif