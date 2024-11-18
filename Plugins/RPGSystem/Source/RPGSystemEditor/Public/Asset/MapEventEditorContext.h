#pragma once

#include "CoreMinimal.h"

#include "MapEventEditorContext.generated.h"

class UMapEvent;
class FMapEventEditor;

UCLASS()
class RPGSYSTEMEDITOR_API UMapEventEditorContext : public UObject
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category="Tool Menus")
	UMapEvent* GetMapEvent() const;

	TWeakPtr<FMapEventEditor> MapEventEditor;
};
