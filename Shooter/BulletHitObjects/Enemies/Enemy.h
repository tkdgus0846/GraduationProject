// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "../BulletHitInterface.h"
#include "Shooter/Types/AmmoType.h"
#include "Enemy.generated.h"

UCLASS()
class SHOOTER_API AEnemy : public ACharacter, public IBulletHitInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AEnemy();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintNativeEvent)
	void ShowHealthBar();
	void ShowHealthBar_Implementation();

	UFUNCTION(BlueprintImplementableEvent)
	void HideHealthBar();

	void Die();

	void PlayHitMontage(FName Section, float PlayRate = 1.f);

	void ResetHitReactTimer();

	UFUNCTION(BlueprintCallable)
	void StoreHitNumber(UUserWidget* HitNumber, FVector Location);

	UFUNCTION()
	void DestroyHitNumber(UUserWidget* HitNumber);

	void UpdateHitNumbers();

	UFUNCTION()
	void DetectSphereOverlap(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult);

	UFUNCTION()
	void DetectSphereEndOverlap(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex);

	UFUNCTION(BlueprintCallable)
	void SetStunned(bool Stunned);	

	UFUNCTION(BlueprintCallable)
	void PlayAttackMontage(FName Section, float PlayRate = 1.f);

	UFUNCTION(BlueprintPure)
	FName GetAttackSectionName();	

	virtual void DoDamage(class AShooterCharacter* Victim);
	virtual void DoDamage(class AVictoryPoint* VictoryPoint);

	void SpawnBlood(class AShooterCharacter* Victim, FName SocketName);

	void StunCharacter(class AShooterCharacter* Victim);

	void ResetCanAttack();

	UFUNCTION(BlueprintCallable)
	void FinishDeath();

	UFUNCTION()
	void DestroyEnemy();

private:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat, Meta = (AllowPrivateAccess = true))
	class UParticleSystem* ImpactParticles;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat, Meta = (AllowPrivateAccess = true))
	class USoundCue* ImpactSound;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Combat, Meta = (AllowPrivateAccess = true))
	float Health;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat, Meta = (AllowPrivateAccess = true))
	float MaxHealth;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat, Meta = (AllowPrivateAccess = true))
	FString HeadBone;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat, Meta = (AllowPrivateAccess = true))
	float HealthBarDisplayTime;

	FTimerHandle HealthBarTimer;

	/* 피격, 죽음 애니메이션 몽타주*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat, Meta = (AllowPrivateAccess = true))
	UAnimMontage* HitMontage;

	FTimerHandle HitReactTimer;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat, Meta = (AllowPrivateAccess = true))
	float HitReactTimeMin;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat, Meta = (AllowPrivateAccess = true))
	float HitReactTimeMax;

	bool bCanHitReact;

	UPROPERTY(VisibleAnywhere, Category = Combat, Meta = (AllowPrivateAccess = true))
	TMap<UUserWidget*, FVector> HitNumbers;

	UPROPERTY(EditAnywhere, Category = Combat, Meta = (AllowPrivateAccess = true))
	float HitNumberDestroyTime;

	UPROPERTY(EditAnywhere, Category = "Behavior Tree", Meta = (AllowPrivateAccess = true))
	class UBehaviorTree* BehaviorTree;

	class AEnemyController* EnemyController;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat, Meta = (AllowPrivateAccess = true))
	class USphereComponent* DetectSphere;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Combat, Meta = (AllowPrivateAccess = true))
	bool bStunned;

	/* 0.0f = 0% , 1.0f = 100% */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat, Meta = (AllowPrivateAccess = true))
	float StunChance;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Combat, Meta = (AllowPrivateAccess = true))
	bool bInAttackRange;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat, Meta = (AllowPrivateAccess = true))
	UAnimMontage* AttackMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat, Meta = (AllowPrivateAccess = true))
	float BaseDamage;

	UPROPERTY(VisibleAnywhere, Category = Combat, Meta = (AllowPrivateAccess = true))
	bool bCanAttack;

	FTimerHandle AttackWaitTimer;

	UPROPERTY(EditAnywhere, Category = Combat, Meta = (AllowPrivateAccess = true))
	float AttackWaitTime;

	bool bDying;

	FTimerHandle Deathtimer;

	UPROPERTY(EditAnywhere, Category = Combat, Meta = (AllowPrivateAccess = true))
	float DeathTime;

	UPROPERTY(EditAnywhere, Category = Combat, Meta = (AllowPrivateAccess = true))
	int32 Score;
	
	UPROPERTY(EditAnywhere, Category = Combat, Meta = (AllowPrivateAccess = true))
	bool bIsDropItems;

	// 드랍 아이템 목록
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Combat, Meta = (AllowPrivateAccess = true))
	TSubclassOf<class AWeapon> DropWeapon;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Combat, Meta = (AllowPrivateAccess = true))
	TMap<EAmmoType,TSubclassOf<class AAmmo>> DropAmmo;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Combat, Meta = (AllowPrivateAccess = true))
	TSubclassOf<class AItem> DropHealPack;

	// 액터 커스텀 지연
	UFUNCTION()
	void SetCustomDilation(float Dilation);

	void DropItems();
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void BulletHit_Implementation(FHitResult HitResult, AActor* Shooter, AController* ShooterController) override;

	virtual float TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

	FORCEINLINE FString GetHeadBone() const { return HeadBone; }

	UFUNCTION(BlueprintImplementableEvent)
	void ShowHitNumber(int32 Damage, FVector HitLocation, bool bHeadShot);

	FORCEINLINE class UBehaviorTree* GetBehaviorTree() const { return BehaviorTree; }
	FORCEINLINE class AEnemyController* GetEnemyController() const { return EnemyController; }
	FORCEINLINE void SetInAttackRange(bool bState) { bInAttackRange = bState; }
	FORCEINLINE bool GetInAttackRange() const { return bInAttackRange; }
	FORCEINLINE float GetBaseDamage() const { return BaseDamage; }
	FORCEINLINE int32 GetEnemyScore() const { return Score; }
};
