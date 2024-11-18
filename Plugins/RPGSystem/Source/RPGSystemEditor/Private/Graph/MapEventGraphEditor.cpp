#pragma once

#include "Graph/MapEventGraphEditor.h"

#include "Graph/Nodes/CommandNode.h"

#include "Asset/MapEventEditor.h"
#include "MapEventEditorCommands.h"

#define  LOCTEXT_NAMESPACE	"MapEventGraphEditor"

void SMapEventGraphEditor::Construct(const FArguments& InArgs, const TSharedPtr<FMapEventEditor> InEditor)
{
	MapEventEditor = InEditor;
	MapEvent = MapEventEditor.Pin()->GetMapEvent();

	DetailsView = InArgs._DetailsView;

	SGraphEditor::FArguments Arguments;
	Arguments._Appearance = GetGraphAppearanceInfo();
	Arguments._GraphToEdit = MapEvent->GetGraph();
	//Arguments._GraphEvents = InArgs._GraphEvents;
	Arguments._AutoExpandActionMenu = true;
	Arguments._GraphEvents.OnSelectionChanged = FOnSelectionChanged::CreateSP(this, &SMapEventGraphEditor::OnSelectionChanged);
	/*Arguments._GraphEvents.OnNodeDoubleClicked = FSingleNodeEvent::CreateSP(this, &SMapEventGraphEditor::OnNodeDoubleClicked);
	Arguments._GraphEvents.OnTextCommitted = FOnNodeTextCommitted::CreateSP(this, &SMapEventGraphEditor::OnNodeTitleCommitted);
	Arguments._GraphEvents.OnSpawnNodeByShortcut = FOnSpawnNodeByShortcut::CreateStatic(&SMapEventGraphEditor::OnSpawnGraphNodeByShortcut, static_cast<UEdGraph*>(FlowAsset->GetGraph()));*/

	SGraphEditor::Construct(Arguments);
}

void SMapEventGraphEditor::OnSelectionChanged(const TSet<UObject*>& Nodes)
{
	TArray<UObject*> SelectedObjects;

	if (Nodes.Num() > 0)
	{
		MapEventEditor.Pin()->SetUISelectionState(FMapEventEditor::GraphTab);

		for (TSet<UObject*>::TConstIterator SetIt(Nodes); SetIt; ++SetIt)
		{
			if (const UCommandNode* CommandNode = Cast<UCommandNode>(*SetIt))
			{
				SelectedObjects.Add(Cast<UObject>(CommandNode->GetCommandData()));
			}
			else
			{
				SelectedObjects.Add(*SetIt);
			}
		}
	}
	else
	{
		MapEventEditor.Pin()->SetUISelectionState(NAME_None);
		SelectedObjects.Add(MapEvent.Get());
	}

	if (DetailsView.IsValid())
	{
		DetailsView->SetObjects(SelectedObjects);
	}

	OnSelectionChangedEvent.ExecuteIfBound(Nodes);
}

//void SMapEventGraphEditor::BindGraphCommands()
//{
//	FGraphEditorCommands::Register();
//	FMapEventGraphCommands::Register();
//	//FFlowSpawnNodeCommands::Register();
//
//	const FGenericCommands& GenericCommands = FGenericCommands::Get();
//	const FGraphEditorCommandsImpl& GraphEditorCommands = FGraphEditorCommands::Get();
//	const FMapEventGraphCommands& FlowGraphCommands = FMapEventGraphCommands::Get();
//
//	CommandList = MakeShareable(new FUICommandList);
//
//	// Graph commands
//	//CommandList->MapAction(GraphEditorCommands.CreateComment,
//	//	FExecuteAction::CreateSP(this, &SFlowGraphEditor::OnCreateComment),
//	//	FCanExecuteAction::CreateStatic(&SFlowGraphEditor::CanEdit));
//
//	//CommandList->MapAction(GraphEditorCommands.StraightenConnections,
//	//	FExecuteAction::CreateSP(this, &SFlowGraphEditor::OnStraightenConnections));
//
//	//CommandList->MapAction(GraphEditorCommands.DeleteAndReconnectNodes,
//	//	FExecuteAction::CreateSP(this, &SFlowGraphEditor::DeleteSelectedNodes),
//	//	FCanExecuteAction::CreateSP(this, &SFlowGraphEditor::CanDeleteNodes));
//
//	//// Generic Node commands
//	//CommandList->MapAction(GenericCommands.Undo,
//	//	FExecuteAction::CreateStatic(&SFlowGraphEditor::UndoGraphAction),
//	//	FCanExecuteAction::CreateStatic(&SFlowGraphEditor::CanEdit));
//
//	//CommandList->MapAction(GenericCommands.Redo,
//	//	FExecuteAction::CreateStatic(&SFlowGraphEditor::RedoGraphAction),
//	//	FCanExecuteAction::CreateStatic(&SFlowGraphEditor::CanEdit));
//
//	//CommandList->MapAction(GenericCommands.SelectAll,
//	//	FExecuteAction::CreateSP(this, &SFlowGraphEditor::SelectAllNodes),
//	//	FCanExecuteAction::CreateSP(this, &SFlowGraphEditor::CanSelectAllNodes));
//
//	//CommandList->MapAction(GenericCommands.Delete,
//	//	FExecuteAction::CreateSP(this, &SFlowGraphEditor::DeleteSelectedNodes),
//	//	FCanExecuteAction::CreateSP(this, &SFlowGraphEditor::CanDeleteNodes));
//
//	//CommandList->MapAction(GenericCommands.Copy,
//	//	FExecuteAction::CreateSP(this, &SFlowGraphEditor::CopySelectedNodes),
//	//	FCanExecuteAction::CreateSP(this, &SFlowGraphEditor::CanCopyNodes));
//
//	//CommandList->MapAction(GenericCommands.Cut,
//	//	FExecuteAction::CreateSP(this, &SFlowGraphEditor::CutSelectedNodes),
//	//	FCanExecuteAction::CreateSP(this, &SFlowGraphEditor::CanCutNodes));
//
//	//CommandList->MapAction(GenericCommands.Paste,
//	//	FExecuteAction::CreateSP(this, &SFlowGraphEditor::PasteNodes),
//	//	FCanExecuteAction::CreateSP(this, &SFlowGraphEditor::CanPasteNodes));
//
//	//CommandList->MapAction(GenericCommands.Duplicate,
//	//	FExecuteAction::CreateSP(this, &SFlowGraphEditor::DuplicateNodes),
//	//	FCanExecuteAction::CreateSP(this, &SFlowGraphEditor::CanDuplicateNodes));
//
//	//// Pin commands
//	//CommandList->MapAction(FlowGraphCommands.RefreshContextPins,
//	//	FExecuteAction::CreateSP(this, &SFlowGraphEditor::RefreshContextPins),
//	//	FCanExecuteAction::CreateSP(this, &SFlowGraphEditor::CanRefreshContextPins));
//
//	//CommandList->MapAction(FlowGraphCommands.AddInput,
//	//	FExecuteAction::CreateSP(this, &SFlowGraphEditor::AddInput),
//	//	FCanExecuteAction::CreateSP(this, &SFlowGraphEditor::CanAddInput));
//
//	//CommandList->MapAction(FlowGraphCommands.AddOutput,
//	//	FExecuteAction::CreateSP(this, &SFlowGraphEditor::AddOutput),
//	//	FCanExecuteAction::CreateSP(this, &SFlowGraphEditor::CanAddOutput));
//
//	//CommandList->MapAction(FlowGraphCommands.RemovePin,
//	//	FExecuteAction::CreateSP(this, &SFlowGraphEditor::RemovePin),
//	//	FCanExecuteAction::CreateSP(this, &SFlowGraphEditor::CanRemovePin));
//
//	//// Breakpoint commands
//	//CommandList->MapAction(GraphEditorCommands.AddBreakpoint,
//	//	FExecuteAction::CreateSP(this, &SFlowGraphEditor::OnAddBreakpoint),
//	//	FCanExecuteAction::CreateSP(this, &SFlowGraphEditor::CanAddBreakpoint),
//	//	FIsActionChecked(),
//	//	FIsActionButtonVisible::CreateSP(this, &SFlowGraphEditor::CanAddBreakpoint)
//	//);
//
//	//CommandList->MapAction(GraphEditorCommands.RemoveBreakpoint,
//	//	FExecuteAction::CreateSP(this, &SFlowGraphEditor::OnRemoveBreakpoint),
//	//	FCanExecuteAction::CreateSP(this, &SFlowGraphEditor::CanRemoveBreakpoint),
//	//	FIsActionChecked(),
//	//	FIsActionButtonVisible::CreateSP(this, &SFlowGraphEditor::CanRemoveBreakpoint)
//	//);
//
//	//CommandList->MapAction(GraphEditorCommands.EnableBreakpoint,
//	//	FExecuteAction::CreateSP(this, &SFlowGraphEditor::OnEnableBreakpoint),
//	//	FCanExecuteAction::CreateSP(this, &SFlowGraphEditor::CanEnableBreakpoint),
//	//	FIsActionChecked(),
//	//	FIsActionButtonVisible::CreateSP(this, &SFlowGraphEditor::CanEnableBreakpoint)
//	//);
//
//	//CommandList->MapAction(GraphEditorCommands.DisableBreakpoint,
//	//	FExecuteAction::CreateSP(this, &SFlowGraphEditor::OnDisableBreakpoint),
//	//	FCanExecuteAction::CreateSP(this, &SFlowGraphEditor::CanDisableBreakpoint),
//	//	FIsActionChecked(),
//	//	FIsActionButtonVisible::CreateSP(this, &SFlowGraphEditor::CanDisableBreakpoint)
//	//);
//
//	//CommandList->MapAction(GraphEditorCommands.ToggleBreakpoint,
//	//	FExecuteAction::CreateSP(this, &SFlowGraphEditor::OnToggleBreakpoint),
//	//	FCanExecuteAction::CreateSP(this, &SFlowGraphEditor::CanToggleBreakpoint),
//	//	FIsActionChecked(),
//	//	FIsActionButtonVisible::CreateSP(this, &SFlowGraphEditor::CanToggleBreakpoint)
//	//);
//
//	//// Pin Breakpoint commands
//	//CommandList->MapAction(FlowGraphCommands.AddPinBreakpoint,
//	//	FExecuteAction::CreateSP(this, &SFlowGraphEditor::OnAddPinBreakpoint),
//	//	FCanExecuteAction::CreateSP(this, &SFlowGraphEditor::CanAddPinBreakpoint),
//	//	FIsActionChecked(),
//	//	FIsActionButtonVisible::CreateSP(this, &SFlowGraphEditor::CanAddPinBreakpoint)
//	//);
//
//	//CommandList->MapAction(FlowGraphCommands.RemovePinBreakpoint,
//	//	FExecuteAction::CreateSP(this, &SFlowGraphEditor::OnRemovePinBreakpoint),
//	//	FCanExecuteAction::CreateSP(this, &SFlowGraphEditor::CanRemovePinBreakpoint),
//	//	FIsActionChecked(),
//	//	FIsActionButtonVisible::CreateSP(this, &SFlowGraphEditor::CanRemovePinBreakpoint)
//	//);
//
//	//CommandList->MapAction(FlowGraphCommands.EnablePinBreakpoint,
//	//	FExecuteAction::CreateSP(this, &SFlowGraphEditor::OnEnablePinBreakpoint),
//	//	FCanExecuteAction::CreateSP(this, &SFlowGraphEditor::CanEnablePinBreakpoint),
//	//	FIsActionChecked(),
//	//	FIsActionButtonVisible::CreateSP(this, &SFlowGraphEditor::CanEnablePinBreakpoint)
//	//);
//
//	//CommandList->MapAction(FlowGraphCommands.DisablePinBreakpoint,
//	//	FExecuteAction::CreateSP(this, &SFlowGraphEditor::OnDisablePinBreakpoint),
//	//	FCanExecuteAction::CreateSP(this, &SFlowGraphEditor::CanDisablePinBreakpoint),
//	//	FIsActionChecked(),
//	//	FIsActionButtonVisible::CreateSP(this, &SFlowGraphEditor::CanDisablePinBreakpoint)
//	//);
//
//	//CommandList->MapAction(FlowGraphCommands.TogglePinBreakpoint,
//	//	FExecuteAction::CreateSP(this, &SFlowGraphEditor::OnTogglePinBreakpoint),
//	//	FCanExecuteAction::CreateSP(this, &SFlowGraphEditor::CanTogglePinBreakpoint),
//	//	FIsActionChecked(),
//	//	FIsActionButtonVisible::CreateSP(this, &SFlowGraphEditor::CanTogglePinBreakpoint)
//	//);
//
//	//// Execution Override commands
//	//CommandList->MapAction(FlowGraphCommands.EnableNode,
//	//	FExecuteAction::CreateSP(this, &SFlowGraphEditor::SetSignalMode, EFlowSignalMode::Enabled),
//	//	FCanExecuteAction::CreateSP(this, &SFlowGraphEditor::CanSetSignalMode, EFlowSignalMode::Enabled),
//	//	FIsActionChecked(),
//	//	FIsActionButtonVisible::CreateSP(this, &SFlowGraphEditor::CanSetSignalMode, EFlowSignalMode::Enabled)
//	//);
//
//	//CommandList->MapAction(FlowGraphCommands.DisableNode,
//	//	FExecuteAction::CreateSP(this, &SFlowGraphEditor::SetSignalMode, EFlowSignalMode::Disabled),
//	//	FCanExecuteAction::CreateSP(this, &SFlowGraphEditor::CanSetSignalMode, EFlowSignalMode::Disabled),
//	//	FIsActionChecked(),
//	//	FIsActionButtonVisible::CreateSP(this, &SFlowGraphEditor::CanSetSignalMode, EFlowSignalMode::Disabled)
//	//);
//
//	//CommandList->MapAction(FlowGraphCommands.SetPassThrough,
//	//	FExecuteAction::CreateSP(this, &SFlowGraphEditor::SetSignalMode, EFlowSignalMode::PassThrough),
//	//	FCanExecuteAction::CreateSP(this, &SFlowGraphEditor::CanSetSignalMode, EFlowSignalMode::PassThrough),
//	//	FIsActionChecked(),
//	//	FIsActionButtonVisible::CreateSP(this, &SFlowGraphEditor::CanSetSignalMode, EFlowSignalMode::PassThrough)
//	//);
//
//	//CommandList->MapAction(FlowGraphCommands.ForcePinActivation,
//	//	FExecuteAction::CreateSP(this, &SFlowGraphEditor::OnForcePinActivation),
//	//	FCanExecuteAction::CreateStatic(&SFlowGraphEditor::IsPIE),
//	//	FIsActionChecked(),
//	//	FIsActionButtonVisible::CreateStatic(&SFlowGraphEditor::IsPIE)
//	//);
//
//	//// Jump commands
//	//CommandList->MapAction(FlowGraphCommands.FocusViewport,
//	//	FExecuteAction::CreateSP(this, &SFlowGraphEditor::FocusViewport),
//	//	FCanExecuteAction::CreateSP(this, &SFlowGraphEditor::CanFocusViewport));
//
//	//CommandList->MapAction(FlowGraphCommands.JumpToNodeDefinition,
//	//	FExecuteAction::CreateSP(this, &SFlowGraphEditor::JumpToNodeDefinition),
//	//	FCanExecuteAction::CreateSP(this, &SFlowGraphEditor::CanJumpToNodeDefinition));
//
//	//// Organisation Commands
//	//CommandList->MapAction(GraphEditorCommands.AlignNodesTop,
//	//	FExecuteAction::CreateSP(this, &SFlowGraphEditor::OnAlignTop));
//
//	//CommandList->MapAction(GraphEditorCommands.AlignNodesMiddle,
//	//	FExecuteAction::CreateSP(this, &SFlowGraphEditor::OnAlignMiddle));
//
//	//CommandList->MapAction(GraphEditorCommands.AlignNodesBottom,
//	//	FExecuteAction::CreateSP(this, &SFlowGraphEditor::OnAlignBottom));
//
//	//CommandList->MapAction(GraphEditorCommands.AlignNodesLeft,
//	//	FExecuteAction::CreateSP(this, &SFlowGraphEditor::OnAlignLeft));
//
//	//CommandList->MapAction(GraphEditorCommands.AlignNodesCenter,
//	//	FExecuteAction::CreateSP(this, &SFlowGraphEditor::OnAlignCenter));
//
//	//CommandList->MapAction(GraphEditorCommands.AlignNodesRight,
//	//	FExecuteAction::CreateSP(this, &SFlowGraphEditor::OnAlignRight));
//
//	//CommandList->MapAction(GraphEditorCommands.StraightenConnections,
//	//	FExecuteAction::CreateSP(this, &SFlowGraphEditor::OnStraightenConnections));
//}

FGraphAppearanceInfo SMapEventGraphEditor::GetGraphAppearanceInfo() const
{
	FGraphAppearanceInfo AppearanceInfo;
	AppearanceInfo.CornerText = GetCornerText();

	/*if (UFlowDebuggerSubsystem::IsPlaySessionPaused())
	{
		AppearanceInfo.PIENotifyText = LOCTEXT("PausedLabel", "PAUSED");
	}*/

	return AppearanceInfo;
}

FText SMapEventGraphEditor::GetCornerText() const
{
	return LOCTEXT("AppearanceCornerText_MapEvent", "MAP EVENT");
}

#undef LOCTEXT_NAMESPACE