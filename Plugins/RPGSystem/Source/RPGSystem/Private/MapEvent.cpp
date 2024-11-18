// Fill out your copyright notice in the Description page of Project Settings.


#include "MapEvent.h"

#include "GameFramework/PlayerController.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(MapEvent)

UMapEvent::UMapEvent(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
#if WITH_EDITOR
	, Graph(nullptr)
#endif
{
}

void UMapEvent::Execute(APlayerController* Player)
{
	if (RootCommand != nullptr)
		RootCommand->Execute(Player);
}

#if WITH_EDITOR
UBaseCommand* UMapEvent::CreateCommand(const UClass* EventCommandClass, UEdGraphNode* GraphNode)
{
	UBaseCommand* NewCommand = NewObject<UBaseCommand>(this, EventCommandClass, FName(), RF_Transactional);
	NewCommand->SetGraphNode(GraphNode);

	RegisterCommand(GraphNode->NodeGuid, NewCommand);

	if (RootCommand == nullptr)
	{
		RootCommand = NewCommand;
	}
	return NewCommand;
}

void UMapEvent::RegisterCommand(const FGuid& NewGuid, UBaseCommand* NewCommand)
{
	NewCommand->SetGuid(NewGuid);
	Commands.Emplace(NewGuid, NewCommand);
}
#endif
