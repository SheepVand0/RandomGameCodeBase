// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameCore/Player/Ability.h"
#include "FlashAbility.generated.h"

/**
 *
 */
UCLASS()
class GAMECORE_API AFlashAbility : public AAbility
{
	GENERATED_BODY()

public:

	AFlashAbility();

	virtual void OnUse() override;

	virtual void OnAbilityStop() override;

};
