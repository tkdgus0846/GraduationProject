// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "RootWidget.generated.h"

/**
 * 
 */
UCLASS()
class SHOOTER_API URootWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	//virtual void SetMenuInterface(IMenuInterface* IMenu);
	virtual void Setup();
	virtual void TearDown();
};
