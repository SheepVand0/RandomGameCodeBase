// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Defaults/UI_PrimaryTextBlock.h"
#include "Defaults/UI_SecondaryTextBlock.h"
#include "UI_Player.generated.h"

/**
 *
 */
UCLASS()
class GAMECORE_API UUI_Player : public UUserWidget
{
	GENERATED_BODY()

protected:

	virtual void NativeTick(const FGeometry& p_Geometry, float p_DeltaTime) override;

	float m_TimerDuration = 0;

	float m_CurrentTime = 0;

public:

	UPROPERTY(meta = (BindWidget))
		class UUI_PrimaryTextBlock* CAmmoCount;

	UPROPERTY(meta = (BindWidget))
		class UUI_SecondaryTextBlock* CMaxMagazineAmmoCount;

	UPROPERTY(meta = (BindWidget))
		class UUI_SecondaryTextBlock* CMaxAmmoCount;

	UPROPERTY(meta = (BindWidget))
		class UUI_PrimaryTextBlock* CLife;

	UPROPERTY(meta = (BindWidget))
		class UUI_SecondaryTextBlock* CShield;

	UPROPERTY(meta = (BindWidget))
		class UUI_PrimaryTextBlock* CAgentsComingTime;

	UPROPERTY(meta = (BindWidget))
		class UUI_SecondaryTextBlock* CEnnemies;

	void SetLife(int p_Life) {
		CLife->SetText(FText::FromString(FString::SanitizeFloat(p_Life, 0)));
	}

	void SetShield(int p_Shield) {
		CShield->SetText(FText::FromString(FString::SanitizeFloat(p_Shield, 0)));
	}

	void SetMagazineAmmoCount(int p_Count) {
		CAmmoCount->SetText(FText::FromString(FString::SanitizeFloat(p_Count, 0)));
	}

	void SetMaxMagazineAmmoCount(int p_Count) {
		CMaxMagazineAmmoCount->SetText(FText::FromString(FString("/") + FString::SanitizeFloat(p_Count, 0)));
	}

	void SetMaxAmmoCount(int p_Count) {
		CMaxAmmoCount->SetText(FText::FromString(FString::SanitizeFloat(p_Count, 0)));
	}

	void SetEnnemiesCount(int p_Count) {
		CEnnemies->SetText(FText::FromString(FString::SanitizeFloat(p_Count, 0)));
	}

	void StartTimer(float p_Duration) {
		m_TimerDuration = p_Duration;
		m_CurrentTime = 0;
	}

	void ShowAmmoBox() {
		CMaxAmmoCount->SetVisibility(ESlateVisibility::Visible);
		CAmmoCount->SetVisibility(ESlateVisibility::Visible);
		CMaxMagazineAmmoCount->SetVisibility(ESlateVisibility::Visible);
	}

	void HideAmmoBox() {
		CMaxAmmoCount->SetVisibility(ESlateVisibility::Hidden);
		CAmmoCount->SetVisibility(ESlateVisibility::Hidden);
		CMaxMagazineAmmoCount->SetVisibility(ESlateVisibility::Hidden);
	}
};
