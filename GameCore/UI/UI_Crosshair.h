// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Image.h"
#include "Components/PanelSlot.h"
#include "UI_Crosshair.generated.h"

/**
 * 
 */
UCLASS()
class GAMECORE_API UUI_Crosshair : public UUserWidget
{
	GENERATED_BODY()
	
public:

	UPROPERTY(meta = (BindWidget))
		class UImage* m_UpLine;

	UPROPERTY(meta = (BindWidget))
		class UImage* m_DownLine;

	UPROPERTY(meta = (BindWidget))
		class UImage* m_RightLine;

	UPROPERTY(meta = (BindWidget))
		class UImage* m_LeftLine;

	UFUNCTION()
		void SetCrosshairColor(FColor p_Color);

	UFUNCTION()
		void SetLineLength(float p_Length);

	UFUNCTION()
		void SetLineWeigth(float p_Weigth);

};
