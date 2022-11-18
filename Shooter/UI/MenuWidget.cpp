// Fill out your copyright notice in the Description page of Project Settings.


#include "MenuWidget.h"

#include "Shooter/Shooter_GameInstance.h"

void UMenuWidget::Setup()
{
	this->AddToViewport();

	UWorld* World = GetWorld();
	if (World == nullptr) return;

	APlayerController* PlayerController = World->GetFirstPlayerController();
	if (PlayerController == nullptr) return;

	FInputModeUIOnly InputModeData;

	this->bIsFocusable = true;
	/*InputModeData.SetWidgetToFocus(this->TakeWidget());*/
	InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	PlayerController->SetInputMode(InputModeData);
	PlayerController->bShowMouseCursor = true;
}

void UMenuWidget::TearDown()
{
	this->RemoveFromViewport();
	this->bIsFocusable = false;

	UWorld* World = GetWorld();
	if (World == nullptr) return;

	APlayerController* PlayerController = World->GetFirstPlayerController();
	if (PlayerController == nullptr) return;

	FInputModeGameOnly InputModeData;

	PlayerController->SetInputMode(InputModeData);
	PlayerController->bShowMouseCursor = false;
}

void UMenuWidget::CallMenuButton()
{
	auto Instance = Cast<UShooter_GameInstance>(GetGameInstance());
	if (Instance)
	{
		UWorld* World = GetWorld();
		if (World)
		{
			TearDown();
			World->ServerTravel(FString("/Game/_Game/Maps/MainMenu"));
			
			Instance->LoadMainMenu();
		}
	}
}

void UMenuWidget::CallExitButton()
{
	UWorld* World = GetWorld();
	if (World == nullptr) return;

	APlayerController* PC = World->GetFirstPlayerController();
	if (PC == nullptr) return;

	PC->ConsoleCommand(TEXT("quit"));
}
