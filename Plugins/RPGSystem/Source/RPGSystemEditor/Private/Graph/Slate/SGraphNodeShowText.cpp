#include "Graph/Slate/SGraphNodeShowText.h"

#include "Commands/ShowText.h"
#include "MapEvent.h"

void SGraphNodeShowText::Construct(const FArguments& InArgs, UEdGraphNode* InNode)
{
	check(InNode);
	GraphNode = InNode;
	SetCursor(EMouseCursor::Default);

	CommandNode = Cast<UCommandNode>(GraphNode);
	check(CommandNode);

	ShowTextCommand = Cast<UShowText>(CommandNode->GetCommandData());

	UpdateGraphNode();
}

TSharedRef<SWidget> SGraphNodeShowText::ShowContent()
{
	Name = SNew(SEditableTextBox)
		.ForegroundColor(FLinearColor::Black)
		.BackgroundColor(FLinearColor::White)
		.Text(ShowTextCommand->Name)
		.OnTextCommitted(this, &SGraphNodeShowText::OnNameCommitted);

	Text = SNew(SMultiLineEditableTextBox)
		.ForegroundColor(FLinearColor::Black)
		.BackgroundColor(FLinearColor::White)
		.Text(ShowTextCommand->Text)
		.OnTextCommitted(this, &SGraphNodeShowText::OnTextCommitted);

	return SNew(SBorder)
		.HAlign(HAlign_Fill)
		.VAlign(VAlign_Fill)
		.Padding(FMargin(4))
		[
			SNew(SVerticalBox)
				+ SVerticalBox::Slot()
				.AutoHeight()
				[
					ShowLabelAndValue(FText::FromString(TEXT("Face:")),
						ShowFace(),
						FText::FromString(TEXT("Set the Name to be displayed above messages.")))
				]
				+ SVerticalBox::Slot()
				.AutoHeight()
				[
					ShowLabelAndValue(FText::FromString(TEXT("Name:")),
						SNew(SBox)
						.MaxDesiredHeight(20)
						[
							Name.ToSharedRef()
						],
						FText::FromString(TEXT("Specify the face image to display in the left side of the Message Window.")))
				]
				+ SVerticalBox::Slot()
				.AutoHeight()
				[
					ShowLabelAndValue(FText::FromString(TEXT("Text:")),
						SNew(SBox)
						.HeightOverride(100)
						[
							Text.ToSharedRef()
						],
						FText::FromString(TEXT("Specify the face image to display in the left side of the Message Window.")))
				]
				/*
	.HAlign(HAlign_Fill)
	.VAlign(VAlign_Fill)
							]
						SNew(SHorizontalBox)
							.HAlign(HAlign_Fill)
							.VAlign(VAlign_Fill)
							+ SHorizontalBox::Slot()
							[
								SNew(SBorder;)
							]

						NodeTitle
					]
					+ SVerticalBox::Slot()
					.AutoHeight()
					.Padding(FMargin(4.0f))
					[
						NodeContent
					]*/
		];
		
	/*SEditableTextBox)
	.Text(FText::FromString("Hello world"));*/

	//return EditableText.ToSharedRef();
}

TSharedRef<SWidget> SGraphNodeShowText::ShowFace()
{
	return SNew(SBox)
		.WidthOverride(127)
		.HeightOverride(178)
		.HAlign(HAlign_Left)
		.Padding(FMargin(0))
		[
			SNew(SOverlay)
				+ SOverlay::Slot()
				[
					SNew(SBorder)
						.HAlign(HAlign_Fill)
						.VAlign(VAlign_Fill)
						.BorderBackgroundColor(FLinearColor::Black)
				]
		];
}

void SGraphNodeShowText::OnNameCommitted(const FText& InText, ETextCommit::Type InCommitType)
{
	if (InCommitType == ETextCommit::OnEnter)
	{
		if (UMapEvent* MapEvent = ShowTextCommand->GetTypedOuter<UMapEvent>())
		{
			MapEvent->Modify();
		}
		ShowTextCommand->Modify();
		ShowTextCommand->SetName(InText);
	}
}

void SGraphNodeShowText::OnTextCommitted(const FText& InText, ETextCommit::Type InCommitType)
{
	if (InCommitType == ETextCommit::OnUserMovedFocus)
	{
		if (UMapEvent* MapEvent = ShowTextCommand->GetTypedOuter<UMapEvent>())
		{
			MapEvent->Modify();
		}
		ShowTextCommand->Modify();
		ShowTextCommand->SetText(InText);
	}
}
