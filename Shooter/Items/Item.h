// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Engine/DataTable.h"
#include "Item.generated.h"

UENUM(BlueprintType)
enum class EItemRarity : uint8
{
	EIR_Damaged UMETA(DisplayName = "Damaged"),
	EIR_Common UMETA(DisplayName = "Common"),
	EIR_Uncommon UMETA(DisplayName = "Uncommon"),
	EIR_Rare UMETA(DisplayName = "Rare"),
	EIR_Legendary UMETA(DisplayName = "Legendary"),
	EIR_MAX UMETA(DisplayName = "DefaultMAX")
};

UENUM(BlueprintType)
enum class EItemState : uint8
{
	EIS_Pickup UMETA(DisplayName = "Pickup"),
	EIS_EquipInterping UMETA(DisplayName = "EquipInterping"),
	EIS_PickedUp UMETA(DisplayName = "PickedUp"),
	EIS_Equipped UMETA(DisplayName = "Equipped"),
	EIS_Falling UMETA(DisplayName = "Falling"),
	EIS_MAX UMETA(DisplayName = "DefaultMAX")
};

UENUM(BlueprintType)
enum class EItemType : uint8
{
	EIT_Ammo UMETA(DisplayName = "Ammo"),
	EIT_Weapon UMETA(DisplayName = "Weapon"),

	EIT_MAX UMETA(DisplayName = "DefaultMAX")
};

USTRUCT(BlueprintType)
struct FItemRarityTable : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FLinearColor GlowColor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FLinearColor LightColor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FLinearColor DarkColor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 NumberOfStars;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTexture2D* IconBackground;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 CustomDepthStencil;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 BonusDamage;
};

UCLASS()
class SHOOTER_API AItem : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AItem();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnSphereOverlap(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult);

	UFUNCTION()
	void OnSphereEndOverlap(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex);

	void SetActiveStars();

	virtual void SetItemProperties(EItemState State);

	void FinishInterping();

	void ItemInterp(float DeltaTime);

	FVector GetInterpLocation();

	void PlayPickupSound(bool bForcePlaySound = false);
	
	
	virtual void InitializeCustomDepth();

	void SetItemCondition(EItemRarity NewItemRarity);

	virtual void OnConstruction(const FTransform& Transform) override;

	void UpdatePulse();
	void ResetPulseTimer();
	void StartPulseTimer();
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void PlayEquipSound(bool bForcePlaySound = false);
private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Item Properties", Meta = (AllowPrivateAccess = true))
	class USkeletalMeshComponent* ItemMesh;

	/* 박스에 라인트레이스 충돌이 일어난다면 HUD 위젯에 보여준다 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item Properties", Meta = (AllowPrivateAccess = true))
	class UBoxComponent* CollisionBox;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item Properties", Meta = (AllowPrivateAccess = true))
	class UWidgetComponent* PickupWidget;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item Properties", Meta = (AllowPrivateAccess = true))
	class USphereComponent* AreaSphere;

	/* 위젯에 나타내는 이름 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item Properties", Meta = (AllowPrivateAccess = true))
	FString ItemName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item Properties", Meta = (AllowPrivateAccess = true))
	int32 ItemCount;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Rarity, Meta = (AllowPrivateAccess = true))
	EItemRarity ItemRarity;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Item Properties", Meta = (AllowPrivateAccess = true))
	TArray<bool> ActiveStars;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Item Properties", Meta = (AllowPrivateAccess = true))
	EItemState ItemState;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Item Properties", Meta = (AllowPrivateAccess = true))
	class UCurveFloat* ItemZCurve;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Item Properties", Meta = (AllowPrivateAccess = true))
	FVector ItemInterpStartLocation;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Item Properties", Meta = (AllowPrivateAccess = true))
	FVector CameraTargetLocation;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Item Properties", Meta = (AllowPrivateAccess = true))
	bool bInterping;

	/* 보간을 시작하는 타이머 핸들*/
	FTimerHandle ItemInterpTimer;
	/**/
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Item Properties", Meta = (AllowPrivateAccess = true))
	float ZCurveTime;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Item Properties", Meta = (AllowPrivateAccess = true))
	class AShooterCharacter* Character;

	float ItemInterpX;
	float ItemInterpY;

	float InterpInitialYawOffset;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Item Properties", Meta = (AllowPrivateAccess = true))
	class UCurveFloat* ItemScaleCurve;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item Properties", Meta = (AllowPrivateAccess = true))
	class USoundCue* PickupSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item Properties", Meta = (AllowPrivateAccess = true))
	class USoundCue* EquipSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item Properties", Meta = (AllowPrivateAccess = true))
	EItemType ItemType;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Item Properties", Meta = (AllowPrivateAccess = true))
	int32 InterpLocIndex;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item Properties", Meta = (AllowPrivateAccess = true))
	int32 MaterialIndex;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Item Properties", Meta = (AllowPrivateAccess = true))
	UMaterialInstanceDynamic* DynamicMaterialInstance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item Properties", Meta = (AllowPrivateAccess = true))
	UMaterialInstance* MaterialInstance;

	bool bCanChangeCustomDepth;
	
	/**/
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Item Properties", Meta = (AllowPrivateAccess = true))
	class UCurveVector* PulseCurve;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Item Properties", Meta = (AllowPrivateAccess = true))
	class UCurveVector* InterpPulseCurve;

	FTimerHandle PulseTimer;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Item Properties", Meta = (AllowPrivateAccess = true))
	float PulseCurvetime;

	UPROPERTY(VisibleAnywhere, Category = "Item Properties", Meta = (AllowPrivateAccess = true))
	float GlowAmount;

	UPROPERTY(VisibleAnywhere, Category = "Item Properties", Meta = (AllowPrivateAccess = true))
	float FresnelExponent;

	UPROPERTY(VisibleAnywhere, Category = "Item Properties", Meta = (AllowPrivateAccess = true))
	float FresnelReflectionFraction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item Properties", Meta = (AllowPrivateAccess = true))
	UTexture2D* IconItem;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item Properties", Meta = (AllowPrivateAccess = true))
	UTexture2D* IconAmmo;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Item Properties", Meta = (AllowPrivateAccess = true))
	int32 SlotIndex;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Item Properties", Meta = (AllowPrivateAccess = true))
	bool bCharacterInventoryFull;

	/* Item Rarity Data Table */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = DataTable, Meta = (AllowPrivateAccess = true))
	class UDataTable* ItemRarityDataTable;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Rarity, Meta = (AllowPrivateAccess = true))
	FLinearColor GlowColor;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Rarity, Meta = (AllowPrivateAccess = true))
	FLinearColor LightColor;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Rarity, Meta = (AllowPrivateAccess = true))
	FLinearColor DarkColor;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Rarity, Meta = (AllowPrivateAccess = true))
	int32 NumberOfstars;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Rarity, Meta = (AllowPrivateAccess = true))
	UTexture2D* IconBackground;
public:
	FORCEINLINE EItemRarity GetItemRarity() const { return ItemRarity; }

	void SetItemRarity(EItemRarity NewRarity);
	
	FORCEINLINE class UWidgetComponent* GetPickupWidget() const {return PickupWidget;}
	FORCEINLINE class USphereComponent* GetAreaSphere() const { return AreaSphere; }
	FORCEINLINE class UBoxComponent* GetCollisionBox() const { return CollisionBox; }
	FORCEINLINE EItemState GetItemState() const { return ItemState; }
	void SetItemState(EItemState State);
	FORCEINLINE USkeletalMeshComponent* GetItemMesh() const { return ItemMesh; }
	FORCEINLINE USoundCue* GetPickupSound() const { return PickupSound; }
	FORCEINLINE USoundCue* GetEquipSound() const { return EquipSound; }
	FORCEINLINE void SetPickupSound(USoundCue* Sound) { PickupSound = Sound; }
	FORCEINLINE void SetEquipSound(USoundCue* Sound) { EquipSound = Sound; }
	FORCEINLINE int32 GetItemCount() const { return ItemCount; }
	FORCEINLINE int32 GetSlotIndex() const { return SlotIndex; }
	FORCEINLINE void SetSlotIndex(int32 Index) { SlotIndex = Index;}
	FORCEINLINE void SetCharacter(class AShooterCharacter* Char) { Character = Char; }
	FORCEINLINE void SetCharacterInventoryFull(bool bFull) { bCharacterInventoryFull = bFull; }
	FORCEINLINE void SetItemName(FString Name) { ItemName = Name; }
	FORCEINLINE void SetIconItem(UTexture2D* Icon) { IconItem = Icon; }
	FORCEINLINE void SetIconAmmo(UTexture2D* Icon) { IconAmmo = Icon; }
	FORCEINLINE void SetMaterialInstance(UMaterialInstance* Instance) { MaterialInstance = Instance; }
	FORCEINLINE UMaterialInstance* GetMaterialInstance() const { return MaterialInstance; }
	FORCEINLINE void SetDynamicMaterialInstance(UMaterialInstanceDynamic* Instance) { DynamicMaterialInstance = Instance; }
	FORCEINLINE UMaterialInstanceDynamic* GetDynamicMaterialInstance() const { return DynamicMaterialInstance; }
	FORCEINLINE FLinearColor GetGlowColor() const { return GlowColor; }
	FORCEINLINE int32 GetMaterialIndex() const { return MaterialIndex; }
	FORCEINLINE void SetMaterialIndex(int32 Index) { MaterialIndex = Index; }

	void StartItemCurve(class AShooterCharacter* Char, bool bForcePlaySound = false);

	virtual void EnableCustomDepth();
	virtual void DisableCustomDepth();
	void EnableGlowMaterial();
	void DisableGlowMaterial();

};
