// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Item.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnItemModified);

UENUM(BlueprintType)
enum class EItemRarity : uint8
{
	IR_Common UMETA(DisplayName = "Common"),
	IR_Unommon UMETA(DisplayName = "Uncommon"),
	IR_Rare UMETA(DisplayName = "Rare"),
	IR_VeryRare UMETA(DisplayName = "Very Rare"),
	IR_Legendary UMETA(DisplayName = "Legendary"),
};

/**
 * 
 */
UCLASS(Blueprintable, EditInlineNew, DefaultToInstanced)
class SURVIVALGAME_API UItem : public UObject
{
	GENERATED_BODY()

protected: 

	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
	virtual bool IsSupportedForNetworking() const override;

#if WITH_EDITOR
	virtual void PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent) override;
#endif

public:

	UItem();

	// Mesh to display for this items pickup
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Item")
	class UStaticMesh* PickupMesh;

	// Thumbnail for the item
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Item")
	class UTexture2D* Thumbnail;

	// Display name for the item in the inventory
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Item")
	FText ItemDisplayName;
	
	// Optional description for the item
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Item", meta = (MultiLine = true))
	FText ItemDescription;

	// Text for using the item (Equip, Eat, etc.)
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Item")
	FText UseActionText;

	// Enum denoting the rarity of the item
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Item")
	EItemRarity Rarity;

	// Weight of the item
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Item", meta = (ClampMin = 0.0))
	float Weight;

	// Whether or not this item can be stacked
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Item")
	bool bStackable;

	// The maximum size that a stack of items can be
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Item", meta = (ClampMin = 2, EditCondition = bStackable))
	int32 MaxStackSize;

	// The tooltip in the inventory for this item
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Item")
	TSubclassOf<class UItemTooltip> ItemTooltip;

	// The amount of the item (handled by the server)
	UPROPERTY(ReplicatedUsing = OnRep_Quantity, EditAnywhere, Category = "Item", meta = (UIMin = 1, EditCondition = bStackable))
	int32 Quantity;

	// Pointer to the inventory that owns this item
	UPROPERTY()
	class UInventoryComponent* OwningInventory;

	// Used to efficiently replicate inventory items
	UPROPERTY()
	int32 RepKey;

	UPROPERTY(BlueprintAssignable)
	FOnItemModified OnItemModified;

	UFUNCTION()
	void OnRep_Quantity();

	UFUNCTION(BlueprintCallable, Category = "Item")
	void SetQuantity(const int32 NewQuantity);

	UFUNCTION(BlueprintPure, Category = "Item")
	FORCEINLINE int32 GetQuantity() const { return Quantity; }

	UFUNCTION(BlueprintCallable, Category = "Item")
	FORCEINLINE float GetStackWeight() const { return Quantity * Weight; }

	UFUNCTION(BlueprintPure, Category = "Item")
	virtual bool ShouldShowInInventory() const;

	virtual void Use(class ASurvivalCharacter* Character);
	virtual void AddedToInventory(class UInventoryComponent* Inventory);

	// Mark the object as needing replication. We must call this internally after modifying any replicated properties
	void MarkDirtyForReplication();

};
