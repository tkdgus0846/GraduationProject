// Fill out your copyright notice in the Description page of Project Settings.


#include "Crunch.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"
#include "Particles/ParticleSystemComponent.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/KismetMathLibrary.h"
#include "DrawDebugHelpers.h"
#include "EnemyController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Components/SphereComponent.h"
#include "../../Player/ShooterCharacter.h"
#include "Components/CapsuleComponent.h"
#include "Components/BoxComponent.h"
#include "Engine/SkeletalMeshSocket.h"
#include "Shooter/GameProgressObjects/VictoryPoint.h"

ACrunch::ACrunch()
{
	PrimaryActorTick.bCanEverTick = true;

	CombatRangeSphere = CreateDefaultSubobject<USphereComponent>(TEXT("CombatRangeSphere"));
	if (CombatRangeSphere == nullptr) return;

	CombatRangeSphere->SetupAttachment(GetRootComponent());

	LeftWeaponCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("LeftWeaponCollision"));
	if (LeftWeaponCollision == nullptr) return;
	LeftWeaponCollision->SetupAttachment(GetMesh(), TEXT("LeftWeaponBone"));

	RightWeaponCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("RightWeaponCollision"));
	if (RightWeaponCollision == nullptr) return;
	RightWeaponCollision->SetupAttachment(GetMesh(), TEXT("RightWeaponBone"));

	/*UE_LOG(LogTemp, Warning, TEXT("%s Call Constructor"), *GetFName().ToString());*/
}

void ACrunch::BeginPlay()
{
	Super::BeginPlay();

	/*UE_LOG(LogTemp, Warning, TEXT("%s Call BeginPlay"), *GetFName().ToString());*/

	if (CombatRangeSphere)
	{
		CombatRangeSphere->OnComponentBeginOverlap.AddDynamic(this, &ACrunch::CombatRangeSphereOverlap);
		CombatRangeSphere->OnComponentEndOverlap.AddDynamic(this, &ACrunch::CombatRangeSphereEndOverlap);
	}

	// 
	if (LeftWeaponCollision)
	{
		LeftWeaponCollision->OnComponentBeginOverlap.AddDynamic(this, &ACrunch::OnLeftWeaponOverlap);
		LeftWeaponCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		LeftWeaponCollision->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
		LeftWeaponCollision->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
		LeftWeaponCollision->SetCollisionResponseToChannel(
			ECollisionChannel::ECC_Pawn,
			ECollisionResponse::ECR_Overlap);
		LeftWeaponCollision->SetCollisionResponseToChannel(
			ECollisionChannel::ECC_GameTraceChannel3,
			ECollisionResponse::ECR_Overlap);
		
	}
	if (RightWeaponCollision)
	{
		RightWeaponCollision->OnComponentBeginOverlap.AddDynamic(this, &ACrunch::OnRightWeaponOverlap);
		RightWeaponCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		RightWeaponCollision->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
		RightWeaponCollision->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
		RightWeaponCollision->SetCollisionResponseToChannel(
			ECollisionChannel::ECC_Pawn,
			ECollisionResponse::ECR_Overlap);
		RightWeaponCollision->SetCollisionResponseToChannel(
			ECollisionChannel::ECC_GameTraceChannel3,
			ECollisionResponse::ECR_Overlap);
	}

	const FVector WorldPatrolPoint = UKismetMathLibrary::TransformLocation(
		GetActorTransform(),
		PatrolPoint);
	const FVector WorldPatrolPoint2 = UKismetMathLibrary::TransformLocation(
		GetActorTransform(),
		PatrolPoint2);

	/*DrawDebugSphere(
		GetWorld(),
		WorldPatrolPoint,
		25.f,
		12,
		FColor::Red,
		true);

	DrawDebugSphere(
		GetWorld(),
		WorldPatrolPoint2,
		25.f,
		12,
		FColor::Blue,
		true);*/

	if (GetEnemyController())
	{
		GetEnemyController()->GetBlackboardComponent()->SetValueAsVector(
			TEXT("PatrolPos1"),
			WorldPatrolPoint);
		GetEnemyController()->GetBlackboardComponent()->SetValueAsVector(
			TEXT("PatrolPos2"),
			WorldPatrolPoint2);
	}
	
}

void ACrunch::CombatRangeSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor == nullptr) return;
	auto ShooterCharacter = Cast<AShooterCharacter>(OtherActor);
	auto VictoryPoint = Cast<AVictoryPoint>(OtherActor);

	if (ShooterCharacter || VictoryPoint)
	{
		SetInAttackRange(true);

		if (GetEnemyController())
		{
			GetEnemyController()->GetBlackboardComponent()->SetValueAsBool(TEXT("InAttackRange"), GetInAttackRange());
		}
	}

}

void ACrunch::CombatRangeSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor == nullptr) return;
	auto ShooterCharacter = Cast<AShooterCharacter>(OtherActor);
	auto VictoryPoint = Cast<AVictoryPoint>(OtherActor);

	if (ShooterCharacter || VictoryPoint)
	{
		SetInAttackRange(false);

		if (GetEnemyController())
		{
			GetEnemyController()->GetBlackboardComponent()->SetValueAsBool(TEXT("InAttackRange"), GetInAttackRange());
		}
	}

}

void ACrunch::OnLeftWeaponOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor == nullptr) return;
	
	/*UE_LOG(LogTemp,Warning,TEXT("Overlap: %s"), *OtherActor->GetFName().ToString());*/
	AShooterCharacter* Character = Cast<AShooterCharacter>(OtherActor);
	AVictoryPoint* VictoryPoint = Cast<AVictoryPoint>(OtherActor);

	if (Character)
	{
		DoDamage(Character);
		SpawnBlood(Character, TEXT("FX_Blood_l"));
		StunCharacter(Character);
	}
	else if (VictoryPoint)
	{
		DoDamage(VictoryPoint);
	}
}

void ACrunch::OnRightWeaponOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor == nullptr) return;

	/*UE_LOG(LogTemp, Warning, TEXT("Overlap: %s"), *OtherActor->GetFName().ToString());*/
	AShooterCharacter* Character = Cast<AShooterCharacter>(OtherActor);
	AVictoryPoint* VictoryPoint = Cast<AVictoryPoint>(OtherActor);

	if (Character)
	{
		DoDamage(Character);
		SpawnBlood(Character, TEXT("FX_Blood_r"));
		StunCharacter(Character);
	}
	else if (VictoryPoint)
	{
		DoDamage(VictoryPoint);
	}	
}

void ACrunch::DoDamage(AShooterCharacter* Victim)
{
	if (Victim)
	{
		UGameplayStatics::ApplyDamage(
			Victim,
			GetBaseDamage(),
			GetEnemyController(),
			this,
			UDamageType::StaticClass());
		if (Victim->GetMeleeImpactSound())
		{
			UGameplayStatics::PlaySoundAtLocation(
				this,
				Victim->GetMeleeImpactSound(),
				GetActorLocation());
		}
	}
}

void ACrunch::DoDamage(AVictoryPoint* VictoryPoint)
{
	if (VictoryPoint)
	{
		UGameplayStatics::ApplyDamage(
			VictoryPoint,
			GetBaseDamage(),
			GetEnemyController(),
			this,
			UDamageType::StaticClass());
		/*if (VictoryPoint->GetMeleeImpactSound())
		{
			UGameplayStatics::PlaySoundAtLocation(
				this,
				VictoryPoint->GetMeleeImpactSound(),
				GetActorLocation());
		}*/
	}
}

void ACrunch::ActivateLeftWeapon()
{
	LeftWeaponCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
}

void ACrunch::DeactivateLeftWeapon()
{
	LeftWeaponCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void ACrunch::ActivateRightWeapon()
{
	RightWeaponCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
}

void ACrunch::DeactivateRightWeapon()
{
	RightWeaponCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}