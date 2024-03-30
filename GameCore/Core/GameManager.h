// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Kismet/GameplayStatics.h"
#include "GameCore/Player/Ability.h"
#include "GameCore/Components/DataComponent.h"
#include "GameCore/Components/EventComponent.h"
//#include "GameCore/Utils/SEvent.h"
//#include "GameCore/Utils/MethodsHelper.h"
#include "GameFramework/Character.h"
#include "Constants.h"
#include "GameManager.generated.h"

USTRUCT()
struct FTeam {

	GENERATED_BODY()

public:

	FTeam();

	FTeam(int p_TeamID, TArray<int> p_Players);

	TArray<int> Players;

	int TeamID;

};

UCLASS()
class GAMECORE_API AGameManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGameManager();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	float m_LastTime;
	
	float m_RoundStartTime;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	//UPROPERTY()
		//SEvent<float> OnTimeUpdate;

	UPROPERTY()
		TMap<int, int> Scores;

	UPROPERTY()
		TMap<int, int> WeaponPoints;

	UPROPERTY()
		TArray<FTeam> Players;

	UPROPERTY()
		TMap<int, bool> AlivesPlayers;

	UPROPERTY()
		float RoundDuration;
	
	UPROPERTY()
		float Time;

	UPROPERTY()
		bool Paused;

	UPROPERTY()
		TMap<int, int> TeamLevel;

	UPROPERTY()
		int CurrentIsle;
		
	UPROPERTY()
		int LastWinner;

	UFUNCTION()
	void NotifyPlayerDeath(int p_PlayerID);

	UFUNCTION()
	void NextRound();

	UFUNCTION()
	float GetRoundTime();

	UFUNCTION()
	float GetRoundRemainingTime();

	UFUNCTION()
	void TeleportPlayersToSpawn();

	int GetAlivesPlayerForTeam(int p_Team);

	void CheckPlayers();

	void TeamWonRound(int p_TeamID);

};