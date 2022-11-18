// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "../Types/WeaponType.h"
#include "ShooterAnimInstance.generated.h"

UENUM(BlueprintType)
enum class EOffsetState : uint8 
{
	EOS_Aiming UMETA(DisplayName = "Aiming"),
	EOS_Hip UMETA(DisplayName = "Hip"),
	EOS_Reloading UMETA(DisplayName = "Reloading"),
	EOS_InAir UMETA(DisplayName = "InAir"),

	EOS_MAX UMETA(DisplayName = "DefaultMAX")
};

/**
 * 
 */
UCLASS()
class SHOOTER_API UShooterAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	UShooterAnimInstance();

	UFUNCTION(BlueprintCallable)
	void UpdateAnimationProperties(float DeltaTime);

	virtual void NativeInitializeAnimation() override;

protected:
	
	void TurnInPlace();

	void Lean(float DeltaTime);

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Movement, Meta = (AllowPrivateAccess = true))
	class AShooterCharacter* ShooterCharacter;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Movement, Meta = (AllowPrivateAccess = true))
	float Speed;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Movement, Meta = (AllowPrivateAccess = true))
	bool bIsInAir;

	// 낙뎀 계산을 위함.
	UPROPERTY()
	bool bFallingMode;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Movement, Meta = (AllowPrivateAccess = true))
	bool bIsAccelerating;

	/* offset yaw for strafing */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Movement, Meta = (AllowPrivateAccess = true))
	float MovementOffset;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Movement, Meta = (AllowPrivateAccess = true))
	float LastMovementOffset;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Movement, Meta = (AllowPrivateAccess = true))
	bool bAiming;

	float TIPCharacterYaw;

	float TIPCharacterYawLastFrame;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Turn In Place", Meta = (AllowPrivateAccess = true))
	float RootYawOffset;

	float RotationCurve;

	float RotationCurveValueLastFrame;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Turn In Place", Meta = (AllowPrivateAccess = true))
	float Pitch;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Turn In Place", Meta = (AllowPrivateAccess = true))
	bool bReloading;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Turn In Place", Meta = (AllowPrivateAccess = true))
	EOffsetState OffsetState;

	FRotator CharacterRotation;
	FRotator CharacterRotationLastFrame;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Lean, Meta = (AllowPrivateAccess = true))
	float YawDelta;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Crouching, Meta = (AllowPrivateAccess = true))
	bool bCrouching;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Crouching, Meta = (AllowPrivateAccess = true))
	bool bEquipping;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Combat, Meta = (AllowPrivateAccess = true))
	float RecoilWeight;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Combat, Meta = (AllowPrivateAccess = true))
	bool bTurningInPlace;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Combat, Meta = (AllowPrivateAccess = true))
	EWeaponType EquippedWeaponType;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Combat, Meta = (AllowPrivateAccess = true))
	bool bShouldUseFABRIK;

public:
	FORCEINLINE bool GetIsInAir() {return bIsInAir;}
};
