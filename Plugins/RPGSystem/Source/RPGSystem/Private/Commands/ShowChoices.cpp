#include "Commands/ShowChoices.h"

#include "Actors/MapEventActor.h"
#include "Interface/MessageInterface.h"

#include "GameFramework/HUD.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(ShowChoices)

void UShowChoices::Execute(APlayerController* Player, AMapEventActor* MapEventActor)
{
	if (AHUD* HUD = Player->GetHUD())
	{
		if (HUD->GetClass()->ImplementsInterface(UMessageInterface::StaticClass()))
		{
			IMessageInterface::Execute_ShowChoices(HUD, this);
		}
	}
}