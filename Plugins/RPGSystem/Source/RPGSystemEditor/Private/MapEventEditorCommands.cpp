#include "MapEventEditorCommands.h"

#define LOCTEXT_NAMESPACE "MapEventEditorCommands"

FMapEventGraphCommands::FMapEventGraphCommands()
	: TCommands<FMapEventGraphCommands>("MapEventGraph", LOCTEXT("MapEventGraph", "Map Event Graph"), NAME_None, FAppStyle::GetAppStyleSetName())
{
}

void FMapEventGraphCommands::RegisterCommands()
{
	UI_COMMAND(RefreshContextPins, "Refresh context pins", "Refresh pins generated from the context asset", EUserInterfaceActionType::Button, FInputChord());

	UI_COMMAND(AddInput, "Add Input", "Adds an input to the node", EUserInterfaceActionType::Button, FInputChord());
	UI_COMMAND(AddOutput, "Add Output", "Adds an output to the node", EUserInterfaceActionType::Button, FInputChord());
	UI_COMMAND(RemovePin, "Remove Pin", "Removes a pin from the node", EUserInterfaceActionType::Button, FInputChord());

	/*UI_COMMAND(AddPinBreakpoint, "Add Pin Breakpoint", "Adds a breakpoint to the pin", EUserInterfaceActionType::Button, FInputChord());
	UI_COMMAND(RemovePinBreakpoint, "Remove Pin Breakpoint", "Removes a breakpoint from the pin", EUserInterfaceActionType::Button, FInputChord());
	UI_COMMAND(EnablePinBreakpoint, "Enable Pin Breakpoint", "Enables a breakpoint on the pin", EUserInterfaceActionType::Button, FInputChord());
	UI_COMMAND(DisablePinBreakpoint, "Disable Pin Breakpoint", "Disables a breakpoint on the pin", EUserInterfaceActionType::Button, FInputChord());
	UI_COMMAND(TogglePinBreakpoint, "Toggle Pin Breakpoint", "Toggles a breakpoint on the pin", EUserInterfaceActionType::Button, FInputChord());

	UI_COMMAND(EnableNode, "Enable Node", "Default state, node is fully executed.", EUserInterfaceActionType::Button, FInputChord());
	UI_COMMAND(DisableNode, "Disable Node", "No logic executed, any Input Pin activation is ignored. Node instantly enters a deactivated state.", EUserInterfaceActionType::Button, FInputChord());
	UI_COMMAND(SetPassThrough, "Set Pass Through", "Internal node logic not executed. All connected outputs are triggered, node finishes its work.", EUserInterfaceActionType::Button, FInputChord());
	UI_COMMAND(ForcePinActivation, "Force Pin Activation", "Forces execution of the pin in a graph, used to bypass blockers", EUserInterfaceActionType::Button, FInputChord());

	UI_COMMAND(FocusViewport, "Focus Viewport", "Focus viewport on actor assigned to the node", EUserInterfaceActionType::Button, FInputChord());
	UI_COMMAND(JumpToNodeDefinition, "Jump to Node Definition", "Jump to the node definition", EUserInterfaceActionType::Button, FInputChord());*/
}

#undef LOCTEXT_NAMESPACE