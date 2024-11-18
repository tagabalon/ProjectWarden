#pragma once

#include "EdGraph/EdGraphSchema.h"
#include "Framework/Commands/Commands.h"
#include "Framework/Commands/UICommandInfo.h"
#include "Templates/SharedPointer.h"

class RPGSYSTEMEDITOR_API FMapEventGraphCommands : public TCommands<FMapEventGraphCommands>
{
public:
	FMapEventGraphCommands();

	/** Context Pins */
	TSharedPtr<FUICommandInfo> RefreshContextPins;

	/** Pins */
	TSharedPtr<FUICommandInfo> AddInput;
	TSharedPtr<FUICommandInfo> AddOutput;
	TSharedPtr<FUICommandInfo> RemovePin;

	///** Breakpoints */
	//TSharedPtr<FUICommandInfo> AddPinBreakpoint;
	//TSharedPtr<FUICommandInfo> RemovePinBreakpoint;
	//TSharedPtr<FUICommandInfo> EnablePinBreakpoint;
	//TSharedPtr<FUICommandInfo> DisablePinBreakpoint;
	//TSharedPtr<FUICommandInfo> TogglePinBreakpoint;

	///** Execution Override */
	//TSharedPtr<FUICommandInfo> EnableNode;
	//TSharedPtr<FUICommandInfo> DisableNode;
	//TSharedPtr<FUICommandInfo> SetPassThrough;
	//TSharedPtr<FUICommandInfo> ForcePinActivation;

	///** Jumps */
	//TSharedPtr<FUICommandInfo> FocusViewport;
	//TSharedPtr<FUICommandInfo> JumpToNodeDefinition;

	virtual void RegisterCommands() override;
};