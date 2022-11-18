// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MenuWidget.h"
#include "MainMenu.generated.h"

/**
 * 
 */
UCLASS()
class SHOOTER_API UMainMenu : public UMenuWidget
{
	GENERATED_BODY()

protected:
	virtual bool Initialize() override;

public:
	UPROPERTY(Meta = (BindWidget))
	class UButton* StartButton;

	UPROPERTY(Meta = (BindWidget))
	class UButton* ExitButton;

private:
	UFUNCTION()
	void CallStartButton();

	/*UFUNCTION()
	void CallExitButton();*/
	
};
