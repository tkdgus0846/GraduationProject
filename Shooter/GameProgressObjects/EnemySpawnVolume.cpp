// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemySpawnVolume.h"
#include "Components/BoxComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Shooter/BulletHitObjects/Enemies/Enemy.h"

// Sets default values
AEnemySpawnVolume::AEnemySpawnVolume()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	WhereToSpawn = CreateDefaultSubobject<UBoxComponent>(TEXT("WhereToSpawn"));
	RootComponent = WhereToSpawn;

	SpawnDelayRangeLow = 1.f;
	SpawnDelayRangeHigh = 4.5f;
}

FVector AEnemySpawnVolume::GetRandomPointInVolume()
{
	FVector SpawnOrigin = WhereToSpawn->Bounds.Origin;
	FVector SpawnExtent = WhereToSpawn->Bounds.BoxExtent;
	return UKismetMathLibrary::RandomPointInBoundingBox(SpawnOrigin, SpawnExtent);
}

void AEnemySpawnVolume::SetSpawningActive(bool bShouldSpawn)
{
	//UE_LOG(LogTemp, Warning, TEXT("SetSpawningActive!!"));
	if (bShouldSpawn)
	{
		// Set the timer on Spawn Pickup
		SpawnDelay = FMath::FRandRange(SpawnDelayRangeLow, SpawnDelayRangeHigh);
		GetWorldTimerManager().SetTimer(SpawnTimer, this, &AEnemySpawnVolume::SpawnEnemies, SpawnDelay, false);
	}
	else
	{
		// clear the timer on Spawn Pickup
		GetWorldTimerManager().ClearTimer(SpawnTimer);
	}
}

// Called when the game starts or when spawned
void AEnemySpawnVolume::BeginPlay()
{
	Super::BeginPlay();
	
	SetSpawningActive(true);
}

// Called every frame
void AEnemySpawnVolume::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AEnemySpawnVolume::SpawnEnemies()
{
	if (WhatToSpawn != nullptr)
	{
		UWorld* const World = GetWorld();
		if (World)
		{
			FActorSpawnParameters SpawnParams;
			SpawnParams.Owner = this;
			SpawnParams.Instigator = GetInstigator();

			FVector SpawnLocation = GetRandomPointInVolume();
			FRotator SpawnRotation;
			//SpawnRotation.Yaw = FMath::FRand() * 360.f;
			/*SpawnRotation.Pitch = FMath::FRand() * 360.f;*/
			/*SpawnRotation.Roll = FMath::FRand() * 360.f;*/

			AEnemy* const SpawnedEnemy = World->SpawnActor<AEnemy>(WhatToSpawn, SpawnLocation, SpawnRotation, SpawnParams);

			SpawnDelay = FMath::FRandRange(SpawnDelayRangeLow, SpawnDelayRangeHigh);
			GetWorldTimerManager().SetTimer(SpawnTimer, this, &AEnemySpawnVolume::SpawnEnemies, SpawnDelay, false);
		}
	}
}

