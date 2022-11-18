// Fill out your copyright notice in the Description page of Project Settings.


#include "InGameMenu.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"

bool UInGameMenu::Initialize()
{
	bool Success = Super::Initialize();
	if (!Success) return Success;

	if (ResumeButton == nullptr) return false;
	//if (OptionButton == nullptr) return false;
	if (MainMenuButton == nullptr) return false;
	if (ExitGameButton == nullptr) return false;

	ResumeButton->OnClicked.AddDynamic(this, &UInGameMenu::CallResumeButton);
	//OptionButton->OnClicked.AddDynamic(this, &UInGameMenu::CallOptionButton);
	MainMenuButton->OnClicked.AddDynamic(this, &UMenuWidget::CallMenuButton);
	ExitGameButton->OnClicked.AddDynamic(this, &UMenuWidget::CallExitButton);


	return Success;
}

void UInGameMenu::CallResumeButton()
{
	TearDown();
	UGameplayStatics::SetGamePaused(GetWorld(),false);
}

//void UInGameMenu::CallOptionButton()
//{
//	
//}

//void UInGameMenu::CallMainMenuButton()
//{
//	
//}
//
//void UInGameMenu::CallExitGameButton()
//{
//	UWorld* World = GetWorld();
//	if (World == nullptr) return;
//
//	APlayerController* PC = World->GetFirstPlayerController();
//	if (PC == nullptr) return;
//
//	PC->ConsoleCommand(TEXT("quit"));
//}
