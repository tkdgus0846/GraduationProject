// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnemyAnimInstance.h"
#include "TurretAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class SHOOTER_API UTurretAnimInstance : public UEnemyAnimInstance
{
	GENERATED_BODY()
public:
	UTurretAnimInstance();

	virtual void UpdateAnimationProperties(float DeltaTime) override;

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation Variable", Meta = (AllowPrivateAccess = true))
	bool bOpenShield;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation Variable", Meta = (AllowPrivateAccess = true))
	FRotator AimRotation;
};
