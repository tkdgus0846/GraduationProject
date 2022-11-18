// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy.h"
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
#include "Shooter/ShooterGameModeBase.h"
#include "Shooter/GameProgressObjects/VictoryPoint.h"
#include "Shooter/Items/Ammo.h"
#include "Shooter/Items/Weapon.h"

// Sets default values
AEnemy::AEnemy() :
	Health(100.f),
	MaxHealth(100.f),
	HealthBarDisplayTime(4.f),
	bCanHitReact(true),
	HitReactTimeMin(0.5f),
	HitReactTimeMax(3.f),
	HitNumberDestroyTime(1.5f),
	bStunned(false),
	StunChance(0.5f),
	BaseDamage(20.f),
	bCanAttack(true),
	AttackWaitTime(1.f),
	bDying(false),
	DeathTime(4.f),
	Score(100),
	bIsDropItems(true)
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	DetectSphere = CreateDefaultSubobject<USphereComponent>(TEXT("DetectSphere"));
	if (DetectSphere == nullptr) return;

	DetectSphere->SetupAttachment(GetRootComponent());

}

// Called when the game starts or when spawned
void AEnemy::BeginPlay()
{
	Super::BeginPlay();

	if (DetectSphere)
	{
		DetectSphere->OnComponentBeginOverlap.AddDynamic(this, &AEnemy::DetectSphereOverlap);
		/*DetectSphere->OnComponentEndOverlap.AddDynamic(this, &AEnemy::DetectSphereEndOverlap);*/
	}
	
	GetMesh()->SetCollisionResponseToChannel(
		ECollisionChannel::ECC_GameTraceChannel4,
		ECollisionResponse::ECR_Block);
	GetMesh()->SetCollisionResponseToChannel(
		ECollisionChannel::ECC_Camera,
		ECollisionResponse::ECR_Ignore);
	GetCapsuleComponent()->SetCollisionResponseToChannel(
		ECollisionChannel::ECC_Camera,
		ECollisionResponse::ECR_Ignore);

	EnemyController = Cast<AEnemyController>(GetController());
	if (EnemyController == nullptr) return;

	EnemyController->GetBlackboardComponent()->SetValueAsBool(
		TEXT("CanAttack"),
		true);

	EnemyController->RunBehaviorTree(BehaviorTree);

	auto Mode = Cast<AShooterGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));
	if (Mode)
	{
		Mode->ActorDilationDelegate.AddUFunction(this, FName("SetCustomDilation"));

		if (Mode->GetVictoryPoint())
		{
			EnemyController->GetBlackboardComponent()->SetValueAsObject(TEXT("Target"), Mode->GetVictoryPoint());
		}

	}

	
}

void AEnemy::ShowHealthBar_Implementation()
{
	GetWorldTimerManager().ClearTimer(HealthBarTimer);
	GetWorldTimerManager().SetTimer(
		HealthBarTimer,
		this,
		&AEnemy::HideHealthBar,
		HealthBarDisplayTime);
}

void AEnemy::Die()
{
	if (bDying) return;
	bDying = true;

	HideHealthBar();

	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance && HitMontage)
	{
		AnimInstance->Montage_Play(HitMontage);
		AnimInstance->Montage_JumpToSection(TEXT("Death"));
		/*UE_LOG(LogTemp, Warning, TEXT("Death!!"));*/
	}

	if (EnemyController)
	{
		EnemyController->GetBlackboardComponent()->SetValueAsBool(
			TEXT("Dead"),
			true);
		EnemyController->StopMovement();
		/*UE_LOG(LogTemp, Warning, TEXT("Death!! StopMovement!!"));*/

	}
}

void AEnemy::PlayHitMontage(FName Section, float PlayRate)
{
	if (bCanHitReact)
	{
		UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
		if (AnimInstance)
		{
			/*UE_LOG(LogTemp, Warning, TEXT("HitReact Animation!! %s"), *this->GetFName().ToString());*/
			AnimInstance->Montage_Play(HitMontage, PlayRate);
			AnimInstance->Montage_JumpToSection(Section, HitMontage);
		}

		bCanHitReact = false;
		const float HitReactTime{ FMath::FRandRange(HitReactTimeMin, HitReactTimeMax)};
		GetWorldTimerManager().SetTimer(
			HitReactTimer,
			this,
			&AEnemy::ResetHitReactTimer,
			HitReactTime);
	}
	
}

void AEnemy::ResetHitReactTimer()
{
	bCanHitReact = true;
}

void AEnemy::StoreHitNumber(UUserWidget* HitNumber, FVector Location)
{
	HitNumbers.Add(HitNumber, Location);

	FTimerHandle HitNumberTimer;
	FTimerDelegate HitNumberDelegate;

	HitNumberDelegate.BindUFunction(this,FName("DestroyHitNumber"),HitNumber);
	GetWorld()->GetTimerManager().SetTimer(
		HitNumberTimer,
		HitNumberDelegate,
		HitNumberDestroyTime,
		false);
}

void AEnemy::DestroyHitNumber(UUserWidget* HitNumber)
{
	HitNumbers.Remove(HitNumber);
	HitNumber->RemoveFromParent();
}

void AEnemy::UpdateHitNumbers()
{
	for (auto& HitPair : HitNumbers)
	{
		UUserWidget* HitNumber = HitPair.Key;
		const FVector Location = HitPair.Value;
		FVector2D ScreenPosition;
		UGameplayStatics::ProjectWorldToScreen(
			GetWorld()->GetFirstPlayerController(),
			Location, 
			ScreenPosition);
		HitNumber->SetPositionInViewport(ScreenPosition);
	}
}

void AEnemy::DetectSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor == nullptr) return;
	AShooterCharacter* Character = Cast<AShooterCharacter>(OtherActor);

	if (Character)
	{
		if (EnemyController)
		{
			if (EnemyController->GetBlackboardComponent())
			{
				EnemyController->GetBlackboardComponent()->SetValueAsObject(TEXT("Target"), Character);
			}
		}
		
	}
}

void AEnemy::DetectSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor == nullptr) return;
	
	EnemyController->GetBlackboardComponent()->SetValueAsObject(TEXT("Target"), nullptr);
	
}

void AEnemy::SetStunned(bool Stunned)
{
	bStunned = Stunned;

	if (EnemyController)
	{
		EnemyController->GetBlackboardComponent()->SetValueAsBool(TEXT("Stunned"), Stunned);
	}
}



void AEnemy::PlayAttackMontage(FName Section, float PlayRate /*= 1.f*/)
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance && AttackMontage)
	{
		AnimInstance->Montage_Play(AttackMontage, PlayRate);
		AnimInstance->Montage_JumpToSection(Section, AttackMontage);
	}
	bCanAttack = false;

	GetWorldTimerManager().SetTimer(
		AttackWaitTimer,
		this,
		&AEnemy::ResetCanAttack,
		AttackWaitTime);
	if (EnemyController)
	{
		EnemyController->GetBlackboardComponent()->SetValueAsBool(
			TEXT("CanAttack"),
			bCanAttack);
	}
}

FName AEnemy::GetAttackSectionName()
{
	FName SectionName;
	const int32 Section = FMath::RandRange(1, 4);
	switch (Section)
	{
	case 1:
		SectionName = TEXT("Attack01");
		break;
	case 2:
		SectionName = TEXT("Attack02");
		break;
	case 3:
		SectionName = TEXT("Attack03");
		break;
	case 4:
		SectionName = TEXT("Attack04");
		break;
	}
	return SectionName;
}

void AEnemy::DoDamage(class AShooterCharacter* Victim)
{

}

void AEnemy::DoDamage(AVictoryPoint* VictoryPoint)
{
}

void AEnemy::SpawnBlood(class AShooterCharacter* Victim, FName SocketName)
{
	const USkeletalMeshSocket* TipSocket = GetMesh()->GetSocketByName(SocketName);
	if (TipSocket)
	{
		const FTransform SocketTransform = TipSocket->GetSocketTransform(GetMesh());
		if (Victim->GetBloodParticles())
		{
			UGameplayStatics::SpawnEmitterAtLocation(
				GetWorld(),
				Victim->GetBloodParticles(),
				SocketTransform);
		}
	}
}

void AEnemy::StunCharacter(AShooterCharacter* Victim)
{
	if (Victim)
	{
		const float Stun = FMath::FRandRange(0.f, 1.f);
		if (Stun <= Victim->GetStunChance())
		{
			Victim->Stun();
		}
	}
}

void AEnemy::ResetCanAttack()
{
	bCanAttack = true;

	if (EnemyController)
	{
		EnemyController->GetBlackboardComponent()->SetValueAsBool(
			TEXT("CanAttack"),
			bCanAttack);
	}
}

void AEnemy::FinishDeath()
{
	GetMesh()->bPauseAnims = true;

	DropItems();

	GetWorldTimerManager().SetTimer(
		Deathtimer,
		this,
		&AEnemy::DestroyEnemy,
		DeathTime);
	
}

void AEnemy::DestroyEnemy()
{
	Destroy();
}

void AEnemy::SetCustomDilation(float Dilation)
{
	CustomTimeDilation = Dilation;
}

void AEnemy::DropItems()
{
	if (bIsDropItems)
	{
		
		float ItemTypes = FMath::FRandRange(0.f, 1.f);
		if (ItemTypes <= 0.1f)
		{
			// HealPack
		}
		else if (ItemTypes <= 0.3f)
		{
			// Weapon
			float WeaponType = FMath::FRandRange(0.f, 1.f);
			EWeaponType SpawnWeaponType;
			EItemRarity SpawnItemRarity;

			if (WeaponType <= 0.5f)
			{
				SpawnWeaponType = EWeaponType::EWT_Pistol;
			}
			else if (WeaponType <= 0.8f)
			{
				SpawnWeaponType = EWeaponType::EWT_SMG;
			}
			else if (WeaponType <= 1.f)
			{
				SpawnWeaponType = EWeaponType::EWT_AR;
			}

			float Rarity = FMath::FRandRange(0.f, 1.f);

			if (Rarity <= 0.2f)
			{
				SpawnItemRarity = EItemRarity::EIR_Damaged;
			}
			else if (Rarity <= 0.6f)
			{
				SpawnItemRarity = EItemRarity::EIR_Common;
			}
			else if (Rarity <= 0.8f)
			{
				SpawnItemRarity = EItemRarity::EIR_Uncommon;
			}
			else if (Rarity <= 0.92f)
			{
				SpawnItemRarity = EItemRarity::EIR_Rare;
			}
			else if (Rarity <= 1.f)
			{
				SpawnItemRarity = EItemRarity::EIR_Legendary;
			}

			DropWeapon.GetDefaultObject()->SetWeaponCondition(SpawnWeaponType, SpawnItemRarity);			
		
			GetWorld()->SpawnActor<AWeapon>(DropWeapon, GetActorLocation(), FRotator(0.f,0.f,0.f));

		}
		else if (ItemTypes <= 0.8f)
		{
			// Ammo
			float AmmoTypes = FMath::FRandRange(0.f, 1.f);

			if (AmmoTypes <= 0.5f)
			{
				GetWorld()->SpawnActor<AAmmo>(*DropAmmo.Find(EAmmoType::EAT_9mm), GetActorLocation(), FRotator(0.f, 0.f, 0.f));
			}
			else if (AmmoTypes <= 1.f)
			{
				GetWorld()->SpawnActor<AAmmo>(*DropAmmo.Find(EAmmoType::EAT_AR), GetActorLocation(), FRotator(0.f, 0.f, 0.f));
			}
			
		}
		else if (ItemTypes <= 1.0f)
		{
			// NoItem
		}
		
	}

}

// Called every frame
void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	UpdateHitNumbers();
}

// Called to bind functionality to input
void AEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

float AEnemy::TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	if (EnemyController)
	{
		EnemyController->GetBlackboardComponent()->SetValueAsObject(
			TEXT("Target"),
			DamageCauser);
	}

	if (Health == 0.f) return 0.f;

	if (Health - Damage <= 0.f)
	{
		Health = 0.f;
		auto Gamemode = Cast<AShooterGameModeBase>(GetWorld()->GetAuthGameMode());

		if (Gamemode)
		{
			Gamemode->AddGameScore(Score);
		}

		GetMesh()->SetCollisionResponseToChannel(
			ECollisionChannel::ECC_GameTraceChannel4,
			ECollisionResponse::ECR_Ignore);
		/*GetCapsuleComponent()->SetCollisionResponseToChannel(
			ECollisionChannel::ECC_Visibility,
			ECollisionResponse::ECR_Ignore);*/
		Die();
	}
	else
	{
		Health -= Damage;
	}

	if (bDying) return Damage;

	ShowHealthBar();

	const float Stunned = FMath::FRandRange(0.f, 1.f);
	if (Stunned <= StunChance)
	{
		/*UE_LOG(LogTemp,Warning,TEXT("Get Stunned!!! HitReact!! %s"),*this->GetFName().ToString());*/
		PlayHitMontage(FName("HitReactFront"));
		SetStunned(true);
	}
	return Damage;
}

void AEnemy::BulletHit_Implementation(FHitResult HitResult, AActor* Shooter, AController* ShooterController)
{
	if (ImpactSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, ImpactSound, GetActorLocation());
	}

	if (ImpactParticles)
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ImpactParticles, HitResult.Location, FRotator(0.f), true);
	}	
}

