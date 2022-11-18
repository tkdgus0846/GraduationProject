// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Explosive.h"
#include "Projectile.generated.h"

UCLASS()
class SHOOTER_API AProjectile : public AExplosive
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AProjectile();

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	void Launch(FVector StartLocation, AActor* NewTarget, bool bFollow = false);
	void Move(float DeltaTime);

protected:
	// Called when the game starts or when spawned
	UFUNCTION()
	void CollisionSphereOverlap(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult);

	UFUNCTION()
	void CollisionSphereEndOverlap(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex);

private:

	UPROPERTY(EditAnywhere, Meta = (AllowPrivateAccess = true))
	class USphereComponent* CollisionComponent;

	UPROPERTY(VisibleAnywhere, Category = "Movement")
	class UProjectileMovementComponent* ProjectileMovementComponent;

	AActor* Target;
	bool bFollowTarget;
	bool bAccelerated;
	
	FVector TargetInitialLocation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess = true))
	float ProjectileSpeed;
public:
	FORCEINLINE void SetTarget(AActor* NewTarget) { Target = NewTarget; }
	FORCEINLINE void SetFollow(bool bState) { bFollowTarget = bState; }
};
