#pragma once

#include "EdGraph/EdGraph.h"

#include "MapEvent.h"
#include "MapEventGraph.generated.h"

class UMapEventGraphSchema;

UCLASS()
class RPGSYSTEMEDITOR_API UMapEventGraph : public UEdGraph
{
	GENERATED_UCLASS_BODY()

public:
	static UEdGraph* CreateGraph(UMapEvent* InMapEvent);
	static UEdGraph* CreateGraph(UMapEvent* InMapEvent, TSubclassOf<UMapEventGraphSchema> MapEventSchema);
	void Refresh();
	// UEdGraph
	virtual void NotifyGraphChanged() override;
	// --

	UMapEvent* GetMapEvent() const;

	//~ Begin UObject Interface.
	virtual void Serialize(FArchive& Ar) override;
	//~ End UObject Interface.

	virtual void CollectAllNodeInstances(TSet<UObject*>& NodeInstances);
	virtual bool CanRemoveNestedObject(UObject* TestObject) const;
	virtual void OnNodeInstanceRemoved(UObject* NodeInstance);


	virtual void OnCreated() {}
	virtual void OnLoaded();
	virtual void OnSave();

	virtual void Initialize() {}

	virtual void UpdateAsset(int32 UpdateFlags = 0);
	virtual void UpdateVersion() {}
	virtual void MarkVersion() {}

	virtual void OnSubNodeDropped() {}
	virtual void OnNodesPasted(const FString& ImportStr) {}

	bool UpdateUnknownNodeClasses() { return false; }
	void UpdateDeprecatedClasses() {}
	void RemoveOrphanedNodes() {}
	void UpdateClassData() {}

	bool IsLocked() const { return false; }
	void LockUpdates() {}
	void UnlockUpdates() {}


};