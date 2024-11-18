#include "Commands/Start.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(Start)

UStart::UStart(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	Label = FText::FromString(TEXT("Start"));
}