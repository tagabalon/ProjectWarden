// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MapEvent.h"
#include "MapEventActor.generated.h"

class AMainCharacter;
class UInteractInterface;
class USphereComponent;
class UWidgetComponent;
class UInputAction;

UCLASS()
class RPGSYSTEM_API AMapEventActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMapEventActor();

	UFUNCTION(BlueprintCallable, Category="Components|Sphere")
	void SetPromptDistance(float InPromptDistance);

	UFUNCTION()
	void OnInteract();

#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnSphereLeave(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);


	UPROPERTY(EditInstanceOnly, Category="Map Event")
	UMapEvent* MapEvent = nullptr;

	/** The radius of the sphere **/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, export, Category = "Map Event")
	float GlintDistance = 600.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, export, Category = "Map Event")
	float PromptDistance = 200.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, export, Category = "Map Event")
	float PromptAngle = 30.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Map Event")
	TSubclassOf<UUserWidget> PromptWidget;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Components")
	USphereComponent* Sphere;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Components")
	UWidgetComponent* Prompt;

	UPROPERTY(EditAnywhere, Category = "RPG Input")
	UInputAction* InputInteract;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	bool CharacterCanBePrompted() const;

	AMainCharacter* GlintedCharacter = nullptr;
	UUserWidget* InteractWidget = nullptr;
	bool bIsCharacterPrompted = false;

};
