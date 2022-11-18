// Fill out your copyright notice in the Description page of Project Settings.


#include "Projectile.h"
#include "Components/SphereComponent.h"
#include "Engine/Classes/GameFramework/ProjectileMovementComponent.h"
#include "Enemies/Enemy.h"
#include "Shooter/Player/ShooterCharacter.h"

// Sets default values
AProjectile::AProjectile() :
	Target(nullptr),
	bFollowTarget(false),
	bAccelerated(false),
	ProjectileSpeed(6000.f)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
	CollisionComponent->InitSphereRadius(15.0f);
	RootComponent = CollisionComponent;

	OverlapSphere->SetupAttachment(RootComponent);
	ExplosiveMesh->SetupAttachment(RootComponent);

	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComponent"));
	ProjectileMovementComponent->SetUpdatedComponent(CollisionComponent);
	ProjectileMovementComponent->InitialSpeed = ProjectileSpeed;
	ProjectileMovementComponent->MaxSpeed = ProjectileSpeed;
	ProjectileMovementComponent->bRotationFollowsVelocity = true;
	ProjectileMovementComponent->bShouldBounce = false;
	ProjectileMovementComponent->ProjectileGravityScale = 0.f;

	InitialLifeSpan = 4.f;

}

// Called when the game starts or when spawned
void AProjectile::BeginPlay()
{
	Super::BeginPlay();

	if (CollisionComponent)
	{
		CollisionComponent->OnComponentBeginOverlap.AddDynamic(this, &AProjectile::CollisionSphereOverlap);
		
	}
}

// Called every frame
void AProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//Move(DeltaTime);
}

void AProjectile::Launch(FVector StartLocation, AActor* NewTarget, bool bFollow)
{
	SetTarget(NewTarget);
	SetFollow(bFollow);

	if (Target == nullptr)
	{
		UE_LOG(LogTemp,Warning,TEXT("Target Is NONE!!"));
		return;
	}

	TargetInitialLocation = Target->GetActorLocation();

	if (bFollowTarget)
	{
		ProjectileMovementComponent->bIsHomingProjectile = true;
		ProjectileMovementComponent->HomingTargetComponent = NewTarget->GetRootComponent();
		/*UE_LOG(LogTemp, Warning, TEXT("HomingProjectile: %d %s"),
			ProjectileMovementComponent->bIsHomingProjectile, *ProjectileMovementComponent->HomingTargetComponent->GetFName().ToString());*/

	}
	else
	{
		ProjectileMovementComponent->bIsHomingProjectile = false;
	}

	FVector Velocity = TargetInitialLocation - StartLocation;
	
	const bool bNormalizeSucceed = Velocity.Normalize();
	if (bNormalizeSucceed == true)
	{
		ProjectileMovementComponent->Velocity = Velocity * 10000;
		/*UE_LOG(LogTemp, Warning, TEXT("Velocity: %f %f %f Magnitude: %f"),
			Velocity.X, Velocity.Y, Velocity.Z, Velocity.Size());
		UE_LOG(LogTemp, Warning, TEXT("Component Velocity: %f %f %f Magnitude: %f"),
			ProjectileMovementComponent->Velocity.X, ProjectileMovementComponent->Velocity.Y, ProjectileMovementComponent->Velocity.Z, ProjectileMovementComponent->Velocity.Size());*/
	}
	
	
	
}

void AProjectile::Move(float DeltaTime)
{
	if (Target == nullptr) return;

	/*if (bFollowTarget == true)
	{
		SetActorLocation()
	}
	else
	{
		SetActorLocation()
	}*/
}

void AProjectile::CollisionSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	UE_LOG(LogTemp,Warning,TEXT("Overlap: %s"),*OtherActor->GetFName().ToString());
	AShooterCharacter* Character = Cast<AShooterCharacter>(OtherActor);
	if (Character)
	{
		/*DoExplosion();*/
	}
}

void AProjectile::CollisionSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
}

//void AProjectile::BulletHit_Implementation(FHitResult HitResult, AActor* Shooter, AController* ShooterController)
//{
//	Super::BulletHit_Implementation(HitResult, Shooter, ShooterController);
//	UE_LOG(LogTemp,Warning,TEXT("Bullet Hits!!"));
//}

