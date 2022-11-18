// Fill out your copyright notice in the Description page of Project Settings.


#include "Turret.h"
#include "Kismet/GameplayStatics.h"
#include "../../Player/ShooterCharacter.h"
#include "EnemyController.h"
#include "Sound/SoundCue.h"
#include "EnemyController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "DrawDebugHelpers.h"
#include "Engine/SkeletalMeshSocket.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/ArrowComponent.h"
#include "../Projectile.h"

ATurret::ATurret()
{
	PrimaryActorTick.bCanEverTick = true;

	
}

void ATurret::BeginPlay()
{
	Super::BeginPlay();

	
}

void ATurret::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	AdjustAim();
}

void ATurret::DoDamage(AShooterCharacter* Victim)
{
	if (Victim)
	{
		UGameplayStatics::ApplyDamage(
			Victim,
			GetBaseDamage(),
			GetEnemyController(),
			this,
			UDamageType::StaticClass());
		if (Victim->GetRangedImpactSound())
		{
			UGameplayStatics::PlaySoundAtLocation(
				this,
				Victim->GetRangedImpactSound(),
				GetActorLocation());
		}
	}
}

void ATurret::Attack(FName Section, float PlayRate /*= 1.f*/)
{
	PlayAttackMontage(Section,PlayRate);
	SendBullet();
}

void ATurret::SendBullet()
{
	UObject* Target = GetEnemyController()->GetBlackboardComponent()->GetValueAsObject(TEXT("Target"));
	AActor* Character = Cast<AActor>(Target);
	if (Character == nullptr) return;
	if (ProjectileClass)
	{
		MuzzleSocketLocation = GetMesh()->GetSocketLocation(TEXT("Muzzle"));
		/*UE_LOG(LogTemp,Warning,TEXT("%f %f %f"),MuzzleSocketLocation.X, MuzzleSocketLocation.Y, MuzzleSocketLocation.Z);*/
		
		AProjectile* Projectile = GetWorld()->SpawnActor<AProjectile>(ProjectileClass, MuzzleSocketLocation, FRotator(0.f, 0.f, 0.f));
		Projectile->Launch(MuzzleSocketLocation,Character,true);
		
	}
	
	/*FHitResult HitResult;

	MuzzleSocketLocation = GetMesh()->GetSocketLocation(TEXT("Muzzle"));

	const FVector Start = MuzzleSocketLocation;
	FVector End = Character->GetActorLocation() - Start;
	bool bNormalizeSucceed = End.Normalize();

	UE_LOG(LogTemp,Warning,TEXT("%f %f %f"),Start.X,Start.Y,Start.Z);
	if (bNormalizeSucceed == false) return;
	End = End * ATTACK_RANGE;

	FCollisionQueryParams QueryParams;

	bool bBlockingHit = GetWorld()->LineTraceSingleByChannel(
		HitResult,
		Start,
		Start + End,
		ECollisionChannel::ECC_Visibility,
		QueryParams);
	DrawDebugLine(GetWorld(), Start, Start + End, FColor::Green, false, 2.0f, 0, 10.f);*/


	//if (bBlockingHit)
	//{
	//	UE_LOG(LogTemp, Warning, TEXT("Hit Actor: %s Distance: %f"), *HitResult.Actor->GetFName().ToString(), HitResult.Distance);

	//}

	//const USkeletalMeshSocket* BarrelSocket = EquippedWeapon->GetItemMesh()->GetSocketByName("BarrelSocket");
	//if (BarrelSocket)
	//{
	//	const FTransform SocketTransform = BarrelSocket->GetSocketTransform(EquippedWeapon->GetItemMesh());

	//	if (EquippedWeapon->GetMuzzleFlash())
	//	{
	//		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), EquippedWeapon->GetMuzzleFlash(), SocketTransform);
	//	}

	//	FHitResult BeamHitResult;

	//	bool bBeamEnd = GetBeamEndLocation(SocketTransform.GetLocation(), BeamHitResult);

	//	if (!bBeamEnd || !BeamHitResult.Actor.IsValid()) return;

	//	UE_LOG(LogTemp, Warning, TEXT("Hit Actor!! %s"), *BeamHitResult.Actor->GetFName().ToString());
	//	IBulletHitInterface* BulletHitInterface = Cast<IBulletHitInterface>(BeamHitResult.GetActor());

	//	if (BulletHitInterface)
	//	{
	//		BulletHitInterface->BulletHit_Implementation(BeamHitResult, this, GetController());

	//		AEnemy* HitEnemy = Cast<AEnemy>(BeamHitResult.GetActor());
	//		if (HitEnemy)
	//		{
	//			int32 Damage = 0;
	//			bool bHeadShot = false;

	//			if (BeamHitResult.BoneName.ToString() == HitEnemy->GetHeadBone()) // 머리에 맞았을때
	//			{
	//				Damage = EquippedWeapon->GetHeadShotDamage();
	//				bHeadShot = true;
	//				UGameplayStatics::ApplyDamage(
	//					BeamHitResult.GetActor(),
	//					Damage,
	//					GetController(),
	//					this,
	//					UDamageType::StaticClass());
	//			}
	//			else // 몸에 맞았을때
	//			{
	//				Damage = EquippedWeapon->GetDamage();
	//				bHeadShot = false;
	//				UGameplayStatics::ApplyDamage(
	//					BeamHitResult.GetActor(),
	//					Damage,
	//					GetController(),
	//					this,
	//					UDamageType::StaticClass());
	//			}

	//			HitEnemy->ShowHitNumber(Damage, BeamHitResult.Location, bHeadShot);
	//		}
	//	}
	//	else
	//	{
	//		UE_LOG(LogTemp, Warning, TEXT("X Hit Actor!!"));

	//		if (ImpactParticles)
	//		{

	//			UGameplayStatics::SpawnEmitterAtLocation(
	//				GetWorld(),
	//				ImpactParticles,
	//				BeamHitResult.Location);
	//		}
	//	}



	//	if (BeamParticles)
	//	{
	//		UParticleSystemComponent* Beam = UGameplayStatics::SpawnEmitterAtLocation(
	//			GetWorld(),
	//			BeamParticles,
	//			SocketTransform.GetLocation());
	//		if (Beam)
	//		{
	//			Beam->SetVectorParameter("Target", BeamHitResult.Location);
	//		}
	//	}
	//}
}

void ATurret::AdjustAim()
{
	/*UObject* Target = GetEnemyController()->GetBlackboardComponent()->GetValueAsObject(TEXT("Target"));
	AActor* Character = Cast<AActor>(Target);

	if (Character)
	{
		const FVector Start = GetArrowComponent()->GetComponentLocation();
		FVector End = Start - Character->GetActorLocation();
		const bool bNormalizeSucceed = End.Normalize();

		if (bNormalizeSucceed)
		{
			FRotator Rotation = End.Rotation();
			Rotation.Pitch = Rotation.Pitch * -1;

			AimRotation = Rotation;
		}

	}*/
}
