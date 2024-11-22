#include "Commands/PlaySpeech.h"

#include "Actors/MapEventActor.h"

#include "Components/AudioComponent.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(PlaySpeech)

void UPlaySpeech::Execute(APlayerController* Player, AMapEventActor* MapEventActor)
{
	if (MapEventActor != nullptr)
	{
		if (UObject* AudioSource = MapEventActor->GetDefaultSubobjectByName("AudioSource"))
		{
			if (UAudioComponent* AudioComponent = CastChecked<UAudioComponent>(AudioSource))
			{
				AudioComponent->SetSound(Speech);
				AudioComponent->Play();
			}
		}
	}
}