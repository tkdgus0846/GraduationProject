// Fill out your copyright notice in the Description page of Project Settings.


#include "Inventory.h"
#include "../../Items/Item.h"
#include "../ShooterCharacter.h"
#include "../../Items/Weapon.h"

// Sets default values for this component's properties
UInventory::UInventory() :
	HighlightedSlot(-1)
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
	
}


// Called when the game starts
void UInventory::BeginPlay()
{
	Super::BeginPlay();

	// ...
	InventoryItems.Add(nullptr);
}


// Called every frame
void UInventory::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UInventory::AddItem(AItem* Item)
{
	InventoryItems.Add(Item);
}

AItem* UInventory::GetItem(int32 Index)
{
	return (InventoryItems.Num() > Index) ? InventoryItems[Index] : nullptr;
}

void UInventory::SetItem(int32 Index, AItem* Item)
{
	InventoryItems[Index] = Item;
}

void UInventory::ExchangeInventoryItems(int32 CurrentItemIndex, int32 NewItemIndex)
{
	AShooterCharacter* Character = Cast<AShooterCharacter>(GetOwner());

	if (Character == nullptr) return;

	const bool bCanExchangeItems =
		(CurrentItemIndex != NewItemIndex) &&
		(NewItemIndex < GetNum()) &&
		(Character->GetCombatState() == ECombatState::ECS_Unoccupied || 
		Character->GetCombatState() == ECombatState::ECS_Equipping);

	if (bCanExchangeItems)
	{
		if (Character->GetAiming())
		{
			Character->StopAiming();
		}
		AWeapon* OldEquippedWeapon = Cast<AWeapon>(InventoryItems[CurrentItemIndex]);
		AWeapon* NewWeapon = Cast<AWeapon>(InventoryItems[NewItemIndex]);

		if (OldEquippedWeapon == nullptr || NewWeapon == nullptr) return;

		Character->EquipWeapon(NewWeapon);

		OldEquippedWeapon->SetItemState(EItemState::EIS_PickedUp);
		NewWeapon->SetItemState(EItemState::EIS_Equipped);

		Character->SetCombatState(ECombatState::ECS_Equipping);

		UAnimInstance* AnimInstance = Character->GetMesh()->GetAnimInstance();
		if (AnimInstance && Character->GetEquipMontage())
		{
			AnimInstance->Montage_Play(Character->GetEquipMontage());
			AnimInstance->Montage_JumpToSection(FName("Equip"));
		}
		NewWeapon->PlayEquipSound(true);
	}
	
}

int32 UInventory::GetEmptyInventorySlot()
{
	for (int32 i = 0; i < GetNum(); i++)
	{
		if (GetItem(i) == nullptr)
		{
			return i;
		}
	}

	if (GetNum() < INVENTORY_CAPACITY)
	{
		return GetNum();
	}
	return -1; // 인벤토리가 꽉 참.
}

void UInventory::HighlightInventorySlot()
{
	const int32 EmptySlot{ GetEmptyInventorySlot() };

	HighlightIconDelegate.Broadcast(EmptySlot, true);
	HighlightedSlot = EmptySlot;
}

void UInventory::UnHighlightInventorySlot()
{
	HighlightIconDelegate.Broadcast(HighlightedSlot, false);
	HighlightedSlot = -1;
}

