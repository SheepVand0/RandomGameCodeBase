// Fill out your copyright notice in the Description page of Project Settings.


#include "GameManager.h"

FTeam::FTeam() {

}

FTeam::FTeam(int p_TeamID, TArray<int> p_Players) {
	TeamID = p_TeamID;
	Players = p_Players;
}

// Sets default values
AGameManager::AGameManager()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AGameManager::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AGameManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	Time = UKismetSystemLibrary::GetGameTimeInSeconds(GetWorld());
}

int AGameManager::GetAlivesPlayerForTeam(int p_Team) {
	int l_Count = 0;

	for (int l_i1 = 0; l_i1 < Players[p_Team].Players.Num(); l_i1++) {
		if (*AlivesPlayers.Find(Players[p_Team].Players[l_i1]) == true) {
			l_Count += 1;
		}
	}

	return l_Count;
}

void AGameManager::NotifyPlayerDeath(int p_PlayerID) {
	AlivesPlayers[p_PlayerID] = false;
}

void AGameManager::CheckPlayers() {
	for (int l_i = 0; l_i < Players.Num(); l_i++) {
		int l_AlivePeople = GetAlivesPlayerForTeam(l_i);
		if (l_AlivePeople == 0) {
			for (int l_i1 = 0; l_i1 < Scores.Num(); l_i1++) {
				if (l_i1 != l_i) {
					TeamWonRound(l_i1);
				}
			}

			break;
		}
	}
	
	NextRound();
}

void AGameManager::TeamWonRound(int p_Team) {
	Scores[p_Team] = *Scores.Find(p_Team) + 1;
}

void AGameManager::NextRound() {
	TArray<AActor*> l_Actors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AAbility::StaticClass(), l_Actors);
	for (int l_i = 0; l_i < l_Actors.Num(); l_i++) {
		l_Actors[l_i]->Destroy(true, true);
	}
	m_LastTime = Time;
	m_RoundStartTime = Time;
	TeleportPlayersToSpawn();
}

float AGameManager::GetRoundTime() {
	return Time - m_RoundStartTime;
}

float AGameManager::GetRoundRemainingTime() {
	return RoundDuration - GetRoundTime();
}

void AGameManager::TeleportPlayersToSpawn() {
	TArray<AActor*> l_Isles;
	UGameplayStatics::GetAllActorsWithTag(GetWorld(), TEXT(TAG_ISLE), l_Isles);
	for (int l_i = 0; l_i < l_Isles.Num(); l_i++) {
		UDataComponent* l_DataComp = l_Isles[l_i]->GetComponentByClass<UDataComponent>();
		int l_IsleId = (int)l_DataComp->GetFloat(ISLE_ID);

		if (l_IsleId != CurrentIsle) continue;

		UEventComponent* l_EventComponent = l_Isles[l_i]->GetComponentByClass<UEventComponent>();
		l_DataComp->AddFloat(LAST_WINNER, LastWinner);
		l_EventComponent->SendEvent(RESET_PLAYERS_EVENT);

		break;
	}
}