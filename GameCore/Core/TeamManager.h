// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TeamManager.generated.h"

UCLASS()
class GAMECORE_API ATeamManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATeamManager();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	int m_IleIndex;

	int m_FirstIleIndex;

	int m_LastIleIndex;

	bool m_IsFirstSide;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void Init(int p_FirstIleIndex, int p_LastIleIndex, bool p_TeamSider);

	void NotifyRoundWin();

	void NotifyRoundLost();

};
