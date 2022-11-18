// Fill out your copyright notice in the Description page of Project Settings.


#include "VictoryWidget.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Shooter/ShooterGameModeBase.h"
#include "Shooter/Shooter_GameInstance.h"


bool UVictoryWidget::Initialize()
{
	bool Success = Super::Initialize();
	if (!Success) return Success;

	if (ReturnMenuButton == nullptr) return false;
	if (ScoreText == nullptr) return false;

	ReturnMenuButton->OnClicked.AddDynamic(this, &UMenuWidget::CallMenuButton);
	auto Gamemode = Cast<AShooterGameModeBase>(GetWorld()->GetAuthGameMode());

	if (Gamemode)
	{
		const int32 Score = Gamemode->GetGameScore();
		

		ScoreText->SetText(FText::FromString(FString::FromInt(Score)));
	}

	return Success;
}

void UVictoryWidget::Setup()
{
	Super::Setup();


}

void UVictoryWidget::SetVictoryMsg(FString Msg)
{
	if (VictoryMsg)
	{
		VictoryMsg->SetText(FText::FromString(Msg));
	}
}

//void UVictoryWidget::CallMenuButton()
//{
//	auto Instance = Cast<UShooter_GameInstance>(GetGameInstance());
//	if (Instance)
//	{
//		UWorld* World = GetWorld();
//		if (World)
//		{
//			TearDown();
//			World->ServerTravel(FString("/Game/_Game/Maps/MainMenu"));
//			Instance->LoadMainMenu();
//		}
//	}
//}
