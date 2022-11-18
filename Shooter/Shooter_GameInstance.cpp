// Fill out your copyright notice in the Description page of Project Settings.


#include "Shooter_GameInstance.h"

#include "Kismet/GameplayStatics.h"
#include "UI/InGameMenu.h"
#include "UI/VictoryWidget.h"
#include "UI/MainMenu.h"
#include "ShooterGameModeBase.h"

UShooter_GameInstance::UShooter_GameInstance()
{

}

void UShooter_GameInstance::LoadInGameMenu()
{
	if (InGameMenuClass == nullptr) return;
	
	InGameMenu = CreateWidget<UInGameMenu>(this, InGameMenuClass);
	if (InGameMenu == nullptr) return;

	InGameMenu->Setup();
	UGameplayStatics::SetGamePaused(GetWorld(), true);
}

void UShooter_GameInstance::LoadVictoryWidget(FString VictoryMsg)
{

	auto Mode = Cast<AShooterGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));
	if (Mode)
	{
		Mode->SetActorDilation(0.2f);
	}

	if (VictoryWidgetClass == nullptr) return;

	VictoryWidget = CreateWidget<UVictoryWidget>(this, VictoryWidgetClass);
	if (VictoryWidget == nullptr) return;



	VictoryWidget->SetVictoryMsg(VictoryMsg);
	VictoryWidget->Setup();
	
	FTimerHandle Handle;
	GetTimerManager().SetTimer(
		Handle,
		this,
		&UShooter_GameInstance::SetGamePaused,
		3.0f
	);
}

void UShooter_GameInstance::LoadMainMenu()
{
	if (MainMenuClass == nullptr) return;

	MainMenu = CreateWidget<UMainMenu>(this, MainMenuClass);
	if (MainMenu == nullptr) return;

	MainMenu->Setup();
}

void UShooter_GameInstance::SetGamePaused()
{
	UGameplayStatics::SetGamePaused(GetWorld(), true);
}
