#pragma once

#include "Commands/BaseCommand.h"

#include "Start.generated.h"

/*
*/
UCLASS(NotBlueprintable, NotPlaceable, meta = (DisplayName = "Start"))
class RPGSYSTEM_API UStart : public UBaseCommand
{
	GENERATED_UCLASS_BODY()

	friend class UMapEvent;

public:
	virtual FText GetLabel() const
	{
		return Label;
	}

	virtual uint32 GetInputPins() const
	{
		return 0;
	}

private:
	FText Label;

};