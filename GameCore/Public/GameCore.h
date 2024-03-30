// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"
#include "GameCore/Item/C_Item.h"
#include "GameCore/Item/Abilities/C_Hand.h"

#define MAIN_WEAPONS_CATEGORY FString("MainWeapon")
#define PISTOLS_CATEGORY FString("Pistol")
#define SNIPERS_CATEGORY FString("Sniper")
#define BODY_WEAPONS_CATEGORY FString("BodyWeapon")

class FGameCoreModule : public IModuleInterface
{
public:

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
};

class FGameCore {

public:

	static FItemProperties GetHandProperties() {
		return AC_Hand::GetPropertiesCopy();
	}
};
