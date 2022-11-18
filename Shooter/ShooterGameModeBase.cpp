// Copyright Epic Games, Inc. All Rights Reserved.


#include "ShooterGameModeBase.h"
#include "Shooter/Shooter_GameInstance.h"
#include "Shooter/GameProgressObjects/VictoryPoint.h"

AShooterGameModeBase::AShooterGameModeBase():
	MatchStartTime(0.f),
	GameScore(0),
	GameMinute(5),
	GameSeconds(0)
{
	PrimaryActorTick.bCanEverTick = true;

}



void AShooterGameModeBase::SetActorDilation(float Dilation)
{
	ActorDilationDelegate.Broadcast(Dilation);
}

void AShooterGameModeBase::BeginPlay()
{
	Super::BeginPlay();

	GetWorldTimerManager().SetTimer(WorldTimeHandle,this,&AShooterGameModeBase::CalculateTime,1.0f,true);
	
}

void AShooterGameModeBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	UWorld* World = GetWorld();


}

void AShooterGameModeBase::CalculateTime()
{

	if (GameMinute == 0 && GameSeconds == 0)
	{
		GetWorldTimerManager().ClearTimer(WorldTimeHandle);
		WinDelegate.Broadcast();
		return;
	}

	GameSeconds = GameSeconds-1;
	if (GameSeconds < 0)
	{
		GameMinute = GameMinute - 1;
		GameSeconds = 59;
	}

}

void AShooterGameModeBase::SetVictoryPoint(AVictoryPoint* Point)
{
	VictoryPoint = Point;
}
