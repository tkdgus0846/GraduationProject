// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EnemySpawnVolume.generated.h"

UCLASS()
class SHOOTER_API AEnemySpawnVolume : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AEnemySpawnVolume();

	UFUNCTION(BlueprintPure, Category = "Spawning")
	FVector GetRandomPointInVolume();

	UFUNCTION(BlueprintCallable, Category = "Spawning")
	void SetSpawningActive(bool bShouldSpawn);

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnyWhere, Category = "Spawning")
	TSubclassOf<class AEnemy> WhatToSpawn;

	FTimerHandle SpawnTimer;

	// Minimum spawn delay
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Spawning")
	float SpawnDelayRangeLow;

	// Maximum spawn delay
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Spawning")
	float SpawnDelayRangeHigh;

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawning", Meta = (AllowPrivateAccess = true))
	class UBoxComponent* WhereToSpawn;

	void SpawnEnemies();

	float SpawnDelay;

public:	
	

};
