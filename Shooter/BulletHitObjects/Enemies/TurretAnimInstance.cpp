// Fill out your copyright notice in the Description page of Project Settings.


#include "TurretAnimInstance.h"
#include "Turret.h"
#include "Components/ArrowComponent.h"

UTurretAnimInstance::UTurretAnimInstance()
{

}

void UTurretAnimInstance::UpdateAnimationProperties(float DeltaTime)
{
	Super::UpdateAnimationProperties(DeltaTime);

	if (Enemy)
	{
		ATurret* Turret = Cast<ATurret>(Enemy);

		if (Turret)
		{
			AimRotation = Turret->GetAimRotation();
		}
		
	}
}
