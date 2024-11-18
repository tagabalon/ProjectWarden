// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Commands/IEventCommand.h"
#include "Commands/BaseCommand.h"
#include "Interface/MessageInterface.h"
#include "RPGEngine.h"

#include "MapEvent.generated.h"

class APlayerController;
class UEdGraph;
class UEdGraphNode;
class UMapEvent;

#if WITH_EDITOR

/** Interface for calling the graph editor methods */
class RPGSYSTEM_API IMapEventGraphInterface
{
public:
	IMapEventGraphInterface() {}
	virtual ~IMapEventGraphInterface() {}

	virtual void OnInputTriggered(UEdGraphNode* GraphNode, const int32 Index) const {}
	virtual void OnOutputTriggered(UEdGraphNode* GraphNode, const int32 Index) const {}
};

DECLARE_DELEGATE(FMapEventGraphEvent);

#endif

UCLASS(BlueprintType, hideCategories = Object)
class RPGSYSTEM_API UMapEvent : public UObject
{
	GENERATED_UCLASS_BODY()

public:
	UPROPERTY(/*VisibleAnywhere, BlueprintReadOnly, Category = "Map Event"*/)
	FGuid AssetGuid;

	const TArray<UBaseCommand*>& GetCommands() const { return Commands; }

	//IEventCommandInterface* GetRootCommand() const { return RootCommand; }
	void Execute(APlayerController* Player);


#if WITH_EDITOR
	friend class UMapEventGraph;

	UEdGraph* GetGraph() const { return Graph; }
	void SetGraph(UEdGraph* InGraph) { Graph = InGraph; }
	UBaseCommand* CreateCommand(const UClass* EventCommandClass, UEdGraphNode* GraphNode);
	void RegisterCommand(const FGuid& NewGuid, UBaseCommand* NewCommand);
#endif

private:
	UPROPERTY(VisibleAnywhere)
	TArray<UBaseCommand*> Commands;
	
	/*UPROPERTY(VisibleAnywhere)
	UBaseCommand* RootCommand = nullptr;*/

#if WITH_EDITORONLY_DATA

	UPROPERTY()
	UEdGraph* Graph = nullptr;

#endif
};
