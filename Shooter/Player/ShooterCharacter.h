// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "../Types/AmmoType.h"
#include "ShooterCharacter.generated.h"

UENUM(BlueprintType)
enum class ECombatState : uint8
{
	ECS_Unoccupied UMETA(DisplayName = "Unoccupied"),
	ECS_FireTimerInProgress UMETA(DisplayName = "FireTimerInProgress"),
	ECS_Reloading UMETA(DisplayName = "Reloading"),
	ECS_Equipping UMETA(DisplayName = "Equipping"),
	ECS_Stunned UMETA(DisplayName = "Stunned"),

	ECS_MAX UMETA(DisplayName = "DefaultMax")
};

USTRUCT(BlueprintType)
struct FInterpLocation
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class USceneComponent* SceneComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	int32 ItemCount;
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FEquipItemDelegate, int32, CurrentSlotIndex, int32, NewSlotIndex);

UCLASS()
class SHOOTER_API AShooterCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AShooterCharacter();

	virtual float TakeDamage(
		float Damage,
		struct FDamageEvent const& DamageEvent,
		AController* EventInstigator,
		AActor* DamageCauser) override;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void MoveForward(float Value);
	
	void MoveRight(float Value);

	/* 키보드 화살표나 게임패드의 입력을 받을 때 실행되는 함수 */
	void TurnAtRate(float Rate);
	void LookUpAtRate(float Rate);

	/* 마우스의 X,Y값 Value를 기반으로 실행되는 함수 */
	void Turn(float Value);
	void LookUp(float Value);

	/* 총 발사 함수*/
	void FireWeapon();

	bool GetBeamEndLocation(const FVector& MuzzleSocketLocation, FHitResult& OutHitResult);

	void AimingButtonPressed();
	void AimingButtonReleased();

	void CameraInterpZoom(float DeltaTime);

	void SetLookRates();

	void CalculateCrosshairSpread(float DeltaTime);

	void StartCrosshairBulletFire();

	UFUNCTION()
	void FinishCrosshairBulletFire();

	void FireButtonPressed();
	void FireButtonReleased();

	void StartFireTimer();

	UFUNCTION()
	void AutoFireReset();

	bool TraceUnderCrosshairs(FHitResult& OutHitResult, FVector& OutHitLocation);

	/* 아이템의 트레이스, OverlappedItemCount > 0 일경우에 */
	void TraceForItems();

	class AWeapon* SpawnDefaultWeapon();

	void DropWeapon();

	void SelectButtonPressed();
	void SelectButtonReleased();

	void SwapWeapon(AWeapon* WeaponToSwap);

	void InitializeAmmoMap();

	bool WeaponHasAmmo();

	/* 총 발사 관련 함수들 */
	void PlayFireSound();
	void SendBullet();
	void PlayGunFireMontage();

	/* R키 바인드 함수 */
	void ReloadButtonPressed();

	/* 무기의 재장전을 처리 */
	void ReloadWeapon();

	bool CarryingAmmo();

	UFUNCTION(BlueprintCallable)
	void GrabClip();

	UFUNCTION(BlueprintCallable)
	void ReleaseClip();

	void CrouchButtonPressed();

	virtual void Jump() override;

	void InterpCapsuleHalfHeight(float DeltaTime);

	void Aim();
	

	void PickupAmmo(class AAmmo* Ammo);

	void InitializeInterpLocations();

	void FKeyPressed();
	void OneKeyPressed();
	void TwoKeyPressed();
	void ThreeKeyPressed();
	void FourKeyPressed();
	void FiveKeyPressed();

	UFUNCTION(BlueprintCallable)
	EPhysicalSurface GetSurfaceType();

	UFUNCTION(BlueprintCallable)
	void EndStun();

	void Die();

	UFUNCTION(BlueprintCallable)
	void FinishDeath();
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	virtual void PostInitializeComponents() override;

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, Meta = (AllowPrivateAccess = true))
	class USpringArmComponent* CameraBoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, Meta = (AllowPrivateAccess = true))
	class UCameraComponent* FollowCamera;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Meta = (AllowPrivateAccess = true))
	float BaseTurnRate;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Meta = (AllowPrivateAccess = true))
	float BaseLookUpRate;

	/* 조준하고 있지 않을때의 TurnRate */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Meta = (AllowPrivateAccess = true))
	float HipTurnRate;

	/* 조준하고 있지 않을때의 LookUpRate */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Meta = (AllowPrivateAccess = true))
	float HipLookUpRate;

	/* 조준하고 있을때의 TurnRate */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Meta = (AllowPrivateAccess = true))
	float AimingTurnRate;

	/* 조준하고 있을때의 LookUpRate */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Meta = (AllowPrivateAccess = true))
	float AimingLookUpRate;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Meta = (AllowPrivateAccess = true), Meta = (ClampMin = "0.0", ClampMax = "1.0", UIMin = "0.0", UIMax = "1.0"))
	float MouseHipTurnRate;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Meta = (AllowPrivateAccess = true), Meta = (ClampMin = "0.0", ClampMax = "1.0", UIMin = "0.0", UIMax = "1.0"))
	float MouseHipLookUpRate;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Meta = (AllowPrivateAccess = true), Meta = (ClampMin = "0.0", ClampMax = "1.0", UIMin = "0.0", UIMax = "1.0"))
	float MouseAimingTurnRate;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Meta = (AllowPrivateAccess = true), Meta = (ClampMin = "0.0", ClampMax = "1.0", UIMin = "0.0", UIMax = "1.0"))
	float MouseAimingLookUpRate;

	/* 애니 몽타주들 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat, Meta = (AllowPrivateAccess = true))
	class UAnimMontage* HipFireMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat, Meta = (AllowPrivateAccess = true))
	class UAnimMontage* ReloadMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat, Meta = (AllowPrivateAccess = true))
	class UAnimMontage* EquipMontage;

	UFUNCTION(BlueprintCallable)
	void FinishReloading();

	UFUNCTION(BlueprintCallable)
	void FinishEquipping();


	/* 파티클 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat, Meta = (AllowPrivateAccess = true))
	class UParticleSystem* ImpactParticles;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat, Meta = (AllowPrivateAccess = true))
	class UParticleSystem* BeamParticles;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Combat, Meta = (AllowPrivateAccess = true))
	bool bAiming;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat, Meta = (AllowPrivateAccess = true))
	float CameraDefaultFOV;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat, Meta = (AllowPrivateAccess = true))
	float CameraZoomedFOV;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat, Meta = (AllowPrivateAccess = true))
	float CameraCurrentFOV;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat, Meta = (AllowPrivateAccess = true))
	float ZoomInterpSpeed;

	/* Crosshair Components*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Crosshairs, Meta = (AllowPrivateAccess = true))
	float CrosshairSpreadMultiplier;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Crosshairs, Meta = (AllowPrivateAccess = true))
	float CrosshairVelocityFactor;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Crosshairs, Meta = (AllowPrivateAccess = true))
	float CrosshairInAirFactor;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Crosshairs, Meta = (AllowPrivateAccess = true))
	float CrosshairAimFactor;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Crosshairs, Meta = (AllowPrivateAccess = true))
	float CrosshairShootingFactor;

	bool bFireButtonPressed;

	bool bShouldFire;

	FTimerHandle AutoFireTimer;

	float ShootTimeDuration;
	bool bFiringBullet;
	FTimerHandle CrosshairShootTimer;

	bool bShouldTraceForItems;
	int8 OverlappedItemCount;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Items, Meta = (AllowPrivateAccess = true))
	TArray<class AItem*> OverlappedItemList;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Items, Meta = (AllowPrivateAccess = true))
	class AItem* TraceHitItemLastFrame;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Combat, Meta = (AllowPrivateAccess = true))
	class AWeapon* EquippedWeapon;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Combat, Meta = (AllowPrivateAccess = true))
	TSubclassOf<class AWeapon> DefaultWeaponClass;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Combat, Meta = (AllowPrivateAccess = true))
	class AItem* TraceHitItem;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Items, Meta = (AllowPrivateAccess = true))
	float CameraInterpDistance;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Items, Meta = (AllowPrivateAccess = true))
	float CameraInterpElevation;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Items, Meta = (AllowPrivateAccess = true))
	TMap<EAmmoType, int32> AmmoMap;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Items, Meta = (AllowPrivateAccess = true))
	int32 Starting9mmAmmo;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Items, Meta = (AllowPrivateAccess = true))
	int32 StartingARAmmo;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Combat, Meta = (AllowPrivateAccess = true))
	ECombatState CombatState;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat, Meta = (AllowPrivateAccess = true))
	FTransform ClipTransform;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat, Meta = (AllowPrivateAccess = true))
	USceneComponent* HandSceneComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Movement, Meta = (AllowPrivateAccess = true))
	bool bCrouching;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Movement, Meta = (AllowPrivateAccess = true))
	float BaseMovementSpeed;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Movement, Meta = (AllowPrivateAccess = true))
	float CrouchMovementSpeed;

	/* 현재 캡슐의 중간 지점 높이 */
	float CurrentCapsuleHalfHeight;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Movement, Meta = (AllowPrivateAccess = true))
	float StandingCapsuleHalfHeight;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Movement, Meta = (AllowPrivateAccess = true))
	float CrouchingCapsuleHalfHeight;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Movement, Meta = (AllowPrivateAccess = true))
	float BaseGroundFriction;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Movement, Meta = (AllowPrivateAccess = true))
	float CrouchingGroundFriction;

	bool bAimingButtonPressed;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Meta = (AllowPrivateAccess = true))
	USceneComponent* WeaponInterpComp;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Meta = (AllowPrivateAccess = true))
	USceneComponent* InterpComp1;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Meta = (AllowPrivateAccess = true))
	USceneComponent* InterpComp2;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Meta = (AllowPrivateAccess = true))
	USceneComponent* InterpComp3;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Meta = (AllowPrivateAccess = true))
	USceneComponent* InterpComp4;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Meta = (AllowPrivateAccess = true))
	USceneComponent* InterpComp5;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Meta = (AllowPrivateAccess = true))
	USceneComponent* InterpComp6;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Meta = (AllowPrivateAccess = true))
	TArray<FInterpLocation> InterpLocations;

	FTimerHandle PickupSoundTimer;
	FTimerHandle EquipSoundTimer;

	bool bShouldPlayPickupSound;
	bool bShouldPlayEquipSound;

	void ResetPickupSoundTimer();
	void ResetEquipSoundTimer();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Items, Meta = (AllowPrivateAccess = true))
	float PickupSoundResetTime;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Items, Meta = (AllowPrivateAccess = true))
	float EquipSoundResetTime;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Meta = (AllowPrivateAccess = true))
	class UInventory* Inventory;

	UPROPERTY(BlueprintAssignable, Category = Delegates, Meta = (AllowPrivateAccess = true))
	FEquipItemDelegate EquipItemDelegate;	

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat, Meta = (AllowPrivateAccess = true))
	float Health;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat, Meta = (AllowPrivateAccess = true))
	float MaxHealth;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat, Meta = (AllowPrivateAccess = true))
	class USoundCue* MeleeImpactSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat, Meta = (AllowPrivateAccess = true))
	class USoundCue* RangedImpactSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat, Meta = (AllowPrivateAccess = true))
	class USoundCue* LandSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat, Meta = (AllowPrivateAccess = true))
	class UParticleSystem* BloodParticles;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat, Meta = (AllowPrivateAccess = true))
	class UAnimMontage* HitReactMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat, Meta = (AllowPrivateAccess = true))
	float StunChance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat, Meta = (AllowPrivateAccess = true))
	class UAnimMontage* DeathMontage;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Combat, Meta = (AllowPrivateAccess = true))
	bool bDead;

	/* 떨어지기 시작할때 => 1 , 1인 상태에서 수시로 떨어짐을 체크 => 위치 차이로 데미지 입힘*/
	

	UPROPERTY()
	float TopPositionZ;

	UFUNCTION()
	void SetCustomDilation(float Dilation);
	
public:
	void FallingStart();
	void FallingFinish();
	void StopAiming();

	FORCEINLINE USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	FORCEINLINE UCameraComponent* GetFollowCamera() const { return FollowCamera; }
	FORCEINLINE bool GetAiming() const { return bAiming; }

	UFUNCTION(BlueprintCallable)
	float GetCrosshairSpreadMultiplier() const;

	FORCEINLINE int8 GetOverlappedItemCount() const { return OverlappedItemCount; }
	void IncrementOverlappedItemCount(int8 Amount, class AItem* TraceItem);

	//FVector GetCameraInterpLocation();

	void GetPickupItem(AItem* Item);

	void EquipWeapon(class AWeapon* WeaponToEquip, bool bSwapping = false);

	FORCEINLINE class UInventory* GetInventory() {return Inventory;}
	FORCEINLINE ECombatState GetCombatState() const { return CombatState; }
	FORCEINLINE void SetCombatState(ECombatState State) { CombatState = State; }
	FORCEINLINE bool GetCrouching() const { return bCrouching; }
	FORCEINLINE class UAnimMontage* GetEquipMontage() const { return EquipMontage; }
	FInterpLocation GetInterpLocation(int32 Index);

	int32 GetInterpLocationIndex();

	void IncrementInterpLocItemCount(int32 Index, int32 Amount);

	FORCEINLINE bool ShouldPlayPickupSound() const { return bShouldPlayPickupSound; }
	FORCEINLINE bool ShouldPlayEquipSound() const { return bShouldPlayEquipSound; }

	void StartPickupSoundTimer();
	void StartEquipSoundTimer();

	FORCEINLINE AWeapon* GetEquippedWeapon() const { return EquippedWeapon; }
	FORCEINLINE class USoundCue* GetMeleeImpactSound() const { return MeleeImpactSound; }
	FORCEINLINE class USoundCue* GetRangedImpactSound() const { return RangedImpactSound; }
	FORCEINLINE class UParticleSystem* GetBloodParticles() const { return BloodParticles; }

	void Stun();
	FORCEINLINE float GetStunChance() const { return StunChance; }
};
