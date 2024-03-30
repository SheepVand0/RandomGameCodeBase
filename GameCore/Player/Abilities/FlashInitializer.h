// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameCore/Player/AbilityInitializer.h"
#include "FlashAbility.h"
#include "FlashInitializer.generated.h"

/**
 *
 */
UCLASS()
class GAMECORE_API AFlashInitializer : public AAbilityInitializer
{
	GENERATED_BODY()

public:

	AFlashInitializer();

	virtual TSubclassOf<AAbility> GetAbilityClass() override { return AFlashAbility::StaticClass(); }

};
