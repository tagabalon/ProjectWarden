// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Factories/Factory.h"
#include "MapEventFactory.generated.h"

UCLASS(HideCategories = Object)
class RPGSYSTEMEDITOR_API UMapEventFactory : public UFactory
{
	GENERATED_UCLASS_BODY()
	
	UPROPERTY(EditAnywhere, Category = Asset)
	TSubclassOf<class UMapEvent> AssetClass;

	virtual bool ConfigureProperties() override;
	virtual UObject* FactoryCreateNew(UClass* Class, UObject* InParent, FName Name, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn) override;

};
