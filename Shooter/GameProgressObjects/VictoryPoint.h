// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "VictoryPoint.generated.h"

UCLASS()
class SHOOTER_API AVictoryPoint : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AVictoryPoint();

	UFUNCTION()
	void EnableSignal();

	UFUNCTION()
	void DisableSignal();

	UFUNCTION()
	void PlaySignalSequence();

	UFUNCTION()
	void PlayDestroySignalSequence();

	UFUNCTION(BlueprintImplementableEvent)
	void SetHUDVisibility(bool bState);

	virtual float TakeDamage(
		float Damage,
		struct FDamageEvent const& DamageEvent,
		AController* EventInstigator,
		AActor* DamageCauser) override;

	UFUNCTION()
	void LoadFailedWidget();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void PostInitializeComponents() override;

	UFUNCTION()
	void SphereOverlap(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult);

	UFUNCTION(BlueprintNativeEvent)
	void ShowHealthBar();
	void ShowHealthBar_Implementation();

	UFUNCTION(BlueprintImplementableEvent)
	void HideHealthBar();

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess = true))
	class USphereComponent* Sphere;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess = true))
	class UStaticMeshComponent* Mesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess = true))
	class UParticleSystemComponent* Signal;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess = true))
	bool bCanWin;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess = true))
	class ALevelSequenceActor* SequenceEnabledSignal;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess = true))
	class ALevelSequenceActor* SequenceDestroyedSignal;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat, Meta = (AllowPrivateAccess = true))
	float Health;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat, Meta = (AllowPrivateAccess = true))
	float MaxHealth;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat, Meta = (AllowPrivateAccess = true))
	class USoundCue* MeleeImpactSound;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	FORCEINLINE void SetCanWin(bool bState) {bCanWin = bState;}

};
