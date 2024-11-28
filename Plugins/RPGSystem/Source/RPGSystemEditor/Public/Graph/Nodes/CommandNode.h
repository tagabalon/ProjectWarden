
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

	void CreateInputPin(int32 Index);
	void CreateOutputPin(int32 Index);	

	// UEdGraphNode
	virtual TSharedPtr<SGraphNode> CreateVisualWidget() override;
	virtual FText GetNodeTitle(ENodeTitleType::Type TitleType) const override;
	virtual void ReconstructNode() override;
	virtual void AllocateDefaultPins() override;
	virtual void AutowireNewNode(UEdGraphPin* FromPin) override;
	virtual bool CanUserDeleteNode() const;
	virtual bool CanDuplicateNode() const override;

	// UGraphNodeDialogueBase Impl
	virtual FText GetContextMenuName() const;

	void OnUpdateMapEvent(int32 UpdateFlags);
	
	/** instance class */
	UPROPERTY()
	TSoftClassPtr<UBaseCommand> EventCommandClass;

	TArray<UEdGraphPin*> InputPins;
	TArray<UEdGraphPin*> OutputPins;


private:
	UPROPERTY(Instanced)
	UBaseCommand* Command;
};
