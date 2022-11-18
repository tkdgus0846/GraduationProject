// Fill out your copyright notice in the Description page of Project Settings.


#include "CrunchAnimInstance.h"
#include "Enemy.h"

UCrunchAnimInstance::UCrunchAnimInstance()
{

}

void UCrunchAnimInstance::UpdateAnimationProperties(float DeltaTime)
{
	Super::UpdateAnimationProperties(DeltaTime);

	if (Enemy)
	{
		FVector Velocity = Enemy->GetVelocity();
		Velocity.Z = 0.f;
		Speed = Velocity.Size();
	}
	
}


