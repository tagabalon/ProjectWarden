#pragma once

#include "CoreMinimal.h"
#include "BaseCommand.h"
#include "ShowMessage.generated.h"

/*
*/
UCLASS(Blueprintable)
class RPGSYSTEM_API UShowMessage : public UBaseCommand
{
	GENERATED_BODY()

	virtual FText GetCategory() const
	{
		return FText::FromString("Message");
	}

	virtual FText GetLabel() const {
		return FText::FromString(TEXT("Unknown Command"));
	}
};