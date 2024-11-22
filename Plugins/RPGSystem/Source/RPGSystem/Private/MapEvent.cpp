// Fill out your copyright notice in the Description page of Project Settings.


#include "MapEvent.h"

#include "Commands/Start.h"

#include "GameFramework/PlayerController.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(MapEvent)

UMapEvent::UMapEvent(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
//#if WITH_EDITOR
//	, Graph(nullptr)
//#endif
{
	/*UStart* StartCommand = CreateDefaultSubobject<UStart>("Start");
	Commands.Add(StartCommand);*/
	//RootCommand = StartCommand;
}

void UMapEvent::Execute(APlayerController* Player, AMapEventActor* MapEventActor)
{
	if (Commands.Num() > 0)
	{
		Commands[0]->Execute(Player, MapEventActor);
	}
	//if (RootCommand != nullptr)
	//	RootCommand->Execute(Player);
}

#if WITH_EDITOR
UBaseCommand* UMapEvent::CreateCommand(const UClass* EventCommandClass, UEdGraphNode* GraphNode, UBaseCommand* FromCommand)
{
	UBaseCommand* NewCommand = NewObject<UBaseCommand>(this, EventCommandClass, EventCommandClass->GetFName(), RF_Transactional);
	NewCommand->SetGraphNode(GraphNode);
	Commands.Add(NewCommand);

	if (FromCommand != nullptr && Commands.Contains(FromCommand))
	{
		FromCommand->SetNextCommand(NewCommand);
	}

	//RegisterCommand(GraphNode->NodeGuid, NewCommand);

	//return NewCommand;
	return NewCommand;
}

void UMapEvent::RegisterCommand(const FGuid& NewGuid, UBaseCommand* NewCommand)
{
	NewCommand->SetGuid(NewGuid);
	Commands.Add(NewCommand);
}
#endif
