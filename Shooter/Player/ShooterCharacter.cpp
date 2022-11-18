// Fill out your copyright notice in the Description page of Project Settings.


#include "ShooterCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"
#include "Engine/SkeletalMeshSocket.h"
#include "DrawDebugHelpers.h"
#include "Particles/ParticleSystemComponent.h"
#include "../Items/Item.h"
#include "../Items/Weapon.h"
#include "Components/WidgetComponent.h"
#include "Components/SphereComponent.h"
#include "Components/BoxComponent.h"
#include "Components/CapsuleComponent.h"
#include "../Items/Ammo.h"
#include "PlayerComponents/Inventory.h"
#include "PhysicalMaterials/PhysicalMaterial.h"
#include "../Shooter.h"
#include "../BulletHitObjects/BulletHitInterface.h"
#include "../BulletHitObjects/Enemies/Enemy.h"
#include "../BulletHitObjects/Enemies/EnemyController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "ShooterAnimInstance.h"
#include "Shooter/Shooter_GameInstance.h"
#include "Shooter/ShooterGameModeBase.h"

// Sets default values
AShooterCharacter::AShooterCharacter() :
	BaseTurnRate(45.f), 
	BaseLookUpRate(45.f), 
	// 조준/비조준 시의 TurnRate
	HipTurnRate(90.f),
	HipLookUpRate(90.f),
	AimingTurnRate(20.f),
	AimingLookUpRate(20.f),
	// 마우스의 민감도 스케일 요소들
	MouseHipTurnRate(1.f),
	MouseHipLookUpRate(1.f),
	MouseAimingTurnRate(0.6f),
	MouseAimingLookUpRate(0.6f),
	bAiming(false),
	// 카메라 FOV 값들
	CameraDefaultFOV(0.f),
	CameraZoomedFOV(25.f),
	CameraCurrentFOV(0.f),
	ZoomInterpSpeed(20.f),
	// 크로스헤어 조정 값들
	CrosshairSpreadMultiplier(0.f),
	CrosshairVelocityFactor(0.f),
	CrosshairInAirFactor(0.f),
	CrosshairAimFactor(0.f),
	CrosshairShootingFactor(0.f),
	// 총 발사 크로스헤어 타이머 관련 변수들
	ShootTimeDuration(0.05f),
	bFiringBullet(false),
	// 자동 총발사 관련 변수들
	bShouldFire(true),
	bFireButtonPressed(false),
	// 아이템 트레이스 변수
	bShouldTraceForItems(false),
	OverlappedItemCount(0),
	// 카메라 보간 변수들
	CameraInterpDistance(250.f),
	CameraInterpElevation(65.f),
	// 시작 총알 수
	Starting9mmAmmo(85),
	StartingARAmmo(120),
	// 전투에 관한 변수
	CombatState(ECombatState::ECS_Unoccupied),
	bCrouching(false),
	BaseMovementSpeed(650.f),
	CrouchMovementSpeed(300.f),
	StandingCapsuleHalfHeight(88.f),
	CrouchingCapsuleHalfHeight(44.f),
	BaseGroundFriction(2.f),
	CrouchingGroundFriction(100.f),
	bAimingButtonPressed(false),
	bShouldPlayPickupSound(true),
	bShouldPlayEquipSound(true),
	PickupSoundResetTime(0.2f),
	EquipSoundResetTime(0.2f),
	Health(100.f),
	MaxHealth(100.f),
	StunChance(0.25f),
	bDead(false)
{
	PrimaryActorTick.bCanEverTick = true;

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 180.f;
	CameraBoom->bUsePawnControlRotation = true;
	CameraBoom->SocketOffset = FVector(0.f, 50.f, 45.f);

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;

	// 컨트롤러에 회전입력을 받을때 캐릭터가 회전하지 않는다.
	// 카메라만 회전하게한다.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = true;
	bUseControllerRotationRoll = false;

	// 캐릭터 무브먼트 설정
	GetCharacterMovement()->bOrientRotationToMovement = false;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 540.f, 0.f);
	GetCharacterMovement()->JumpZVelocity = 600.f;
	GetCharacterMovement()->AirControl = 0.2f;

	// 
	HandSceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("HandSceneComponent"));

	// 보간 컴포넌트들을 만드는 부분
	WeaponInterpComp = CreateDefaultSubobject<USceneComponent>(TEXT("WeaponInterpComp"));
	WeaponInterpComp->SetupAttachment(GetFollowCamera());

	InterpComp1 = CreateDefaultSubobject<USceneComponent>(TEXT("InterpComp1"));
	InterpComp1->SetupAttachment(GetFollowCamera());

	InterpComp2 = CreateDefaultSubobject<USceneComponent>(TEXT("InterpComp2"));
	InterpComp2->SetupAttachment(GetFollowCamera());

	InterpComp3 = CreateDefaultSubobject<USceneComponent>(TEXT("InterpComp3"));
	InterpComp3->SetupAttachment(GetFollowCamera());

	InterpComp4 = CreateDefaultSubobject<USceneComponent>(TEXT("InterpComp4"));
	InterpComp4->SetupAttachment(GetFollowCamera());

	InterpComp5 = CreateDefaultSubobject<USceneComponent>(TEXT("InterpComp5"));
	InterpComp5->SetupAttachment(GetFollowCamera());

	InterpComp6 = CreateDefaultSubobject<USceneComponent>(TEXT("InterpComp6"));
	InterpComp6->SetupAttachment(GetFollowCamera());

	/* 인벤토리 생성 */
	Inventory = CreateDefaultSubobject<UInventory>(TEXT("Inventory"));
}

float AShooterCharacter::TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	
	if (Health - Damage <= 0.f)
	{
		Health = 0.f;

		APlayerController* PC = UGameplayStatics::GetPlayerController(this, 0);
		if (PC)
		{
			DisableInput(PC);
		}

		GetWorldTimerManager().SetTimerForNextTick(this, &AShooterCharacter::Die);

		auto EnemyController = Cast<AEnemyController>(EventInstigator);
		if (EnemyController)
		{
			EnemyController->GetBlackboardComponent()->SetValueAsBool(
				TEXT("CharacterDead"),
				true);
		}
	}
	else
	{
		Health -= Damage;
	}
	return Damage;
}

// Called when the game starts or when spawned
void AShooterCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	if (FollowCamera)
	{
		CameraDefaultFOV = GetFollowCamera()->FieldOfView;
		CameraCurrentFOV = CameraDefaultFOV;
	}

	
	EquipWeapon(SpawnDefaultWeapon());

	if (Inventory)
		Inventory->AddItem(EquippedWeapon);

	EquippedWeapon->SetSlotIndex(1);
	EquippedWeapon->DisableCustomDepth();
	EquippedWeapon->DisableGlowMaterial();
	EquippedWeapon->SetCharacter(this);

	InitializeAmmoMap();

	GetCharacterMovement()->MaxWalkSpeed = BaseMovementSpeed;

	InitializeInterpLocations();

	auto Mode = Cast<AShooterGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));
	if (Mode)
	{
		Mode->ActorDilationDelegate.AddUFunction(this, FName("SetCustomDilation"));
	}
}

void AShooterCharacter::MoveForward(float Value)
{
	if ((Controller != nullptr) && (Value != 0.0f))
	{
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation = FRotator(0, Rotation.Yaw, 0);

		const FVector Direction{FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X)};
		AddMovementInput(Direction, Value);
	}
	
}

void AShooterCharacter::MoveRight(float Value)
{
	if ((Controller != nullptr) && (Value != 0.0f))
	{
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation = FRotator(0, Rotation.Yaw, 0);

		const FVector Direction{ FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y) };
		AddMovementInput(Direction, Value);
	}
}

void AShooterCharacter::TurnAtRate(float Rate)
{
	AddControllerYawInput(BaseTurnRate * Rate * GetWorld()->GetDeltaSeconds());
}

void AShooterCharacter::LookUpAtRate(float Rate)
{
	AddControllerPitchInput(BaseLookUpRate * Rate * GetWorld()->GetDeltaSeconds());
}

void AShooterCharacter::Turn(float Value)
{
	float TurnScaleFactor;
	if (bAiming)
	{
		TurnScaleFactor = MouseAimingTurnRate;
	}
	else
	{
		TurnScaleFactor = MouseHipTurnRate;
	}
	AddControllerYawInput(Value * TurnScaleFactor);
}

void AShooterCharacter::LookUp(float Value)
{
	float LookUpScaleFactor;
	if (bAiming)
	{
		LookUpScaleFactor = MouseAimingLookUpRate;
	}
	else
	{
		LookUpScaleFactor = MouseHipLookUpRate;
	}
	AddControllerPitchInput(Value * LookUpScaleFactor);
}

void AShooterCharacter::FireWeapon()
{
	if (EquippedWeapon == nullptr) return;
	if (CombatState != ECombatState::ECS_Unoccupied) return;

	if (WeaponHasAmmo())
	{
		PlayFireSound();
		SendBullet();
		PlayGunFireMontage();
		EquippedWeapon->DecrementAmmo();
		StartCrosshairBulletFire();

		StartFireTimer();

		if (EquippedWeapon->GetWeaponType() == EWeaponType::EWT_Pistol)
		{
			// Start moving slide timer
			EquippedWeapon->StartSlideTimer();
		}
	}
}

bool AShooterCharacter::GetBeamEndLocation(
	const FVector& MuzzleSocketLocation,
	FHitResult& OutHitResult)
{
	FVector OutBeamLocation;
	FHitResult CrosshairHitResult;
	bool bCrosshairHit = TraceUnderCrosshairs(CrosshairHitResult, OutBeamLocation);

	if (bCrosshairHit)
	{
		OutBeamLocation = CrosshairHitResult.Location;
	}
	else
	{

	}

	const FVector WeaponTraceStart{ MuzzleSocketLocation };
	const FVector StartToEnd{ OutBeamLocation - MuzzleSocketLocation };
	const FVector WeaponTraceEnd{ MuzzleSocketLocation + StartToEnd * 1.25f};
	GetWorld()->LineTraceSingleByChannel(
		OutHitResult,
		WeaponTraceStart,
		WeaponTraceEnd,
		ECollisionChannel::ECC_GameTraceChannel4);

	if (!OutHitResult.bBlockingHit)
	{
		OutHitResult.Location = OutBeamLocation;
		return false;
	}
	return true;

	
}

void AShooterCharacter::AimingButtonPressed()
{
	bAimingButtonPressed = true;
	if (CombatState != ECombatState::ECS_Reloading && CombatState != ECombatState::ECS_Equipping && CombatState != ECombatState::ECS_Stunned)
	{
		Aim();
	}
}

void AShooterCharacter::AimingButtonReleased()
{
	bAimingButtonPressed = false;
	StopAiming();
}

void AShooterCharacter::CameraInterpZoom(float DeltaTime)
{
	if (bAiming)
	{
		if (CameraCurrentFOV == CameraZoomedFOV) return;
		CameraCurrentFOV = FMath::FInterpTo(
			CameraCurrentFOV,
			CameraZoomedFOV,
			DeltaTime,
			ZoomInterpSpeed);
	}
	else
	{
		if (CameraCurrentFOV == CameraDefaultFOV) return;
		CameraCurrentFOV = FMath::FInterpTo(
			CameraCurrentFOV,
			CameraDefaultFOV,
			DeltaTime,
			ZoomInterpSpeed);
	}
	GetFollowCamera()->SetFieldOfView(CameraCurrentFOV);
}

void AShooterCharacter::SetLookRates()
{
	if (bAiming)
	{
		BaseTurnRate = AimingTurnRate;
		BaseLookUpRate = AimingLookUpRate;
	}
	else
	{
		BaseTurnRate = HipTurnRate;
		BaseLookUpRate = HipLookUpRate;
	}
}

void AShooterCharacter::CalculateCrosshairSpread(float DeltaTime)
{
	FVector2D WalkSpeedRange{ 0.f, 600.f };
	FVector2D VelocityMultiplierRange{ 0.f, 1.f };
	FVector Velocity{ GetVelocity() };
	Velocity.Z = 0.f;

	// 0~600의 값을 0~1의 값으로 매핑한다. (걷는 속도)
	CrosshairVelocityFactor = FMath::GetMappedRangeValueClamped(
		WalkSpeedRange,
		VelocityMultiplierRange,
		Velocity.Size());
	
	// 공중에 떠있을때 크로스헤어
	if (GetCharacterMovement()->IsFalling())
	{
		// 공중에 떠있을때 크로스헤어가 천천히 멀어지게
		CrosshairInAirFactor = FMath::FInterpTo(
			CrosshairInAirFactor,
			2.25f,
			DeltaTime,
			2.25f);
	}
	else
	{
		// 바닥에 착지하면 빠르게 가까워짐
		CrosshairInAirFactor = FMath::FInterpTo(
			CrosshairInAirFactor,
			0.f,
			DeltaTime,
			30.f);
	}

	// 조준하고 있을때 크로스헤어 좁게
	if (GetAiming())
	{
		CrosshairAimFactor = FMath::FInterpTo(
			CrosshairAimFactor,
			0.5f,
			DeltaTime,
			30.f);
	}
	else
	{
		CrosshairAimFactor = FMath::FInterpTo(
			CrosshairAimFactor,
			0.f,
			DeltaTime,
			30.f);
	}

	// 총쏘고 0.05초 (ShootTimeDuration) 동안 true
	if (bFiringBullet)
	{
		CrosshairShootingFactor = FMath::FInterpTo(
			CrosshairShootingFactor,
			0.3f,
			DeltaTime,
			60.f);
	}
	else
	{
		CrosshairShootingFactor = FMath::FInterpTo(
			CrosshairShootingFactor,
			0.f,
			DeltaTime,
			60.f);
	}
	CrosshairSpreadMultiplier = 
		0.5f +
		CrosshairVelocityFactor +
		CrosshairInAirFactor -
		CrosshairAimFactor +
		CrosshairShootingFactor;
}

void AShooterCharacter::StartCrosshairBulletFire()
{
	bFiringBullet = true;
	GetWorldTimerManager().SetTimer(
		CrosshairShootTimer, 
		this, 
		&AShooterCharacter::FinishCrosshairBulletFire,
		ShootTimeDuration);
}

void AShooterCharacter::FinishCrosshairBulletFire()
{
	bFiringBullet = false;

}

void AShooterCharacter::FireButtonPressed()
{
	bFireButtonPressed = true;
	FireWeapon();
	
}

void AShooterCharacter::FireButtonReleased()
{
	bFireButtonPressed = false;
}

void AShooterCharacter::StartFireTimer()
{
	if (EquippedWeapon == nullptr) return;
	CombatState = ECombatState::ECS_FireTimerInProgress;

	GetWorldTimerManager().SetTimer(
		AutoFireTimer,
		this,
		&AShooterCharacter::AutoFireReset,
		EquippedWeapon->GetAutoFireRate());
}

void AShooterCharacter::AutoFireReset()
{
	if (CombatState == ECombatState::ECS_Stunned) return;
	CombatState = ECombatState::ECS_Unoccupied;

	if (bDead == true) return;

	if (WeaponHasAmmo())
	{
		if (bFireButtonPressed && EquippedWeapon->GetAutomatic())
		{
			FireWeapon();
		}
	}
	else
	{
		// 총알 재장전
		ReloadWeapon();
	}
	
}

bool AShooterCharacter::TraceUnderCrosshairs(FHitResult& OutHitResult, FVector& OutHitLocation)
{
	FVector2D ViewportSize;
	if (GEngine && GEngine->GameViewport)
	{
		GEngine->GameViewport->GetViewportSize(ViewportSize);
	}

	FVector2D CrosshairLocation(ViewportSize.X / 2.f, ViewportSize.Y / 2.f);
	FVector CrosshairWorldPosition;
	FVector CrosshairWorldDirection;

	bool bScreenToWorld = UGameplayStatics::DeprojectScreenToWorld(
		UGameplayStatics::GetPlayerController(this, 0),
		CrosshairLocation,
		CrosshairWorldPosition,
		CrosshairWorldDirection);

	if (bScreenToWorld)
	{
		const FVector Start { CrosshairWorldPosition };
		const FVector End{ Start + CrosshairWorldDirection * 50'000.f };
		OutHitLocation = End;
		GetWorld()->LineTraceSingleByChannel(
			OutHitResult,
			Start,
			End,
			ECollisionChannel::ECC_Visibility);

		if (OutHitResult.bBlockingHit)
		{
			OutHitLocation = OutHitResult.Location;
			return true;
		}
			
	}
	return false;
}

void AShooterCharacter::TraceForItems()
{
	if (bShouldTraceForItems)
	{
		FHitResult ItemTraceResult;
		FVector HitLocation;
		if (TraceUnderCrosshairs(ItemTraceResult, HitLocation))
		{
			TraceHitItem = Cast<AItem>(ItemTraceResult.GetActor());
			const AWeapon* TraceHitWeapon = Cast<AWeapon>(TraceHitItem);
			if (TraceHitItem)
			{
				if (Inventory->GetHighlightedSlot() == -1)
				{
					//
					Inventory->HighlightInventorySlot();
				}
			}
			else
			{
				if (Inventory->GetHighlightedSlot() != -1)
				{
					Inventory->UnHighlightInventorySlot();
				}
			}

			if (TraceHitItem && TraceHitItem->GetItemState() == EItemState::EIS_EquipInterping)
			{
				TraceHitItem = nullptr;
			}

			if (TraceHitItem && TraceHitItem->GetPickupWidget() && OverlappedItemList.Find(TraceHitItem) != INDEX_NONE)
			{
				TraceHitItem->GetPickupWidget()->SetVisibility(true);
				TraceHitItem->EnableCustomDepth();

				if (Inventory->GetNum() >= INVENTORY_CAPACITY)
				{
					TraceHitItem->SetCharacterInventoryFull(true);
				}
				else
				{
					TraceHitItem->SetCharacterInventoryFull(false);
				}
			}

			if (TraceHitItemLastFrame)
			{
				if (TraceHitItem != TraceHitItemLastFrame)
				{
					TraceHitItemLastFrame->GetPickupWidget()->SetVisibility(false);
					TraceHitItemLastFrame->DisableCustomDepth();
				}
			}

			TraceHitItemLastFrame = TraceHitItem;
		}
	}
	else if (TraceHitItemLastFrame)
	{
		TraceHitItemLastFrame->GetPickupWidget()->SetVisibility(false);
	}
}

AWeapon* AShooterCharacter::SpawnDefaultWeapon()
{
	if (DefaultWeaponClass)
	{
		DefaultWeaponClass.GetDefaultObject()->SetWeaponCondition(EWeaponType::EWT_Pistol, EItemRarity::EIR_Common);
		return GetWorld()->SpawnActor<AWeapon>(DefaultWeaponClass);
	}

	return nullptr;
}

void AShooterCharacter::EquipWeapon(AWeapon* WeaponToEquip, bool bSwapping)
{
	if (WeaponToEquip)
	{
		const USkeletalMeshSocket* HandSocket = GetMesh()->GetSocketByName(FName("RightHandSocket"));

		if (HandSocket)
		{
			HandSocket->AttachActor(WeaponToEquip, GetMesh());
		}

		if (EquippedWeapon == nullptr)
		{
			// -1 == no EquippedWEapon yet
			EquipItemDelegate.Broadcast(-1,WeaponToEquip->GetSlotIndex());
		}
		else if (!bSwapping)
		{
			EquipItemDelegate.Broadcast(EquippedWeapon->GetSlotIndex(), WeaponToEquip->GetSlotIndex());
		}

		EquippedWeapon = WeaponToEquip;
		EquippedWeapon->SetItemState(EItemState::EIS_Equipped);
	}
}

void AShooterCharacter::DropWeapon()
{
	if (EquippedWeapon)
	{
		FDetachmentTransformRules DetachmentTransformRules(EDetachmentRule::KeepWorld, true);
		EquippedWeapon->GetItemMesh()->DetachFromComponent(DetachmentTransformRules);

		EquippedWeapon->SetItemState(EItemState::EIS_Falling);
		EquippedWeapon->ThrowWeapon();
	}
}

void AShooterCharacter::SelectButtonPressed()
{
	
		if (CombatState != ECombatState::ECS_Unoccupied) return;
		if (TraceHitItem)
		{
			TraceHitItem->StartItemCurve(this, true);
			TraceHitItem = nullptr;
		}
	
	
}

void AShooterCharacter::SelectButtonReleased()
{

}

void AShooterCharacter::SwapWeapon(AWeapon* WeaponToSwap)
{

	if (Inventory->GetNum() - 1 >= EquippedWeapon->GetSlotIndex())
	{
		Inventory->SetItem(EquippedWeapon->GetSlotIndex(), WeaponToSwap);
		WeaponToSwap->SetSlotIndex(EquippedWeapon->GetSlotIndex());
	}
	DropWeapon();
	EquipWeapon(WeaponToSwap, true);
	TraceHitItem = nullptr;
	TraceHitItemLastFrame = nullptr;
}

void AShooterCharacter::InitializeAmmoMap()
{
	AmmoMap.Add(EAmmoType::EAT_9mm, Starting9mmAmmo);
	AmmoMap.Add(EAmmoType::EAT_AR, StartingARAmmo);

}

bool AShooterCharacter::WeaponHasAmmo()
{
	if (EquippedWeapon == nullptr) return false;

	return EquippedWeapon->GetAmmo() > 0;
}

void AShooterCharacter::PlayFireSound()
{
	// 총 소리 실행
	if (EquippedWeapon->GetFireSound())
	{
		UGameplayStatics::PlaySound2D(this, EquippedWeapon->GetFireSound());
	}
}

void AShooterCharacter::SendBullet()
{
	// 총알을 발사하는 부분
	const USkeletalMeshSocket* BarrelSocket = EquippedWeapon->GetItemMesh()->GetSocketByName("BarrelSocket");
	if (BarrelSocket)
	{
		const FTransform SocketTransform = BarrelSocket->GetSocketTransform(EquippedWeapon->GetItemMesh());

		if (EquippedWeapon->GetMuzzleFlash())
		{
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), EquippedWeapon->GetMuzzleFlash(), SocketTransform);
		}

		FHitResult BeamHitResult;

		bool bBeamEnd = GetBeamEndLocation(SocketTransform.GetLocation(), BeamHitResult);

		if (!bBeamEnd || !BeamHitResult.Actor.IsValid()) return;

		UE_LOG(LogTemp,Warning,TEXT("Hit Actor!! %s"),*BeamHitResult.Actor->GetFName().ToString());
		IBulletHitInterface* BulletHitInterface = Cast<IBulletHitInterface>(BeamHitResult.GetActor());

		if (BulletHitInterface)
		{
			BulletHitInterface->BulletHit_Implementation(BeamHitResult, this, GetController());

			AEnemy* HitEnemy = Cast<AEnemy>(BeamHitResult.GetActor());
			if (HitEnemy)
			{
				int32 Damage = 0;
				bool bHeadShot = false;

				if (BeamHitResult.BoneName.ToString() == HitEnemy->GetHeadBone()) // 머리에 맞았을때
				{
					Damage = EquippedWeapon->GetHeadShotDamage();
					bHeadShot = true;
					UGameplayStatics::ApplyDamage(
						BeamHitResult.GetActor(),
						Damage,
						GetController(),
						this,
						UDamageType::StaticClass());
				}
				else // 몸에 맞았을때
				{
					Damage = EquippedWeapon->GetDamage();
					bHeadShot = false;
					UGameplayStatics::ApplyDamage(
						BeamHitResult.GetActor(),
						Damage,
						GetController(),
						this,
						UDamageType::StaticClass());
				}

				HitEnemy->ShowHitNumber(Damage, BeamHitResult.Location, bHeadShot);
			}
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("X Hit Actor!!"));

			if (ImpactParticles)
			{
					
				UGameplayStatics::SpawnEmitterAtLocation(
					GetWorld(),
					ImpactParticles,
					BeamHitResult.Location);
			}
		}

			

		if (BeamParticles)
		{
			UParticleSystemComponent* Beam = UGameplayStatics::SpawnEmitterAtLocation(
				GetWorld(),
				BeamParticles,
				SocketTransform.GetLocation());
			if (Beam)
			{
				Beam->SetVectorParameter("Target", BeamHitResult.Location);
			}
		}
	}
}

void AShooterCharacter::PlayGunFireMontage()
{
	// Hip Fire Montage 실행.
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance && HipFireMontage)
	{
		AnimInstance->Montage_Play(HipFireMontage);
		AnimInstance->Montage_JumpToSection("StartFire");
	}
}

void AShooterCharacter::ReloadButtonPressed()
{
	ReloadWeapon();
}

void AShooterCharacter::ReloadWeapon()
{
	if (CombatState != ECombatState::ECS_Unoccupied) return;

	

	if (EquippedWeapon == nullptr) return;

	if (CarryingAmmo() && !EquippedWeapon->ClipIsFull())
	{
		if (bAiming)
		{
			StopAiming();
		}
		CombatState = ECombatState::ECS_Reloading;
		UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
		if (AnimInstance && ReloadMontage)
		{
			AnimInstance->Montage_Play(ReloadMontage);
			AnimInstance->Montage_JumpToSection(
				EquippedWeapon->GetReloadMontageSection());
		}
	}
}

bool AShooterCharacter::CarryingAmmo()
{
	if (EquippedWeapon == nullptr) return false;

	auto AmmoType = EquippedWeapon->GetAmmoType();

	if (AmmoMap.Contains(AmmoType))
	{
		return AmmoMap[AmmoType] > 0;
	}
	return false;
}

void AShooterCharacter::GrabClip()
{
	if (EquippedWeapon == nullptr) return;
	if (HandSceneComponent == nullptr) return;

	int32 ClipBoneIndex{ EquippedWeapon->GetItemMesh()->GetBoneIndex(EquippedWeapon->GetClipBoneName())};
	ClipTransform = EquippedWeapon->GetItemMesh()->GetBoneTransform(ClipBoneIndex);

	FAttachmentTransformRules AttachmentRules(EAttachmentRule::KeepRelative, true);
	HandSceneComponent->AttachToComponent(GetMesh(),AttachmentRules,FName(TEXT("hand_l")));
	HandSceneComponent->SetWorldTransform(ClipTransform);

	EquippedWeapon->SetMovingClip(true);
}

void AShooterCharacter::ReleaseClip()
{
	EquippedWeapon->SetMovingClip(false);
}

void AShooterCharacter::CrouchButtonPressed()
{
	if (!GetCharacterMovement()->IsFalling())
	{
		bCrouching = !bCrouching;
	}

	if (bCrouching)
	{
		GetCharacterMovement()->MaxWalkSpeed = CrouchMovementSpeed;
		GetCharacterMovement()->GroundFriction = CrouchingGroundFriction;
		
	}
	else
	{
		GetCharacterMovement()->MaxWalkSpeed = BaseMovementSpeed;
		GetCharacterMovement()->GroundFriction = BaseGroundFriction;
	}
}

void AShooterCharacter::Jump()
{
	if (bCrouching)
	{
		bCrouching = false;
		GetCharacterMovement()->MaxWalkSpeed = BaseMovementSpeed;
		GetCharacterMovement()->GroundFriction = BaseGroundFriction;
	}
	else
	{
		ACharacter::Jump();
	}
}

void AShooterCharacter::InterpCapsuleHalfHeight(float DeltaTime)
{
	float TargetCapsuleHalfHeight = 0.f;
	if (bCrouching)
	{
		TargetCapsuleHalfHeight = CrouchingCapsuleHalfHeight;
	}
	else
	{
		TargetCapsuleHalfHeight = StandingCapsuleHalfHeight;
	}

	const float InterpHalfHeight = FMath::FInterpTo(
		GetCapsuleComponent()->GetScaledCapsuleHalfHeight(),
		TargetCapsuleHalfHeight,
		DeltaTime,
		20.f);

	// Negative value if crouching, Postive value if standing...
	const float DeltaCapsuleHalfHeight = InterpHalfHeight - GetCapsuleComponent()->GetScaledCapsuleHalfHeight();
	const FVector MeshOffset{ 0.f, 0.f, -DeltaCapsuleHalfHeight };
	GetMesh()->AddLocalOffset(MeshOffset);

	GetCapsuleComponent()->SetCapsuleHalfHeight(InterpHalfHeight);
}

void AShooterCharacter::Aim()
{
	bAiming = true;
	GetCharacterMovement()->MaxWalkSpeed = CrouchMovementSpeed;
}

void AShooterCharacter::SetCustomDilation(float Dilation)
{
	CustomTimeDilation = Dilation;
}

void AShooterCharacter::FallingStart()
{
	TopPositionZ = GetActorLocation().Z;
}

void AShooterCharacter::FallingFinish()
{
	float BottomPositionZ = GetActorLocation().Z;

	const float FallDistance = TopPositionZ - BottomPositionZ;
	UE_LOG(LogTemp, Warning, TEXT("FallingFinish! :%f"), FallDistance);
	if (FallDistance >= 400.f)
	{
		Stun();
		if (LandSound)
		{
			UGameplayStatics::PlaySound2D(this, LandSound);
		}
		const float FallDamage = (FallDistance-400.f)*0.1;
		TakeDamage(FallDamage, FDamageEvent(), GetController(), this);
	}
}

void AShooterCharacter::StopAiming()
{
	bAiming = false;
	if (!bCrouching)
	{
		GetCharacterMovement()->MaxWalkSpeed = BaseMovementSpeed;
	}
}

void AShooterCharacter::PickupAmmo(class AAmmo* Ammo)
{
	if (AmmoMap.Find(Ammo->GetAmmoType()))
	{
		int32& AmmoCount = AmmoMap[Ammo->GetAmmoType()];
		AmmoCount += Ammo->GetItemCount();
	}

	if (EquippedWeapon->GetAmmoType() == Ammo->GetAmmoType())
	{
		if (EquippedWeapon->GetAmmo() == 0)
		{
			ReloadWeapon();
		}
	}

	Ammo->Destroy();
}

void AShooterCharacter::InitializeInterpLocations()
{
	FInterpLocation WeaponLocation{ WeaponInterpComp,0 };
	InterpLocations.Add(WeaponLocation);

	FInterpLocation InterpLoc1{ InterpComp1, 0 };
	InterpLocations.Add(InterpLoc1);

	FInterpLocation InterpLoc2{ InterpComp2, 0 };
	InterpLocations.Add(InterpLoc2);

	FInterpLocation InterpLoc3{ InterpComp3, 0 };
	InterpLocations.Add(InterpLoc3);

	FInterpLocation InterpLoc4{ InterpComp4, 0 };
	InterpLocations.Add(InterpLoc4);

	FInterpLocation InterpLoc5{ InterpComp5, 0 };
	InterpLocations.Add(InterpLoc5);

	FInterpLocation InterpLoc6{ InterpComp6, 0 };
	InterpLocations.Add(InterpLoc6);
}

void AShooterCharacter::OneKeyPressed()
{
	if (EquippedWeapon->GetSlotIndex() == 1) return;
	Inventory->ExchangeInventoryItems(EquippedWeapon->GetSlotIndex(), 1);
}

void AShooterCharacter::TwoKeyPressed()
{
	if (EquippedWeapon->GetSlotIndex() == 2) return;
	Inventory->ExchangeInventoryItems(EquippedWeapon->GetSlotIndex(), 2);
}

void AShooterCharacter::ThreeKeyPressed()
{
	if (EquippedWeapon->GetSlotIndex() == 3) return;
	Inventory->ExchangeInventoryItems(EquippedWeapon->GetSlotIndex(), 3);
}

EPhysicalSurface AShooterCharacter::GetSurfaceType()
{
	FHitResult HitResult;
	const FVector Start = GetActorLocation();
	const FVector End = Start + FVector(0.f, 0.f, -400.f);

	FCollisionQueryParams QueryParams;
	QueryParams.bReturnPhysicalMaterial = true;

	GetWorld()->LineTraceSingleByChannel(
		HitResult,
		Start,
		End,
		ECollisionChannel::ECC_Visibility,
		QueryParams);
	
	return UPhysicalMaterial::DetermineSurfaceType(HitResult.PhysMaterial.Get());
	
}

void AShooterCharacter::EndStun()
{
	CombatState = ECombatState::ECS_Unoccupied;

	if (bAimingButtonPressed)
	{
		Aim();
	}
}

void AShooterCharacter::Die()
{
	
	bDead = true;
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance && DeathMontage)
	{
		UE_LOG(LogTemp, Warning, TEXT("DIE!!!!!! "));
		AnimInstance->Montage_Play(DeathMontage);
		AnimInstance->Montage_JumpToSection(TEXT("Death01"));
	}
}

void AShooterCharacter::FinishDeath()
{
	GetMesh()->bPauseAnims = true;
	APlayerController* PC = UGameplayStatics::GetPlayerController(this,0);
	if (PC)
	{
		DisableInput(PC);
	}

	UShooter_GameInstance* Instance = Cast<UShooter_GameInstance>(GetGameInstance());
	if (Instance)
	{
		Instance->LoadVictoryWidget("Failed..");
	}
}

int32 AShooterCharacter::GetInterpLocationIndex()
{
	int32 LowestIndex = 1;
	int32 LowestCount = INT_MAX;

	for (int32 i = 1; i < InterpLocations.Num(); i++)
	{
		if (InterpLocations[i].ItemCount < LowestCount)
		{
			LowestIndex = i;
			LowestCount = InterpLocations[i].ItemCount;
		}
	}

	return LowestIndex;
}

void AShooterCharacter::IncrementInterpLocItemCount(int32 Index, int32 Amount)
{
	if (Amount < -1 || Amount > 1) return;

	if (InterpLocations.Num() >= Index)
	{
		InterpLocations[Index].ItemCount += Amount;
	}
} 

void AShooterCharacter::StartPickupSoundTimer()
{
	bShouldPlayPickupSound = false;
	GetWorldTimerManager().SetTimer(
		PickupSoundTimer,
		this,
		&AShooterCharacter::ResetPickupSoundTimer,
		PickupSoundResetTime);
}

void AShooterCharacter::StartEquipSoundTimer()
{
	bShouldPlayEquipSound = false;
	GetWorldTimerManager().SetTimer(
		EquipSoundTimer,
		this,
		&AShooterCharacter::ResetEquipSoundTimer,
		EquipSoundResetTime);
}

void AShooterCharacter::Stun()
{
	if (Health <= 0.f) return;

	CombatState = ECombatState::ECS_Stunned;

	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance && HitReactMontage)
	{
		AnimInstance->Montage_Play(HitReactMontage);
	}
}

// Called every fre
void AShooterCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	CameraInterpZoom(DeltaTime);
	SetLookRates();

	CalculateCrosshairSpread(DeltaTime);
	// OverlappedItemCount를 확인하고 trace.
	TraceForItems();

	InterpCapsuleHalfHeight(DeltaTime);	
}

// Called to bind functionality to input
void AShooterCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	check(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &AShooterCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AShooterCharacter::MoveRight);
	PlayerInputComponent->BindAxis("Turn", this, &AShooterCharacter::Turn);
	PlayerInputComponent->BindAxis("LookUp", this, &AShooterCharacter::LookUp);
	PlayerInputComponent->BindAxis("TurnRate", this, &AShooterCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUpRate", this, &AShooterCharacter::LookUpAtRate);

	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &AShooterCharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	PlayerInputComponent->BindAction("FireButton", IE_Pressed, this, &AShooterCharacter::FireButtonPressed);
	PlayerInputComponent->BindAction("FireButton", IE_Released, this, &AShooterCharacter::FireButtonReleased);


	PlayerInputComponent->BindAction("AimingButton", IE_Pressed, this, &AShooterCharacter::AimingButtonPressed);
	PlayerInputComponent->BindAction("AimingButton", IE_Released, this, &AShooterCharacter::AimingButtonReleased);

	PlayerInputComponent->BindAction("Select", IE_Pressed, this, &AShooterCharacter::SelectButtonPressed);
	PlayerInputComponent->BindAction("Select", IE_Released, this, &AShooterCharacter::SelectButtonReleased);

	PlayerInputComponent->BindAction("ReloadButton", IE_Pressed, this, &AShooterCharacter::ReloadButtonPressed);
	
	PlayerInputComponent->BindAction("Crouch", IE_Pressed, this, &AShooterCharacter::CrouchButtonPressed);

	PlayerInputComponent->BindAction("1Key", IE_Pressed, this, &AShooterCharacter::OneKeyPressed);
	PlayerInputComponent->BindAction("2Key", IE_Pressed, this, &AShooterCharacter::TwoKeyPressed);
	PlayerInputComponent->BindAction("3Key", IE_Pressed, this, &AShooterCharacter::ThreeKeyPressed);
	

}

void AShooterCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
}

void AShooterCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	
}

void AShooterCharacter::FinishReloading()
{
	if (CombatState == ECombatState::ECS_Stunned) return;
	CombatState = ECombatState::ECS_Unoccupied;

	if (bAimingButtonPressed)
	{
		Aim();
	}
	if (EquippedWeapon == nullptr) return;
	const auto AmmoType = EquippedWeapon->GetAmmoType();

	if (AmmoMap.Contains(AmmoType))
	{
		int32 CarriedAmmo = AmmoMap[AmmoType];

		// 
		const int32 MagEmptySpace = EquippedWeapon->GetMagazineCapacity() - EquippedWeapon->GetAmmo();

		if (MagEmptySpace > CarriedAmmo)
		{
			EquippedWeapon->ReloadAmmo(CarriedAmmo);
			CarriedAmmo = 0;
			AmmoMap.Add(AmmoType, CarriedAmmo);
		}
		else
		{
			EquippedWeapon->ReloadAmmo(MagEmptySpace);
			CarriedAmmo -= MagEmptySpace;
			AmmoMap.Add(AmmoType, CarriedAmmo);
		}
	}
}

void AShooterCharacter::FinishEquipping()
{
	if (CombatState == ECombatState::ECS_Stunned) return;

	CombatState = ECombatState::ECS_Unoccupied;
	if (bAimingButtonPressed)
	{
		Aim();
	}
}

void AShooterCharacter::ResetPickupSoundTimer()
{
	bShouldPlayPickupSound = true;
}

void AShooterCharacter::ResetEquipSoundTimer()
{
	bShouldPlayEquipSound = true;
}

float AShooterCharacter::GetCrosshairSpreadMultiplier() const
{
	return CrosshairSpreadMultiplier;
}

void AShooterCharacter::IncrementOverlappedItemCount(int8 Amount, AItem* TraceItem)
{
	if (OverlappedItemCount + Amount <= 0)
	{
		OverlappedItemCount = 0;
		OverlappedItemList.Remove(TraceItem);
		bShouldTraceForItems = false;
	}
	else
	{
		OverlappedItemCount += Amount;
		OverlappedItemList.Add(TraceItem);
		bShouldTraceForItems = true;
	}
}

//FVector AShooterCharacter::GetCameraInterpLocation()
//{
//	const FVector CameraWorldLocation{ FollowCamera->GetComponentLocation() };
//	const FVector CameraForward{ FollowCamera->GetForwardVector() };
//	// Desired = CameraWorldLocation + (Forward * A) + (Up * B)
//
//	return CameraWorldLocation + CameraForward * CameraInterpDistance + FVector(0.f, 0.f, CameraInterpElevation);
//}

void AShooterCharacter::GetPickupItem(AItem* Item)
{
	if (Inventory == nullptr) return;

	Item->PlayEquipSound();

	auto Weapon = Cast<AWeapon>(Item);
	if (Weapon)
	{
		if (Inventory->GetNum() < INVENTORY_CAPACITY)
		{
			Weapon->SetSlotIndex(Inventory->GetNum());
			Inventory->AddItem(Weapon);
			Weapon->SetItemState(EItemState::EIS_PickedUp);
		}
		else
		{
			SwapWeapon(Weapon);
		}
	}
	AAmmo* Ammo = Cast<AAmmo>(Item);
	if (Ammo)
	{
		PickupAmmo(Ammo);
	}
}

FInterpLocation AShooterCharacter::GetInterpLocation(int32 Index)
{
	if (Index <= InterpLocations.Num())
	{
		return InterpLocations[Index];
	}
	return FInterpLocation();
}

