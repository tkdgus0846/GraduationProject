// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BulletHitInterface.h"
#include "Explosive.generated.h"

UCLASS()
class SHOOTER_API AExplosive : public AActor, public IBulletHitInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AExplosive();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Combat, Meta = (AllowPrivateAccess = true))
	class UStaticMeshComponent* ExplosiveMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat, Meta = (AllowPrivateAccess = true))
	class USphereComponent* OverlapSphere;

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat, Meta = (AllowPrivateAccess = true))
	class UParticleSystem* ExplodeParticles;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat, Meta = (AllowPrivateAccess = true))
	class USoundCue* ExplodeSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat, Meta = (AllowPrivateAccess = true))
	float Damage;
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void BulletHit_Implementation(FHitResult HitResult, AActor* Shooter, AController* ShooterController) override;

	void DoExplosion(AActor* Shooter, AController* ShooterController);
};
