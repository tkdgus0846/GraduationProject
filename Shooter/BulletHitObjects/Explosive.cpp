// Fill out your copyright notice in the Description page of Project Settings.


#include "Explosive.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"
#include "Particles/ParticleSystemComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"
#include "Components/CapsuleComponent.h"

#include "GameFramework/Character.h"
#include "Enemies/Enemy.h"
#include "Kismet/GameplayStatics.h"


// Sets default values
AExplosive::AExplosive() :
	Damage(100.f)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ExplosiveMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ExplosiveMesh"));
	SetRootComponent(ExplosiveMesh);

	OverlapSphere = CreateDefaultSubobject<USphereComponent>(TEXT("OverlapSphere"));
	OverlapSphere->SetupAttachment(GetRootComponent());
}

// Called when the game starts or when spawned
void AExplosive::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AExplosive::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AExplosive::BulletHit_Implementation(FHitResult HitResult, AActor* Shooter, AController* ShooterController)
{
	UE_LOG(LogTemp, Warning, TEXT("Bullet Hits!!"));

	DoExplosion(Shooter, ShooterController);
}

void AExplosive::DoExplosion(AActor* Shooter, AController* ShooterController)
{
	if (ExplodeSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, ExplodeSound, GetActorLocation());
	}

	if (ExplodeParticles)
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ExplodeParticles, GetActorLocation(), FRotator(0.f), true);
	}

	// TODO: Apply explosive damage
	TArray<AActor*> OverlappingActors;
	TArray<UPrimitiveComponent*> OverlappingComponents;
	GetOverlappingComponents(OverlappingComponents);
	for (auto Component : OverlappingComponents)
	{

		UCapsuleComponent* Item = Cast<UCapsuleComponent>(Component);
		if (Item != nullptr)
		{
			UE_LOG(LogTemp, Warning, TEXT("Damaged by explosive: %s"), *Component->GetOwner()->GetName());
			UGameplayStatics::ApplyDamage(
				Component->GetOwner(),
				Damage,
				ShooterController,
				Shooter,
				UDamageType::StaticClass());
		}
	}
	/*GetOverlappingActors(OverlappingActors, ACharacter::StaticClass());

	for (auto Actor : OverlappingActors)
	{
		UE_LOG(LogTemp, Warning, TEXT("Actor Damaged by explosive: %s"),*Actor->GetName());

		UGameplayStatics::ApplyDamage(
			Actor,
			Damage,
			ShooterController,
			Shooter,
			UDamageType::StaticClass());
	}*/
	Destroy();
}

