// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RootWidget.h"
#include "MenuWidget.generated.h"

/**
 * 
 */
UCLASS()
class SHOOTER_API UMenuWidget : public URootWidget
{
	GENERATED_BODY()

public:
	virtual void Setup() override;
	virtual void TearDown() override;

	UFUNCTION()
	virtual void CallMenuButton();

	UFUNCTION()
	virtual void CallExitButton();
	
};
