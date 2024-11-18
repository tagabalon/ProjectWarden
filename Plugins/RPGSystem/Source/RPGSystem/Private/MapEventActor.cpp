// Fill out your copyright notice in the Description page of Project Settings.


#include "MapEventActor.h"

#include "MainCharacter.h"
#include "Interface/InteractInterface.h"

#include "Components/SphereComponent.h"
#include "Components/WidgetComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
AMapEventActor::AMapEventActor()
	:  MapEvent(nullptr)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>("DefaultSceneRoot");

	Sphere = CreateDefaultSubobject<USphereComponent>(FName("SphereBounds"));
	Sphere->SetupAttachment(RootComponent);
	Sphere->InitSphereRadius(GlintDistance);

	Prompt = CreateDefaultSubobject<UWidgetComponent>(FName("Prompt"));
	Prompt->SetupAttachment(RootComponent);
	Prompt->SetWidgetSpace(EWidgetSpace::Screen);
	Prompt->SetDrawSize(FVector2D(140.f, 100.0f));

	static ConstructorHelpers::FClassFinder<UUserWidget> WidgetFinder(TEXT("WidgetBlueprint'/RPGSystem/Widgets/Interact/W_InteractPrompt.W_InteractPrompt_C'"));
	if (WidgetFinder.Succeeded())
	{
		PromptWidget = WidgetFinder.Class;
		Prompt->SetWidgetClass(PromptWidget);
	}
}

// Called when the game starts or when spawned
void AMapEventActor::BeginPlay()
{
	Super::BeginPlay();

	bIsCharacterPrompted = false;

	Sphere->OnComponentBeginOverlap.AddDynamic(this, &AMapEventActor::OnSphereOverlap);
	Sphere->OnComponentEndOverlap.AddDynamic(this, &AMapEventActor::OnSphereLeave);

	if (UUserWidget* PromptObject = Prompt->GetUserWidgetObject())
	{
		if (PromptObject->GetClass()->ImplementsInterface(UInteractInterface::StaticClass()))
		{
			InteractWidget = PromptObject;
		}
	}
}

// Called every frame
void AMapEventActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bIsCharacterPrompted)
	{
		if (!CharacterCanBePrompted())
		{
			IInteractInterface::Execute_ShowGlint(InteractWidget);
			GlintedCharacter->SetInteractable(nullptr);
			bIsCharacterPrompted = false;
		}
	}
	else
	{
		if (CharacterCanBePrompted())
		{
			IInteractInterface::Execute_ShowPrompt(InteractWidget);
			GlintedCharacter->SetInteractable(this);
			bIsCharacterPrompted = true;
		}
	}
}

bool AMapEventActor::CharacterCanBePrompted() const
{
	if (GlintedCharacter != nullptr)
	{
		float squareDistance = (GetActorLocation() - GlintedCharacter->GetActorLocation()).SquaredLength();
		return squareDistance < PromptDistance * PromptDistance;
	}
	return false;
}


void AMapEventActor::OnInteract()
{
	if (MapEvent != nullptr)
	{
		MapEvent->Execute(UGameplayStatics::GetPlayerController(this, 0));
	}
}

void AMapEventActor::SetPromptDistance(float InPromptDistance)
{
	PromptDistance = InPromptDistance;
	Sphere->SetSphereRadius(PromptDistance);
}

void AMapEventActor::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && OtherActor->IsA(AMainCharacter::StaticClass()))
	{
		GlintedCharacter = Cast<AMainCharacter>(OtherActor);
		bIsCharacterPrompted = false;

		IInteractInterface::Execute_ShowGlint(InteractWidget);
	}
}

void AMapEventActor::OnSphereLeave(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	IInteractInterface::Execute_HideInteract(InteractWidget);
	GlintedCharacter = nullptr;
	bIsCharacterPrompted = false;

}

#if WITH_EDITOR

void AMapEventActor::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	FName PropertyName = (PropertyChangedEvent.Property != nullptr) ? PropertyChangedEvent.Property->GetFName() : NAME_None;
	if (PropertyName == FName("GlintDistance"))
	{
		Sphere->InitSphereRadius(GlintDistance);
	}
	else if (PropertyName == FName("PromptWidget"))
	{
		/*if (UUserWidget::StaticClass() !=  PropertyChangedEvent.GetObjectBeingEdited(0)->GetClass())
		{
		}*/
	}
}

#endif