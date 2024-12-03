#include "Graph/Nodes/CommandNode.h"
#include "Graph/MapEventGraphSchema.h"
#include "Graph/Slate/SGraphNodePlaySpeech.h"
#include "Graph/Slate/SGraphNodeShowChoices.h"
#include "Graph/Slate/SGraphNodeShowText.h"

#include "Commands/BaseCommand.h"
#include "Commands/PlaySpeech.h"
#include "Commands/ShowChoices.h"
#include "Commands/ShowText.h"

#include "SGraphNodeDefault.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(CommandNode)

#define LOCTEXT_NAMESPACE "CommandNode"

UCommandNode::UCommandNode(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)	
{
	AssignedCommandClasses = { UBaseCommand::StaticClass()};
}

void UCommandNode::SetCommandData(UBaseCommand* NewCommand)
{
	this->Command = NewCommand;
	AllocateDefaultPins();
}


TSharedPtr<SGraphNode> UCommandNode::CreateVisualWidget()
{
	if (Command == nullptr)
	{
		return SNew(SGraphNodeCommand, this);
	}
	if (Command->GetClass() == UShowText::StaticClass())
	{
		return SNew(SGraphNodeShowText, this);
	}
	else if (Command->GetClass() == UPlaySpeech::StaticClass())
	{
		return SNew(SGraphNodePlaySpeech, this);
	}
	else if (Command->GetClass() == UShowChoices::StaticClass())
	{
		return SNew(SGraphNodeShowChoices, this);
	}
	return SNew(SGraphNodeDefault);
}


FText UCommandNode::GetNodeTitle(ENodeTitleType::Type TitleType) const
{
	if (Command != nullptr)
	{
		return Command->GetLabel();
	}

	return FText::FromString("Start");
}

void UCommandNode::ReconstructNode()
{
	AllocateDefaultPins();
}

void UCommandNode::AllocateDefaultPins()
{
	Pins.Empty();
	if (IsValid(Command))
	{
		InputPins.Empty();
		for (uint32 i = 0; i < Command->GetInputPins(); i++)
		{
			CreateInputPin(i);
		}

		OutputPins.Empty();
		for (uint32 i = 0; i < Command->GetOutputPins(); i++)
		{
			CreateOutputPin(i);
		}
	}
	else
	{
		CreateOutputPin(0);
	}
}

void UCommandNode::AutowireNewNode(UEdGraphPin* FromPin)
{
	if (FromPin != nullptr)
	{
		const UMapEventGraphSchema* Schema = CastChecked<UMapEventGraphSchema>(GetSchema());

		// The pin we are creating from already has a connection that needs to be broken. We want to "insert" the new node in between, so that the output of the new node is hooked up too
		UEdGraphPin* OldLinkedPin = FromPin->LinkedTo.Num() > 0 ? FromPin->LinkedTo[0] : nullptr;

		TSet<UEdGraphNode*> NodeList;

		if (InputPins.Num() > 0)
		{
			check(InputPins[0]);
			FPinConnectionResponse Response = Schema->CanCreateConnection(FromPin, InputPins[0]);
			if (Response.Response != CONNECT_RESPONSE_DISALLOW)
			{
				if (Schema->TryCreateConnection(FromPin, InputPins[0]))
				{
					NodeList.Add(FromPin->GetOwningNode());
					NodeList.Add(this);
				}
			}

			if (Response.Response == CONNECT_RESPONSE_BREAK_OTHERS_A && OldLinkedPin != nullptr)
			{
				FromPin->BreakAllPinLinks();
				
				if (OutputPins.Num() > 0)
				{
					if (Schema->TryCreateConnection(OutputPins[0], OldLinkedPin))
					{
						NodeList.Add(this);
						NodeList.Add(OldLinkedPin->GetOwningNode());
					}
				}
			}
		}

		for (auto It = NodeList.CreateConstIterator(); It; ++It)
		{

			UEdGraphNode* Node = (*It);
			Node->NodeConnectionListChanged();
		}
	}
}

void UCommandNode::CreateInputPin(int32 Index)
{
	const FEdGraphPinType PinType = FEdGraphPinType(UEdGraphSchema_K2::PC_Exec, FName(NAME_None), nullptr, EPinContainerType::None, false, FEdGraphTerminalType());
	UEdGraphPin* NewPin = CreatePin(EGPD_Input, PinType, FName(NAME_None), Index);
	check(NewPin);

	InputPins.Emplace(NewPin);
}

void UCommandNode::CreateOutputPin(int32 Index)
{
	const FEdGraphPinType PinType = FEdGraphPinType(UEdGraphSchema_K2::PC_Exec, FName(NAME_None), nullptr, EPinContainerType::None, false, FEdGraphTerminalType());
	UEdGraphPin* NewPin = CreatePin(EGPD_Output, PinType, FName::FName("Then..."), Index);
	check(NewPin);

	OutputPins.Emplace(NewPin);
}

void UCommandNode::OnUpdateMapEvent(int32 UpdateFlags)
{

}

bool UCommandNode::CanUserDeleteNode() const
{
	return Command != nullptr;
}

bool UCommandNode::CanDuplicateNode() const
{
	return Command != nullptr;
}

FText UCommandNode::GetContextMenuName() const
{
	if (Command != nullptr)
	{
		return FText::FromName(Command->StaticClass()->GetFName());
	}
	return FText::FromString("Start");
}

void UCommandNode::PinConnectionListChanged(UEdGraphPin* Pin)
{
	Super::PinConnectionListChanged(Pin);

	if (Pin->Direction == EEdGraphPinDirection::EGPD_Input && Pin->LinkedTo.Num() > 0)
	{
		if (UEdGraphPin* LinkedPin = Pin->LinkedTo[0])
		{
			if (UEdGraphNode* Owner = LinkedPin->GetOwningNode())
			{
				if (UCommandNode* PrevCommand = CastChecked<UCommandNode>(Owner))
				{
					if (UBaseCommand* CommandData = PrevCommand->GetCommandData())
					{
						CommandData->SetNextCommand(GetCommandData());
					}
				}
			}
		}
	}
}
#undef LOCTEXT_NAMESPACE