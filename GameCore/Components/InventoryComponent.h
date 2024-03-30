// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameCore/Item/C_Item.h"
#include "InventoryComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class GAMECORE_API UInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UInventoryComponent();

protected:

	UPROPERTY()
		TArray<FItemProperties> m_Inventory;

	UPROPERTY()
		TMap<int, FString> m_SlotsCategories;

public:

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnItemSelected, FItemProperties, p_Item);

	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnNeedUpdateOnItemRemoved);

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnItemAdded, FItemProperties, p_Item, int, p_Index);

	int SelectedIndex;

	UPROPERTY(BlueprintAssignable)
		FOnItemSelected OnItemSelected;

	UPROPERTY(BlueprintAssignable)
		FOnNeedUpdateOnItemRemoved OnNeedUpdateOnItemRemoved;

	UPROPERTY(BlueprintAssignable)
		FOnItemAdded OnItemAdded;

	UPROPERTY(EditAnywhere)
		int m_MaxInventoryLength;

	UFUNCTION()
		FItemProperties GetInventoryItem(int p_Index);

	UFUNCTION()
		bool AddInventoryItem(FItemProperties p_Item);

	UFUNCTION()
		void CreateSlot(FString p_Category);

	UFUNCTION()
		void SetIndexCategory(int p_Index, FString p_Category);

	UFUNCTION()
		void SetLastIndexCategory(FString p_Category);

	UFUNCTION()
		bool CanAddItemToIndex(int p_Index, FItemProperties p_Item);

	UFUNCTION(BlueprintCallable)
		void RemoveInventoryItem(int p_ItemIndex);

	UFUNCTION()
		int TryGetInventoryItemIndex(FItemProperties p_Item);

	UFUNCTION()
		void SelectInventoryItem(int p_Index);

	UFUNCTION()
		void SelectInventoryItemFromProperties(FItemProperties p_ItemProperties);

	UFUNCTION()
		void SetInventoryItem(int p_Index, FItemProperties p_Item);

	UFUNCTION()
		void SetInventoryLength(int p_MaxLength, TMap<int, FString> p_Categories, bool p_KeepInventory = false);

	UFUNCTION()
		int GetInventoryLength();

	UFUNCTION()
		int GetInventoryMaxLength();

};
