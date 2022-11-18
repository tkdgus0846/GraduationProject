// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnemyAnimInstance.h"
#include "CrunchAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class SHOOTER_API UCrunchAnimInstance : public UEnemyAnimInstance
{
	GENERATED_BODY()

public:
	UCrunchAnimInstance();

	virtual void UpdateAnimationProperties(float DeltaTime) override;
	
private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Movement, Meta = (AllowPrivateAccess = true))
	float Speed;

	
	
};
