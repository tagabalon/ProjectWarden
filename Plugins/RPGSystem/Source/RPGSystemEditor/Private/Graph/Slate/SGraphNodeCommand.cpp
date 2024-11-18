#include "Graph/Slate/SGraphNodeCommand.h"

#include "MapEventEditorStyle.h"

#include "NodeFactory.h"
#include "Widgets/Text/SInlineEditableTextBlock.h"
#include "SGraphPin.h"


void SGraphNodeCommand::Construct(const FArguments& InArgs, UEdGraphNode* InNode)
{
	check(InNode);
	GraphNode = InNode;
	SetCursor(EMouseCursor::Default);

	CommandNode = Cast<UCommandNode>(GraphNode);
	check(CommandNode);

	//Subscribe to visual update events on parent node
	/*FOnUpdateNode UpdateDelegate;
	UpdateDelegate.BindSP(
		this,
		&SGraphNodeShowText::UpdateGraphNode
	);
	DialogueNode->BindOnUpdateVisuals(UpdateDelegate); */

	//Build the visuals
	UpdateGraphNode();
}

void SGraphNodeCommand::UpdateGraphNode()
{
	InputPins.Empty();
	OutputPins.Empty();

	RightNodeBox.Reset();
	LeftNodeBox.Reset();

	TSharedRef<SBorder> NodeTitle = SNew(SBorder)
		.VAlign(VAlign_Center)
		.HAlign(HAlign_Center)
		.BorderBackgroundColor(FLinearColor::White)
		//.BorderImage(FSlateColorBrush(FLinearColor::White))
		.Padding(FMargin(5, 2, 10, 2))
		[
			SNew(STextBlock)
				//.ShadowColorAndOpacity(FLinearColor::Black)
				.ColorAndOpacity(FLinearColor::Black)
				//.ShadowOffset(FIntPoint(-1, 1))
				//.Font(FSlateFontInfo("Veranda", 16))
				// localized text to be translated with a generic name HelloSlateText
				.Text(GraphNode->GetNodeTitle(ENodeTitleType::FullTitle))
		];

	TSharedRef<SBorder> NodeContent = SNew(SBorder)
		//.BorderImage(FMapEventEditorStyle::GetBrush("CommandNode.Body"))
		.HAlign(HAlign_Fill)
		.VAlign(VAlign_Fill)
		[
			SNew(SHorizontalBox)

				+ SHorizontalBox::Slot()
				.HAlign(HAlign_Left)
				.AutoWidth()
				.Padding(FMargin(-30, 0, 0, 20))
				[
					SAssignNew(LeftNodeBox, SVerticalBox)
				]
				+ SHorizontalBox::Slot()
				.AutoWidth()
				.HAlign(HAlign_Center)
				[
					ShowContent()
				]
				+ SHorizontalBox::Slot()
				.AutoWidth()
				.HAlign(HAlign_Right)
				.Padding(FMargin(20, 0, 0, -30))
				[
					SAssignNew(RightNodeBox, SVerticalBox)
				]
		];

	/*SAssignNew(InlineEditableText, SInlineEditableTextBlock)
		.Style(FAppStyle::Get(), "Graph.Node.NodeTitleInlineEditableText")
		.Text(GraphNode->GetNodeTitle(ENodeTitleType::FullTitle))*/
		//.OnVerifyTextChanged(this, &SFlowGraphNode::OnVerifyNameTextChanged)
		//.OnTextCommitted(this, &SFlowGraphNode::OnNameTextCommited)
		//.IsReadOnly(this, &SFlowGraphNode::IsNameReadOnly)
		//.IsSelected(this, &SFlowGraphNode::IsSelectedExclusively);
	//InlineEditableText->SetColorAndOpacity(TAttribute<FLinearColor>::Create(TAttribute<FLinearColor>::FGetter::CreateSP(this, &SFlowGraphNode::GetNodeTitleTextColor)));
//	]

//];

	TSharedPtr<SOverlay> MainBox;

	this->GetOrAddSlot(ENodeZone::Center)
		.HAlign(HAlign_Center)
		.VAlign(VAlign_Center)
		[
			SAssignNew(MainBox, SOverlay)
				+ SOverlay::Slot()
				[
					SNew(SImage)
						.Image(FMapEventEditorStyle::GetBrush("CommandNode.Body"))
				]
				+ SOverlay::Slot()
				[
					SNew(SVerticalBox)
						+ SVerticalBox::Slot()
						.AutoHeight()
						.Padding(FMargin(4.0f))
						[
							NodeTitle
						]
						+ SVerticalBox::Slot()
						.AutoHeight()
						.Padding(FMargin(4.0f))
						[
							NodeContent
						]
				]
		];


	//CreateBelowWidgetControls(MainBox);
	CreatePinWidgets();
	// Create Pin widgets for each of the pins.
	//for (int32 PinIndex = 0; PinIndex < GraphNode->Pins.Num(); ++PinIndex)
	//{
	//	UEdGraphPin* CurPin = GraphNode->Pins[PinIndex];

	//	if (!ensureMsgf(CurPin->GetOuter() == GraphNode
	//		, TEXT("Graph node ('%s' - %s) has an invalid %s pin: '%s'; (with a bad %s outer: '%s'); skiping creation of a widget for this pin.")
	//		, *GraphNode->GetNodeTitle(ENodeTitleType::ListView).ToString()
	//		, *GraphNode->GetPathName()
	//		, (CurPin->Direction == EEdGraphPinDirection::EGPD_Input) ? TEXT("input") : TEXT("output")
	//		, CurPin->PinFriendlyName.IsEmpty() ? *CurPin->PinName.ToString() : *CurPin->PinFriendlyName.ToString()
	//		, CurPin->GetOuter() ? *CurPin->GetOuter()->GetClass()->GetName() : TEXT("UNKNOWN")
	//		, CurPin->GetOuter() ? *CurPin->GetOuter()->GetPathName() : TEXT("NULL")))
	//	{
	//		continue;
	//	}

	//	TSharedPtr<SGraphPin> NewPin = FNodeFactory::CreatePinWidget(CurPin);
	//	check(NewPin.IsValid());

	//	SGraphPin* PinToAdd = NewPin.Get();

	//	PinToAdd->SetOwner(SharedThis(this));

	//	const UEdGraphPin* PinObj = PinToAdd->GetPinObj();
	//	const bool bAdvancedParameter = (PinObj != nullptr) && PinObj->bAdvancedView;
	//	if (bAdvancedParameter)
	//	{
	//		PinToAdd->SetVisibility(TAttribute<EVisibility>(PinToAdd, &SGraphPin::IsPinVisibleAsAdvanced));
	//	}

	//	if (PinToAdd->GetDirection() == EEdGraphPinDirection::EGPD_Input)
	//	{
	//		LeftNodeBox.Get()->AddSlot()
	//			.AutoHeight()
	//			.HAlign(HAlign_Left)
	//			.VAlign(VAlign_Center)
	//			.Padding(FMargin(4))
	//			[
	//				PinToAdd
	//			];
	//		InputPins.Add(PinToAdd);
	//	}
	//	else // Direction == EEdGraphPinDirection::EGPD_Output
	//	{
	//		RightNodeBox.Get()->AddSlot()
	//			.AutoHeight()
	//			.HAlign(HAlign_Right)
	//			.VAlign(VAlign_Center)
	//			.Padding(FMargin(4))
	//			[
	//				PinToAdd
	//			];
	//		OutputPins.Add(PinToAdd);
	//	}
	//}
}

void SGraphNodeCommand::AddPin(const TSharedRef<SGraphPin>& PinToAdd)
{
	PinToAdd->SetOwner(SharedThis(this));

	const UEdGraphPin* PinObj = PinToAdd->GetPinObj();
	const bool bAdvancedParameter = PinObj && PinObj->bAdvancedView;
	if (bAdvancedParameter)
	{
		PinToAdd->SetVisibility(TAttribute<EVisibility>(PinToAdd, &SGraphPin::IsPinVisibleAsAdvanced));
	}

	if (PinToAdd->GetDirection() == EEdGraphPinDirection::EGPD_Input)
	{
		LeftNodeBox->AddSlot()
			.AutoHeight()
			.HAlign(HAlign_Left)
			.VAlign(VAlign_Center)
			.Padding(FMargin(1))
			[
				PinToAdd
			];
		InputPins.Add(PinToAdd);
	}
	else // Direction == EEdGraphPinDirection::EGPD_Output
	{
		RightNodeBox->AddSlot()
			.AutoHeight()
			.HAlign(HAlign_Right)
			.VAlign(VAlign_Center)
			.Padding(FMargin(1))
			[
				PinToAdd
			];
		OutputPins.Add(PinToAdd);
	}
}

TSharedRef<SWidget> SGraphNodeCommand::ShowContent()
{
	return SNew(SBorder);
}

TSharedRef<SWidget> SGraphNodeCommand::ShowLabelAndValue(FText Label, const TSharedRef<SWidget>& Value, FText Tooltip)
{
	return SNew(SHorizontalBox)
		+ SHorizontalBox::Slot()
		[
			SNew(STextBlock)
				.Text(Label)
				.ToolTipText(Tooltip)
				.Justification(ETextJustify::Left)
				.ColorAndOpacity(FLinearColor::Black)
				.ShadowOffset(FVector2D::ZeroVector)
				.Visibility(EVisibility::Visible)
		]
		+ SHorizontalBox::Slot()
		[
			Value
		];
}