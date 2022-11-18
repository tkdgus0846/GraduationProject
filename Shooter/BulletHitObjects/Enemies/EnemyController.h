// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "EnemyController.generated.h"

/**
 * 
 */
UCLASS()
class SHOOTER_API AEnemyController : public AAIController
{
	GENERATED_BODY()

public:
	AEnemyController();
	virtual void OnPossess(APawn* InPawn) override;

protected:
private:
	UPROPERTY(BlueprintReadWrite, Category = "AI Behavior", Meta = (AllowPrivateAccess = true))
	class UBlackboardComponent* BlackboardComponent;

	UPROPERTY(BlueprintReadWrite, Category = "AI Behavior", Meta = (AllowPrivateAccess = true))
	class UBehaviorTreeComponent* BehaviorTreeComponent;

public:
	FORCEINLINE class UBlackboardComponent* GetBlackboardComponent() const { return BlackboardComponent; }
	FORCEINLINE class UBehaviorTreeComponent* GetBehaviorTreeComponent() const { return BehaviorTreeComponent; }
};
