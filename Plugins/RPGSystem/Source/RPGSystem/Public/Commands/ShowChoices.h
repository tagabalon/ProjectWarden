#pragma once

#include "BaseCommand.h"
#include "ShowChoices.generated.h"

class AMapEventActor;

UCLASS(Blueprintable, Category="RPG System")
class RPGSYSTEM_API UShowChoices : public UBaseCommand
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Message")
	TArray<FText> Choices;

private:

	//BaseCommand
	virtual FText GetCategory() const
	{
		return FText::FromString(TEXT("Message"));
	}

	virtual FText GetLabel() const {
		return FText::FromString(TEXT("Show Choices"));
	}
};