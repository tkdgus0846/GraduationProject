// Fill out your copyright notice in the Description page of Project Settings.


#include "VictoryPoint.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "Shooter/Player/ShooterCharacter.h"
#include "Shooter/Shooter_GameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "Shooter/ShooterGameModeBase.h"
#include "LevelSequenceActor.h"
#include "LevelSequencePlayer.h"
#include "Shooter/Player/ShooterPlayerController.h"

// Sets default values
AVictoryPoint::AVictoryPoint():
	bCanWin(false),
	Health(500.f),
	MaxHealth(500.f)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Sphere = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere"));
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Signal = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Signal"));

	if (Mesh)
	{
		RootComponent = Mesh;
	}

	if (Sphere)
	{
		Sphere->SetupAttachment(Mesh);
	}

	if (Signal)
	{
		Signal->SetupAttachment(Mesh);
	}


}

void AVictoryPoint::EnableSignal()
{
	if (Signal)
	{
		
		auto Controller = Cast<AShooterPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
		if (Controller)
		{
			Controller->SetHUDOverlayVisibility(true);
			
		}

		auto Mode = Cast<AShooterGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));
		if (Mode)
		{
			Mode->SetActorDilation(1.f);
		}
	}

	SetCanWin(true);
	SetHUDVisibility(true);
}

void AVictoryPoint::DisableSignal()
{
	if (Signal)
	{
		Signal->SetVisibility(false);
		SetCanWin(false);
	}
}

void AVictoryPoint::PlaySignalSequence()
{
	if (SequenceEnabledSignal)
	{
		SequenceEnabledSignal->SequencePlayer->Play();
		if (Signal)
			Signal->SetVisibility(true);

		auto Controller = Cast<AShooterPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(),0));
		if (Controller)
		{
			Controller->SetHUDOverlayVisibility(false);
		}
		
		SetHUDVisibility(false);

		auto Mode = Cast<AShooterGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));
		if (Mode)
		{
			Mode->SetActorDilation(0.f);
		}
	}

	FTimerHandle Handle;
	GetWorldTimerManager().SetTimer(Handle,this,&AVictoryPoint::EnableSignal,4.f);
}

void AVictoryPoint::PlayDestroySignalSequence()
{
	if (SequenceDestroyedSignal)
	{
		SequenceDestroyedSignal->SequencePlayer->Play();

		auto Controller = Cast<AShooterPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
		if (Controller)
		{
			Controller->SetHUDOverlayVisibility(false);
		}

		SetHUDVisibility(false);

		auto Mode = Cast<AShooterGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));
		if (Mode)
		{
			Mode->SetActorDilation(0.f);
		}
	}
}

float AVictoryPoint::TakeDamage(float Damage, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	if (Health - Damage <= 0.f)
	{
		Health = 0.f;

		PlayDestroySignalSequence();

		FTimerHandle Handle;
		GetWorldTimerManager().SetTimer(Handle, this, &AVictoryPoint::LoadFailedWidget, 2.5f);
	}
	else
	{
		Health -= Damage;
	}
	return Damage;
}

void AVictoryPoint::LoadFailedWidget()
{
	auto Instance = Cast<UShooter_GameInstance>(GetGameInstance());
	if (Instance)
	{
		Instance->LoadVictoryWidget(TEXT("Failed.."));
	}
}

// Called when the game starts or when spawned
void AVictoryPoint::BeginPlay()
{
	Super::BeginPlay();
	
	
	if (Sphere)
	{
		Sphere->OnComponentBeginOverlap.AddDynamic(this, &AVictoryPoint::SphereOverlap);
	}

	DisableSignal();

	auto Mode = Cast<AShooterGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));
	if (Mode)
	{
		Mode->WinDelegate.AddUFunction(this,FName("PlaySignalSequence"));
	}
	
}

void AVictoryPoint::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	auto Mode = Cast<AShooterGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));
	if (Mode)
	{
		Mode->SetVictoryPoint(this);
	}
}

void AVictoryPoint::SphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (bCanWin)
	{
		AShooterCharacter* Character = Cast<AShooterCharacter>(OtherActor);
		auto Instance = Cast<UShooter_GameInstance>(GetGameInstance());
		if (Character)
		{
			if (Instance)
			{
				Instance->LoadVictoryWidget("Victory");
			}
		}
	}
	
}

void AVictoryPoint::ShowHealthBar_Implementation()
{
}

// Called every frame
void AVictoryPoint::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

