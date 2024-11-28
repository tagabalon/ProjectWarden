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
	if (RootCommand != nullptr)
	{
		RootCommand->Execute(Player, MapEventActor);
	}
	//if (RootCommand != nullptr)
	//	RootCommand->Execute(Player);
}

#if WITH_EDITOR

UBaseCommand* UMapEvent::CreateCommand(const UClass* EventCommandClass, UEdGraphNode* GraphNode, UBaseCommand* FromCommand)
{
	UBaseCommand* NewCommand = NewObject<UBaseCommand>(this, EventCommandClass, NAME_None, RF_Transactional);
	NewCommand->SetGraphNode(GraphNode);

	if (RootCommand == nullptr)
	{
		RootCommand = NewCommand;
	}

	if (FromCommand != nullptr)
	{
		if(Contains(FromCommand))
		{
			FromCommand->SetNextCommand(NewCommand);
		}
	}
	return NewCommand;
}

bool UMapEvent::Contains(IEventCommandInterface* Command) const
{
	TArray<IEventCommandInterface*> Commands;
	GetCommands(Commands);
	return Commands.Contains(Command);
}

void UMapEvent::GetCommands(TArray<IEventCommandInterface*>& Commands) const
{
	Commands.Empty();
	IEventCommandInterface* Curr = RootCommand;
	while (Curr != nullptr) {
		Commands.AddUnique(Curr);

		Curr = Curr->GetNextCommand();
		if (Curr != nullptr && Commands.Contains(Curr))
		{
			break;
		}
	}
}

#endif
