// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Shooter_GameInstance.generated.h"

/**
 * 
 */
UCLASS()
class SHOOTER_API UShooter_GameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	UShooter_GameInstance();

	UFUNCTION(BlueprintCallable)
	void LoadInGameMenu();

	UFUNCTION(BlueprintCallable)
	void LoadVictoryWidget(FString VictoryMsg);

	UFUNCTION(BlueprintCallable)
	void LoadMainMenu();
private:
	void SetGamePaused();

	UPROPERTY(EditAnywhere, Category = Widget, Meta = (AllowPrivateAccess))
	TSubclassOf<class UUserWidget> InGameMenuClass;

	class UInGameMenu* InGameMenu;

	UPROPERTY(EditAnywhere, Category = Widget, Meta = (AllowPrivateAccess))
	TSubclassOf<class UUserWidget> VictoryWidgetClass;

	class UVictoryWidget* VictoryWidget;

	UPROPERTY(EditAnywhere, Category = Widget, Meta = (AllowPrivateAccess))
	TSubclassOf<class UUserWidget> MainMenuClass;

	class UMainMenu* MainMenu;
};
