// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameCore/Item/C_Item.h"

/**
 *
 */
class GAMECORE_API RegistryObject
{
public:
	RegistryObject() {}
	~RegistryObject() {}

	static TMap<TSubclassOf<AC_Item>, FString> RegisteredItemsClasses;

	static void Register(TSubclassOf<AC_Item> p_ItemClass, FString p_Id) {
		RegisteredItemsClasses.Add(TTuple<TSubclassOf<AC_Item>, FString>(p_ItemClass, p_Id));
	}
};

TMap < TSubclassOf<AC_Item>, FString> RegistryObject::RegisteredItemsClasses = TMap < TSubclassOf<AC_Item>, FString>{};