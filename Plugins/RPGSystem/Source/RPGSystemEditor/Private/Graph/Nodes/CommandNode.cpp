#include "Graph/Nodes/CommandNode.h"
#include "Graph/MapEventGraphSchema.h"
#include "Graph/Slate/SGraphNodeShowText.h"

#include "Commands/BaseCommand.h"
#include "Commands/Start.h"
#include "Commands/ShowText.h"

#include "SGraphNodeDefault.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(CommandNode)

#define LOCTEXT_NAMESPACE "CommandNode"

UCommandNode::UCommandNode(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)	
{
	AssignedCommandClasses = { UBaseCommand::StaticClass(), UStart::StaticClass() };
}

void UCommandNode::SetCommandData(UBaseCommand* NewCommand)
{
	this->Command = NewCommand;
}


TSharedPtr<SGraphNode> UCommandNode::CreateVisualWidget()
{
	if (Command->GetClass() == UShowText::StaticClass())
	{
		return SNew(SGraphNodeShowText, this);
	}
	return SNew(SGraphNodeCommand, this);
}


FText UCommandNode::GetNodeTitle(ENodeTitleType::Type TitleType) const
{
	if (Command != nullptr)
	{
		return Command->GetLabel();
	}

	return Super::GetNodeTitle(TitleType);
}

void UCommandNode::ReconstructNode()
{
	AllocateDefaultPins();
}

void UCommandNode::AllocateDefaultPins()
{
	if (IsValid(Command))
	{
		for (uint32 i = 0; i < Command->GetInputPins(); i++)
		{
			CreateInputPin(i);
		}

		for (uint32 i = 0; i < Command->GetOutputPins(); i++)
		{
			CreateOutputPin(i);
		}
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