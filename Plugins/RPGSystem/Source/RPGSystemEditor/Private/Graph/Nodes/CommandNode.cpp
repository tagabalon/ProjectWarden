#include "Graph/Nodes/CommandNode.h"

#include "Commands/BaseCommand.h"
#include "Commands/PlaySpeech.h"
#include "Commands/ShowChoices.h"
#include "Commands/ShowText.h"
#include "Graph/MapEventGraph.h"
#include "Graph/MapEventGraphSchema.h"
#include "Graph/Slate/SGraphNodePlaySpeech.h"
#include "Graph/Slate/SGraphNodeShowChoices.h"
#include "Graph/Slate/SGraphNodeShowText.h"

#include "SGraphNodeDefault.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(CommandNode)

#define LOCTEXT_NAMESPACE "CommandNode"

FName UCommandNode::EntrancePinName = FName::FName("Entrance");
FName UCommandNode::ExitPinName = FName::FName("Exit");

UCommandNode::UCommandNode(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)	
{
	AssignedCommandClasses = { UBaseCommand::StaticClass()};
}

void UCommandNode::SetCommandData(UBaseCommand* NewCommand)
{
	Command = NewCommand;

	ReconstructNode();	
}

void UCommandNode::AllocateBranchPins()
{
	if (Command != nullptr && Command->IsA<UBranchCommand>())
	{
		UBranchCommand* BranchCommand = CastChecked<UBranchCommand>(Command);
		BranchCommand->Modify(false);

		Modify(false);
		BranchPins.Empty();

		uint32 Index = 0;
		for(const FBranch& Branch : BranchCommand->GetBranches())
		{
			UEdGraphPin* BranchPin = CreateOutputPin();
			BranchPins.AddUnique(BranchPin);

			BranchCommand->SetBranchPin(Index, BranchPin);
			Index++;
		}
	}
}

void UCommandNode::RegenerateNodeConnections(UMapEventGraph* MapEventGraph)
{
	const UMapEventGraphSchema* Schema = CastChecked<UMapEventGraphSchema>(GetSchema());
	if (UEdGraphPin* NextCommandPin = GetNextCommandPin(MapEventGraph))
	{
		if (ExitPin->LinkedTo.Num() > 0 && ExitPin->LinkedTo[0] == nullptr)
		{
			ExitPin->LinkedTo.Empty();
		}
		Schema->TryCreateConnection(ExitPin, NextCommandPin);
	}
	if (Command != nullptr && Command->IsA<UBranchCommand>())
	{
		UBranchCommand* BranchCommand = Cast<UBranchCommand>(Command);
		int32 Index = 0;
		for (UEdGraphPin* BranchPin : BranchPins)
		{
			UBaseCommand* BranchConnection = BranchCommand->GetBranchCommand(Index);
			if (UCommandNode* BranchNode = MapEventGraph->FindNodeOf(BranchConnection))
			{
				Schema->TryCreateConnection(BranchPin, BranchNode->EntrancePin);
			}
			Index++;
		}
	}
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
	Pins.Empty();
	BranchPins.Empty();

	AllocateDefaultPins();
	AllocateBranchPins();
}


FText UCommandNode::GetPinDisplayName(const UEdGraphPin* InPin) const
{
	if (InPin->Direction == EEdGraphPinDirection::EGPD_Input)
	{
		return FText::FromString(TEXT(""));
	}

	if (Command == nullptr)
	{
		return FText::FromString(TEXT("Do..."));
	}
	if (InPin == ExitPin)
	{
		return  FText::FromString(TEXT("Then..."));
	}

	if (Command != nullptr && Command->IsA<UBranchCommand>())
	{
		UBranchCommand* BranchCommand = Cast<UBranchCommand>(Command);

		int32 Index = BranchPins.IndexOfByPredicate([&](const UEdGraphPin* Pin) { return InPin == Pin; });
		if (Index >= 0 && Index < BranchCommand->GetBranchSize())
		{
			const FBranch& Branch = BranchCommand->GetBranches()[Index];
			if (UBranchParameter* BranchParam = Branch.BranchParam)
			{
				return BranchParam->GetDisplayName();
			}
		}
	}

	return Super::GetPinDisplayName(InPin);
}

void UCommandNode::AllocateDefaultPins()
{
	Modify(false);
	if (IsValid(Command))
	{
		EntrancePin = CreateInputPin();
		EntrancePin->PinName = EntrancePinName;
	}

	ExitPin = CreateOutputPin();
	ExitPin->PinName = ExitPinName;
}

int32 UCommandNode::GetPinIndex(UEdGraphPin* Pin) const
{
	int32 Index = 0;
	for (UEdGraphPin* BranchPin : BranchPins)
	{
		if (BranchPin == Pin)
		{
			return Index;
		}
		Index++;
	}
	return -1;
}

void UCommandNode::AutowireNewNode(UEdGraphPin* FromPin)
{
	if (FromPin != nullptr)
	{
		const UMapEventGraphSchema* Schema = CastChecked<UMapEventGraphSchema>(GetSchema());

		// The pin we are creating from already has a connection that needs to be broken. We want to "insert" the new node in between, so that the output of the new node is hooked up too
		UEdGraphPin* OldLinkedPin = FromPin->LinkedTo.Num() > 0 ? FromPin->LinkedTo[0] : nullptr;

		TSet<UEdGraphNode*> NodeList;

		if (EntrancePin != nullptr)
		{
			FPinConnectionResponse Response = Schema->CanCreateConnection(FromPin, EntrancePin);
			if (Response.Response != CONNECT_RESPONSE_DISALLOW)
			{
				if (Schema->TryCreateConnection(FromPin, EntrancePin))
				{
					NodeList.Add(FromPin->GetOwningNode());
					NodeList.Add(this);
				}
			}

			if (Response.Response == CONNECT_RESPONSE_BREAK_OTHERS_A && OldLinkedPin != nullptr)
			{
				FromPin->BreakAllPinLinks();
				
				if (ExitPin != nullptr)
				{
					if (Schema->TryCreateConnection(ExitPin, OldLinkedPin))
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

void UCommandNode::LoadPins()
{
	BranchPins.Empty();
	for (UEdGraphPin* Pin : Pins)
	{
		if (Pin->Direction == EEdGraphPinDirection::EGPD_Input)
		{
			EntrancePin = Pin;
		}
		else if (Pin->Direction == EEdGraphPinDirection::EGPD_Output)
		{
			if (Pin->PinName == ExitPinName)
			{
				ExitPin = Pin;
			}
			else
			{
				BranchPins.Add(Pin);
			}
		}
	}
}

UEdGraphPin* UCommandNode::CreateInputPin()
{
	const FEdGraphPinType PinType = FEdGraphPinType(UEdGraphSchema_K2::PC_Exec, FName(NAME_None), nullptr, EPinContainerType::None, false, FEdGraphTerminalType());
	return CreatePin(EGPD_Input, PinType, FName(NAME_None), INDEX_NONE);
}

UEdGraphPin* UCommandNode::CreateOutputPin()
{
	const FEdGraphPinType PinType = FEdGraphPinType(UEdGraphSchema_K2::PC_Exec, FName(NAME_None), nullptr, EPinContainerType::None, false, FEdGraphTerminalType());
	return CreatePin(EGPD_Output, PinType, FName(NAME_None), INDEX_NONE);
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

void UCommandNode::NodeConnectionListChanged()
{
	if (Command != nullptr)
	{
		Command->Modify();
		if (ExitPin != nullptr)
		{
			Command->SetNextCommand(GetConnectedCommand(ExitPin));
		}

		if (Command->IsA<UBranchCommand>())
		{
			UBranchCommand* BranchCommand = Cast<UBranchCommand>(Command);
			int32 Index = 0;
			for (UEdGraphPin* BranchPin : BranchPins)
			{
				UBaseCommand* Connected = GetConnectedCommand(BranchPin);
				BranchCommand->SetBranchCommand(Connected, Index);
				Index++;
			}
		}
	}
	else
	{
		if (ExitPin != nullptr)
		{
			if (UObject* Outer = GetOuter())
			{
				if (Outer->IsA<UMapEventGraph>())
				{
					UMapEventGraph* Graph = Cast<UMapEventGraph>(Outer);
					if (UEdGraphPin* OtherPin = GetNextCommandPin(Graph))
					{
						Graph->SetStartNode(Cast<UCommandNode>(OtherPin->GetOwningNode()));
					}
				}
			}
		}
	}
}

UBaseCommand* UCommandNode::GetConnectedCommand(UEdGraphPin* Pin) const
{
	if (Pin == nullptr || Pin->Direction != EEdGraphPinDirection::EGPD_Output || !Pin->HasAnyConnections())
	{
		return nullptr;
	}

	if (UEdGraphPin* Connection = Pin->LinkedTo[0])
	{
		if (UCommandNode* NextNode = Cast<UCommandNode>(Connection->GetOwningNode()))
		{
			return NextNode->GetCommandData();
		}
	}

	return nullptr;
}

UEdGraphPin* UCommandNode::GetNextCommandPin(UMapEventGraph* MapEventGraph) const
{
	if (Command != nullptr)
	{
		if (IEventCommandInterface* Next = Command->GetNextCommand())
		{
			if (UBaseCommand* NextCommand = Cast<UBaseCommand>(Next))
			{
				if (UCommandNode* NextCommandNode = MapEventGraph->FindNodeOf(NextCommand))
				{
					return NextCommandNode->EntrancePin;
				}
			}
		}
	}
	else
	{
		if (UCommandNode* RootNode = MapEventGraph->GetRootCommandNode())
		{
			return RootNode->EntrancePin;
		}
	}
	return nullptr;
}

UEdGraphPin* UCommandNode::GetBranchPinNamed(FName Name) const
{
	for (UEdGraphPin* Pin : BranchPins)
	{
		if (Pin->GetName() == Name)
		{
			return Pin;
		}
	}
	return nullptr;
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