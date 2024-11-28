#include "Commands/PlaySpeech.h"

#include "Actors/MapEventActor.h"
#include "Commands/ShowText.h"
#include "Interface/MessageInterface.h"
#include "RPGSettings.h"
#include "MapEvent.h"

#include "Components/AudioComponent.h"
#include "GameFramework/HUD.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(PlaySpeech)

void UPlaySpeech::Execute(APlayerController* InPlayer, AMapEventActor* InMapEventActor)
{
	Player = InPlayer;
	MapEventActor = InMapEventActor;
	if (MapEventActor != nullptr)
	{
		if (UObject* AudioSource = MapEventActor->GetDefaultSubobjectByName("AudioSource"))
		{
			AudioComponent = CastChecked<UAudioComponent>(AudioSource);
			if (AudioComponent != nullptr)
			{
				AudioComponent->OnAudioFinished.Add(SpeechPlayed);
				SpeechPlayed.BindUFunction(this, "OnSpeechPlayed");

				AudioComponent->SetSound(Speech);
				AudioComponent->Play();
			}
		}

		if (AHUD* HUD = Player->GetHUD())
		{
			if (HUD->GetClass()->ImplementsInterface(UMessageInterface::StaticClass()))
			{
				IMessageInterface::Execute_ShowCaption(HUD, FText::FromString("Actor"), Text);
			}
		}
	}
}


void UPlaySpeech::OnSpeechPlayed()
{
	AudioComponent->OnAudioFinished.Remove(SpeechPlayed);

	if (NextCommand != nullptr)
	{
		NextCommand->Execute(Player, MapEventActor);
		if (NextCommand->IsA(UShowText::StaticClass()) || NextCommand->IsA(StaticClass()))
		{
			return;
		}
	}
	if (AudioComponent != nullptr)
	{
		AudioComponent->SetSound(nullptr);
	}

	if (AHUD* HUD = Player->GetHUD())
	{
		if (HUD->GetClass()->ImplementsInterface(UMessageInterface::StaticClass()))
		{
			IMessageInterface::Execute_CloseMessages(HUD);
		}
	}
}