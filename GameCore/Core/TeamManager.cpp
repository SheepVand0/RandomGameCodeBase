// Fill out your copyright notice in the Description page of Project Settings.


#include "TeamManager.h"

// Sets default values
ATeamManager::ATeamManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ATeamManager::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ATeamManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ATeamManager::Init(int p_FirstIleIndex, int p_LastIleIndex, bool p_IsFirstSide) {
	m_FirstIleIndex = p_FirstIleIndex;
	m_LastIleIndex = p_LastIleIndex;
	m_IsFirstSide = p_IsFirstSide;

	float l_Middle = (m_LastIleIndex - m_FirstIleIndex) / 2;
	int l_MiddleIle = FMath::CeilToInt(l_Middle);
	if (m_IsFirstSide) {
		m_IleIndex = l_MiddleIle - 1;
	}
	else {
		m_IleIndex = l_MiddleIle + 1;
	}
}

void ATeamManager::NotifyRoundWin() {
	if (m_IsFirstSide) {
		m_IleIndex += 1;
	}
	else {
		m_IleIndex -= 1;
	}
}

void ATeamManager::NotifyRoundLost() {
	if (m_IsFirstSide) {
		m_IleIndex -= 1;
	}
	else {
		m_IleIndex += 1;
	}
}