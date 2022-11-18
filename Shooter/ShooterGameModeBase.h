// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "ShooterGameModeBase.generated.h"

DECLARE_MULTICAST_DELEGATE(FWinDelegate);
DECLARE_MULTICAST_DELEGATE_OneParam(FActorDilationDelegate,float);

UCLASS()
class SHOOTER_API AShooterGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
public:
	AShooterGameModeBase();

	
	void SetActorDilation(float Dilation);

	FWinDelegate WinDelegate;
	FActorDilationDelegate ActorDilationDelegate;
protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Meta = (AllowPrivateAccess))
	float MatchStartTime;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Meta = (AllowPrivateAccess))
	float MatchElapsedTime;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Meta = (AllowPrivateAccess))
	int32 GameScore;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Meta = (AllowPrivateAccess))
	int32 GameMinute;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Meta = (AllowPrivateAccess))
	int32 GameSeconds;

	void CalculateTime();

	FTimerHandle WorldTimeHandle;

	UPROPERTY()
	class AVictoryPoint* VictoryPoint;

public:
	FORCEINLINE void AddGameScore(int32 Score) {GameScore = GameScore+Score;}
	FORCEINLINE int32 GetGameScore() {return GameScore;}
	FORCEINLINE float GetMatchElapsedTime() {return MatchElapsedTime;}
	FORCEINLINE class AVictoryPoint* GetVictoryPoint() { return VictoryPoint;}
	void SetVictoryPoint(class AVictoryPoint* Point);
};
