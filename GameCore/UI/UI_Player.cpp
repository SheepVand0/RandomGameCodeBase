// Fill out your copyright notice in the Description page of Project Settings.


#include "UI_Player.h"

void UUI_Player::NativeTick(const FGeometry& p_Geometry, float p_DeltaTime) {

	m_CurrentTime += p_DeltaTime;

	if (m_TimerDuration <= 0) {
		if (CAgentsComingTime->Text.ToString() != FString("0:00"))
			CAgentsComingTime->SetText(FText::FromString("0:00"));
		return;
	}

	int l_Minutes = (m_TimerDuration - m_CurrentTime)/60;
	int l_Seconds = (m_TimerDuration - m_CurrentTime) - (l_Minutes * 60);

	CAgentsComingTime->SetText(FText::FromString(FString::SanitizeFloat(l_Minutes, 0) + ":" + FString::SanitizeFloat(l_Seconds, 0)));
}