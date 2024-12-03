#pragma once

#include "BaseCommand.h"
#include "DialogueChoice.generated.h"

USTRUCT(BlueprintType)
struct RPGSYSTEM_API FDialogueChoice
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTexture* Icon;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText Text;
};