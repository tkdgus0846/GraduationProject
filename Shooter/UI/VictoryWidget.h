// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MenuWidget.h"
#include "VictoryWidget.generated.h"

/**
 * 
 */
UCLASS()
class SHOOTER_API UVictoryWidget : public UMenuWidget
{
	GENERATED_BODY()

public:
	virtual bool Initialize() override;
	virtual void Setup() override;
	void SetVictoryMsg(FString Msg);

private:
	UPROPERTY(Meta = (BindWidget))
	class UTextBlock* VictoryMsg;

	UPROPERTY(Meta = (BindWidget))
	class UTextBlock* ScoreText;

	UPROPERTY(Meta = (BindWidget))
	class UButton* ReturnMenuButton;

	/*UFUNCTION()
	void CallMenuButton();*/


};
