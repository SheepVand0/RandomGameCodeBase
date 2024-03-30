// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryComponent.h"

// Sets default values for this component's properties
UInventoryComponent::UInventoryComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
}

FItemProperties UInventoryComponent::GetInventoryItem(int p_Index)
{
	if (!m_Inventory.IsValidIndex(p_Index))
		return FItemProperties::GetNullItem();

	return m_Inventory[p_Index];
}

void UInventoryComponent::CreateSlot(FString p_Category) {
	m_Inventory.Add(FItemProperties::GetNullItem());
	SetLastIndexCategory(p_Category);
}

/// <summary>
/// Gets the slot with right category and set the item to p_Item
/// </summary>
/// <param name="p_Item"></param>
/// <returns></returns>
bool UInventoryComponent::AddInventoryItem(FItemProperties p_Item)
{
	TArray<FString> l_Values;
	m_SlotsCategories.GenerateValueArray(l_Values);
	
	if (!(l_Values.Contains(p_Item.Category) || l_Values.Contains(""))) return false;

	int l_Index = *m_SlotsCategories.FindKey(p_Item.Category);

	m_Inventory[l_Index] = p_Item;

	OnItemAdded.Broadcast(p_Item, l_Index);

	return true;
}

void UInventoryComponent::RemoveInventoryItem(int p_ItemIndex)
{
	if (!(m_Inventory.IsValidIndex(p_ItemIndex)))
		return;

	if (m_Inventory[p_ItemIndex].RemovableFromInventory == false) return;

	m_Inventory.RemoveAt(p_ItemIndex);

	if (p_ItemIndex == SelectedIndex)
		OnNeedUpdateOnItemRemoved.Broadcast();
}

void UInventoryComponent::SetIndexCategory(int p_Index, FString p_Category) {
	TArray<int> l_Keys = TArray<int>();
	TArray<FString> l_Values = TArray<FString>();
	m_SlotsCategories.GenerateKeyArray(l_Keys);
	m_SlotsCategories.GenerateValueArray(l_Values);

	for (int l_i = 0; l_i < l_Keys.Num(); l_i++) {
		if (l_Keys[l_i] == p_Index) {
			m_SlotsCategories[l_i] = p_Category;
			return;
		}
	}
}

void UInventoryComponent::SetLastIndexCategory(FString p_Category) {
	SetIndexCategory(m_SlotsCategories.Num() - 1, p_Category);
}

bool UInventoryComponent::CanAddItemToIndex(int p_Index, FItemProperties p_Item) {
	if (!m_SlotsCategories.Contains(p_Index)) return false;
	FString l_Value = m_SlotsCategories.FindRef(p_Index);
	return l_Value == p_Item.Category;
}

int UInventoryComponent::TryGetInventoryItemIndex(FItemProperties p_item)
{
	for (int l_i = 0; l_i < m_Inventory.Num(); l_i++)
	{
		if (FItemProperties::Equal(p_item, GetInventoryItem(l_i)))
			return l_i;
	}

	return -1;
}

void UInventoryComponent::SelectInventoryItem(int p_Index)
{
	FItemProperties l_Item = GetInventoryItem(p_Index);
	if (FItemProperties::IsNullItem(l_Item)) {
		return;
	}

	SelectedIndex = p_Index;
	SelectInventoryItemFromProperties(l_Item);
}

void UInventoryComponent::SelectInventoryItemFromProperties(FItemProperties p_ItemProperties) {
	OnItemSelected.Broadcast(p_ItemProperties);
}

void UInventoryComponent::SetInventoryItem(int p_Index, FItemProperties p_Item)
{
	if (p_Index <= m_MaxInventoryLength)
	{
		if (!CanAddItemToIndex(p_Index, p_Item)) return;

		m_Inventory[p_Index] = p_Item;
	}

	OnItemAdded.Broadcast(p_Item, p_Index);
}


void UInventoryComponent::SetInventoryLength(int p_Length, TMap<int, FString> p_Categories, bool p_KeepInventory) {
	TArray<FItemProperties> l_OldInventory = m_Inventory;
	m_Inventory.Empty();
	m_SlotsCategories = p_Categories;

	for (int l_i = 0; l_i < p_Length; l_i++) {
		if (p_Categories.Contains(l_i))
			CreateSlot(p_Categories.FindRef(l_i));
		else {
			CreateSlot(FString(""));
		}
	}

	if (!p_KeepInventory) return;

	for (int l_i = 0; l_i < l_OldInventory.Num(); l_i++) {
		AddInventoryItem(l_OldInventory[l_i]);
	}
}

int UInventoryComponent::GetInventoryLength() {
	return m_Inventory.Num();
}

int UInventoryComponent::GetInventoryMaxLength() {
	return m_MaxInventoryLength;
}