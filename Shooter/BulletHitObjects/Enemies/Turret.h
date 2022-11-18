// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Enemy.h"
#include "Turret.generated.h"

#define ATTACK_RANGE 10000.f

UCLASS()
class SHOOTER_API ATurret : public AEnemy
{
	GENERATED_BODY()

public:
	ATurret();

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void DoDamage(class AShooterCharacter* Victim);



	UFUNCTION(BlueprintCallable)
	void Attack(FName Section, float PlayRate = 1.f);

	void SendBullet();
	void AdjustAim();
	

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat, Meta = (AllowPrivateAccess = true))
	class UParticleSystem* ShieldParticles;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat, Meta = (AllowPrivateAccess = true))
	class USoundCue* ShieldSound;

	FVector MuzzleSocketLocation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat, Meta = (AllowPrivateAccess = true))
	FRotator AimRotation;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Combat, Meta = (AllowPrivateAccess = true))
	TSubclassOf<class AProjectile> ProjectileClass;

	FTimerHandle AttackTimerHandle;
	
public:

	FRotator GetAimRotation() const {return AimRotation;}
};
