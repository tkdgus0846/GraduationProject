// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "ShooterPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class SHOOTER_API AShooterPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	AShooterPlayerController();

protected:

	virtual void BeginPlay() override;
private:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Widgets, Meta = (AllowPrivateAccess = true))
	TSubclassOf<class UUserWidget> HUDOverlayClass;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Widgets, Meta = (AllowPrivateAccess = true))
	class UUserWidget* HUDOverlay;

public:
	void SetHUDOverlayVisibility(bool bState);
};
