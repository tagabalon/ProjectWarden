#include "Commands/ShowText.h"

#include "Interface/MessageInterface.h"
#include "RPGSettings.h"
#include "MapEvent.h"

#include "Blueprint/WidgetTree.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/HUD.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(ShowText)

//UShowText::UShowText(const FObjectInitializer& ObjectInitializer)
//	: Super(ObjectInitializer)
//{
//	MessagesWidget = URPGSettings::Get()->GetMessagesWidget();
//}

void UShowText::Execute(APlayerController* Player)
{
	if (AHUD* HUD = Player->GetHUD())
	{
		if (HUD->GetClass()->ImplementsInterface(UMessageInterface::StaticClass()))
		{
			IMessageInterface::Execute_ShowText(HUD, this);
		}
	}

}