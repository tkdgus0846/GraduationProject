// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Enemy.h"
#include "Crunch.generated.h"

/**
 * 
 */
UCLASS()
class SHOOTER_API ACrunch : public AEnemy
{
	GENERATED_BODY()

public:
	ACrunch();

protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	void CombatRangeSphereOverlap(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult);

	UFUNCTION()
	void CombatRangeSphereEndOverlap(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex);

	UFUNCTION()
	void OnLeftWeaponOverlap(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult);

	UFUNCTION()
	void OnRightWeaponOverlap(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult);

	virtual void DoDamage(class AShooterCharacter* Victim) override;
	virtual void DoDamage(class AVictoryPoint* VictoryPoint) override;

	UFUNCTION(BlueprintCallable)
	void ActivateLeftWeapon();
	UFUNCTION(BlueprintCallable)
	void DeactivateLeftWeapon();
	UFUNCTION(BlueprintCallable)
	void ActivateRightWeapon();
	UFUNCTION(BlueprintCallable)
	void DeactivateRightWeapon();

private:
	UPROPERTY(EditAnywhere, Category = "Behavior Tree", Meta = (AllowPrivateAccess = true))
	FVector PatrolPoint;

	UPROPERTY(EditAnywhere, Category = "Behavior Tree", Meta = (AllowPrivateAccess = true))
	FVector PatrolPoint2;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Combat, Meta = (AllowPrivateAccess = true))
	class UBoxComponent* LeftWeaponCollision;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Combat, Meta = (AllowPrivateAccess = true))
	class UBoxComponent* RightWeaponCollision;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat, Meta = (AllowPrivateAccess = true))
	class USphereComponent* CombatRangeSphere;
public:
};
