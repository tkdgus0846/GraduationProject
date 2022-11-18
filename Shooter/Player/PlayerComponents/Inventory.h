// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Inventory.generated.h"

const int32 INVENTORY_CAPACITY = 4;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FHighlightIconDelegate, int32, SlotIndex, bool, bStartAnimation);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SHOOTER_API UInventory : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UInventory();

	
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Inventory, Meta = (AllowPrivateAccess = true))
	TArray<class AItem*> InventoryItems;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Inventory, Meta = (AllowPrivateAccess = true))
	int32 HighlightedSlot;

	UPROPERTY(BlueprintAssignable, Category = Delegates, Meta = (AllowPrivateAccess = true))
	FHighlightIconDelegate HighlightIconDelegate;

	
public:	
	FORCEINLINE TArray<class AItem*>* GetInventory() {return &InventoryItems;}
	FORCEINLINE int32 GetNum() const { return InventoryItems.Num(); }
	FORCEINLINE int32 GetHighlightedSlot() const { return HighlightedSlot; }

	void AddItem(class AItem* Item);

	UFUNCTION(BlueprintCallable)
	class AItem* GetItem(int32 Index); 

	void SetItem(int32 Index, class AItem* Item);

	void ExchangeInventoryItems(int32 CurrentItemIndex, int32 NewItemIndex);

	int32 GetEmptyInventorySlot();

	void HighlightInventorySlot();
	void UnHighlightInventorySlot();
};
