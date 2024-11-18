// Fill out your copyright notice in the Description page of Project Settings.

#include "Asset/MapEventFactory.h"

#include "Graph/MapEventGraph.h"
#include "MapEvent.h"

#include "Kismet2/BlueprintEditorUtils.h"

#define LOCTEXT_NAMESPACE "MapEventFactory"

UMapEventFactory::UMapEventFactory(const FObjectInitializer& objectInitializer)
	: UFactory(objectInitializer)
{
	SupportedClass = UMapEvent::StaticClass();

	bCreateNew = true;
	bEditorImport = false;
	bEditAfterNew = true;
}

bool UMapEventFactory::ConfigureProperties()
{
	//const FText TitleText = LOCTEXT("CreateMapEventOptions", "Pick Map Event Class");

	return true;// eConfigurePropertiesInternal(TitleText);//// ConfigurePropertiesInternal(TitleText);
}

UObject* UMapEventFactory::FactoryCreateNew(UClass* uckass, UObject* inParent, FName name, EObjectFlags flags, UObject* context, FFeedbackContext* warn)
{
	UMapEvent* MapEvent = NewObject<UMapEvent>(inParent, name, flags);

	//UMapEventGraph::CreateGraph(MapEvent);

	return MapEvent;
}

#undef LOCTEXT_NAMESPACE