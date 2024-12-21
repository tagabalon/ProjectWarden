
#pragma once

#include "EdGraph/EdGraphNode.h"
#include "CommandNode.generated.h"

class UEdGraphSchema;
class UMapEventGraph;
class UBaseCommand;

UCLASS()
class RPGSYSTEMEDITOR_API UCommandNode : public UEdGraphNode
{
	GENERATED_UCLASS_BODY()


public:
	UPROPERTY()
	TArray<TSubclassOf<UBaseCommand>> AssignedCommandClasses;

	void SetCommandData(UBaseCommand* NewCommand);
	UBaseCommand* GetCommandData() const { return Command; }
	
	void AllocateBranchPins();
	UEdGraphPin* CreateInputPin();
	UEdGraphPin* CreateOutputPin();
	int32 GetPinIndex(UEdGraphPin* Pin) const;
	void RegenerateNodeConnections(UMapEventGraph* MapEventGraph);
	void LoadPins();

	// UEdGraphNode
	virtual TSharedPtr<SGraphNode> CreateVisualWidget() override;
	virtual FText GetNodeTitle(ENodeTitleType::Type TitleType) const override;
	virtual void ReconstructNode() override;
	virtual void AllocateDefaultPins() override;
	virtual void AutowireNewNode(UEdGraphPin* FromPin) override;
	virtual bool CanUserDeleteNode() const;
	virtual bool CanDuplicateNode() const override;
	virtual void NodeConnectionListChanged() override;
	virtual FText GetPinDisplayName(const UEdGraphPin* InPin) const override;

	/** UEdGraphNode Impl. */
	virtual void PinConnectionListChanged(UEdGraphPin* Pin) override;

	// UGraphNodeDialogueBase Impl
	virtual FText GetContextMenuName() const;

	void OnUpdateMapEvent(int32 UpdateFlags);
	
	/** instance class */
	UPROPERTY()
	TSoftClassPtr<UBaseCommand> EventCommandClass;

	UEdGraphPin* EntrancePin = nullptr;
	UEdGraphPin* ExitPin = nullptr;
	TArray<UEdGraphPin*> BranchPins;

	static FName EntrancePinName;
	static FName ExitPinName;

private:
	UBaseCommand* GetConnectedCommand(UEdGraphPin* Pin) const;
	UEdGraphPin* GetNextCommandPin(UMapEventGraph* MapEventGraph) const;
	UEdGraphPin* GetBranchPinNamed(FName Name) const;

	UPROPERTY(Instanced)
	UBaseCommand* Command;
};
