// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MenuWidget.h"
#include "InGameMenu.generated.h"

/**
 * 
 */
UCLASS()
class SHOOTER_API UInGameMenu : public UMenuWidget
{
	GENERATED_BODY()
	
protected:
	virtual bool Initialize() override;

private:
	UPROPERTY(Meta = (BindWidget))
	class UButton* ResumeButton;

	/*UPROPERTY(Meta = (BindWidget))
	class UButton* OptionButton;*/

	UPROPERTY(Meta = (BindWidget))
	class UButton* MainMenuButton;

	UPROPERTY(Meta = (BindWidget))
	class UButton* ExitGameButton;

	UFUNCTION()
	void CallResumeButton();

	/*UFUNCTION()
	void CallOptionButton();*/

	/*UFUNCTION()
	void CallMainMenuButton();

	UFUNCTION()
	void CallExitGameButton();*/
};
