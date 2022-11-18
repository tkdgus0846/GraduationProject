// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item.h"
#include "../Types/AmmoType.h"
#include "Engine/DataTable.h"
#include "../Types/WeaponType.h"
#include "Weapon.generated.h"

USTRUCT(BlueprintType)
struct FWeaponDataTable : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EAmmoType AmmoType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 WeaponAmmo;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 MagazineCapacity;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class USoundCue* PickupSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USoundCue* EquipSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class USkeletalMesh* ItemMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString ItemName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTexture2D* InventoryIcon;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTexture2D* AmmoIcon;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UMaterialInstance* MaterialInstance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 MaterialIndex;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName ClipBoneName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName ReloadMontageSection;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UAnimInstance> AnimBP;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTexture2D* CrosshairsMiddle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTexture2D* CrosshairsLeft;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTexture2D* CrosshairsRight;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTexture2D* CrosshairsBottom;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTexture2D* CrosshairsTop;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float AutoFireRate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UParticleSystem* MuzzleFlash;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class USoundCue* FireSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName BoneToHide;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bAutomatic;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Damage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float HeadShotDamage;
};

/**
 * 
 */
UCLASS()
class SHOOTER_API AWeapon : public AItem
{
	GENERATED_BODY()
public:
	AWeapon();

	virtual void Tick(float DeltaTime) override;

	void SetWeaponCondition(EWeaponType NewWeaponType, EItemRarity NewRarity);
protected:
	void StopFalling();

	virtual void OnConstruction(const FTransform& Transform) override;

	virtual void BeginPlay() override;

	void FinishMovingSlide();
	void UpdateSlideDisplacement();
private:
	FTimerHandle ThrowWeaponTimer;
	float ThrowWeaponTime;
	bool bFalling;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Properties", Meta = (AllowPrivateAccess = true))
	int32 Ammo;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Properties", Meta = (AllowPrivateAccess = true))
	int32 MagazineCapacity;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Properties", Meta = (AllowPrivateAccess = true))
	EWeaponType WeaponType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Properties", Meta = (AllowPrivateAccess = true))
	EAmmoType AmmoType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Properties", Meta = (AllowPrivateAccess = true))
	FName ReloadMontageSection;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon Properties", Meta = (AllowPrivateAccess = true))
	bool bMovingClip;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Properties", Meta = (AllowPrivateAccess = true))
	FName ClipBoneName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = DataTable, Meta = (AllowPrivateAccess = true))
	UDataTable* WeaponDataTable;

	int32 PreviousMaterialIndex;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = DataTable, Meta = (AllowPrivateAccess = true))
	UTexture2D* CrosshairsMiddle;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = DataTable, Meta = (AllowPrivateAccess = true))
	UTexture2D* CrosshairsLeft;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = DataTable, Meta = (AllowPrivateAccess = true))
	UTexture2D* CrosshairsRight;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = DataTable, Meta = (AllowPrivateAccess = true))
	UTexture2D* CrosshairsBottom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = DataTable, Meta = (AllowPrivateAccess = true))
	UTexture2D* CrosshairsTop;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = DataTable, Meta = (AllowPrivateAccess = true))
	float AutoFireRate;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = DataTable, Meta = (AllowPrivateAccess = true))
	class UParticleSystem* MuzzleFlash;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = DataTable, Meta = (AllowPrivateAccess = true))
	class USoundCue* FireSound;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = DataTable, Meta = (AllowPrivateAccess = true))
	FName BoneToHide;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Pistol, Meta = (AllowPrivateAccess = true))
	float SlideDisplacement;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pistol, Meta = (AllowPrivateAccess = true))
	UCurveFloat* SlideDisplacementCurve;

	FTimerHandle SlideTimer;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pistol, Meta = (AllowPrivateAccess = true))
	float SlideDisplacementTime;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Pistol, Meta = (AllowPrivateAccess = true))
	bool bMovingSlide;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pistol, Meta = (AllowPrivateAccess = true))
	float MaxSlideDisplacement;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pistol, Meta = (AllowPrivateAccess = true))
	float MaxRecoilRotation;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Pistol, Meta = (AllowPrivateAccess = true))
	float RecoilRotation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Properties", Meta = (AllowPrivateAccess = true))
	bool bAutomatic;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Properties", Meta = (AllowPrivateAccess = true))
	float Damage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Properties", Meta = (AllowPrivateAccess = true))
	float HeadShotDamage;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon Properties", Meta = (AllowPrivateAccess = true))
	float BonusDamage;
public:
	void ThrowWeapon();

	FORCEINLINE int32 GetAmmo() const { return Ammo; }
	FORCEINLINE int32 GetMagazineCapacity() const { return MagazineCapacity; }


	void DecrementAmmo();


	FORCEINLINE EWeaponType GetWeaponType() const { return WeaponType; }
	FORCEINLINE EAmmoType GetAmmoType() const { return AmmoType; }
	FORCEINLINE FName GetReloadMontageSection() const { return ReloadMontageSection; }
	FORCEINLINE FName GetClipBoneName() const { return ClipBoneName; }
	FORCEINLINE void SetClipBoneName(FName Name) { ClipBoneName = Name; }
	FORCEINLINE void SetReloadMontageSection(FName Name) { ReloadMontageSection = Name; }
	FORCEINLINE float GetAutoFireRate() const { return AutoFireRate; }
	FORCEINLINE class UParticleSystem* GetMuzzleFlash() const { return MuzzleFlash; }
	FORCEINLINE class USoundCue* GetFireSound() const { return FireSound; }
	FORCEINLINE bool GetAutomatic() const { return bAutomatic; }
	FORCEINLINE float GetDamage() const { return Damage; }
	FORCEINLINE float GetHeadShotDamage() const { return HeadShotDamage; }

	void StartSlideTimer();
	void ReloadAmmo(int32 Amount);

	FORCEINLINE void SetMovingClip(bool Move) { bMovingClip = Move; }

	bool ClipIsFull();
};
