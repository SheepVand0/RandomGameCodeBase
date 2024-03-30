// Copyright Epic Games, Inc. All Rights Reserved.

#include "GameCore.h"
#include "RegistryObject.h"
#include "GameCore/Item/Weapons/Rifle.h"

#define LOCTEXT_NAMESPACE "FGameCoreModule"

void FGameCoreModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
	TSubclassOf<ARifle> l_RifleClass;
	RegistryObject::Register(l_RifleClass, "Rifle");
}

void FGameCoreModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FGameCoreModule, GameCore)