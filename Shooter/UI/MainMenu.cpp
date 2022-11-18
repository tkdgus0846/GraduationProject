// Fill out your copyright notice in the Description page of Project Settings.


#include "MainMenu.h"
#include "Components/Button.h"

bool UMainMenu::Initialize()
{
	bool Success = Super::Initialize();
	if (!Success) return Success;

	if (StartButton == nullptr) return false;
	if (ExitButton == nullptr) return false;

	StartButton->OnClicked.AddDynamic(this, &UMainMenu::CallStartButton);
	ExitButton->OnClicked.AddDynamic(this, &UMenuWidget::CallExitButton);

	return Success;
}

void UMainMenu::CallStartButton()
{
	UWorld* World = GetWorld();
	if (World)
	{
		TearDown();
		World->ServerTravel(FString("/Game/_Game/Maps/GameMap"));
	}
}

//void UMainMenu::CallExitButton()
//{
//	UWorld* World = GetWorld();
//	if (World == nullptr) return;
//
//	APlayerController* PC = World->GetFirstPlayerController();
//	if (PC == nullptr) return;
//
//	PC->ConsoleCommand(TEXT("quit"));
//}
