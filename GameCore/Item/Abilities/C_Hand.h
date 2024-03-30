// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameCore/Item/C_Item.h"
#include "Components/TimelineComponent.h"
#include "C_Hand.generated.h"

/**
 *
 */
UCLASS()
class GAMECORE_API AC_Hand : public AC_Item
{
	GENERATED_BODY()

public:

	AC_Hand();

	UPROPERTY()
		UCurveFloat* Curve;

	UPROPERTY()
		FVector OriginalHandLocation;

	FTimeline Timeline;

	void AnimUpdate(float p_Value);

	virtual void OnUse() override;

	virtual void OnStopUse() override;

	static FItemProperties GetPropertiesCopy() {
		return FItemProperties::CreateItemProperties(StaticClass(), FString("Hand"), FString("BodyWeapon"), FTransform(), false, nullptr);
	}

};
