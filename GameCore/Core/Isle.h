// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameCore/Player/C_Player.h"
#include "GameCore/Utils/Other.h"
#include "GameCore/Components/EventComponent.h"
#include "Constants.h"
#include "Isle.generated.h"

UCLASS()
class GAMECORE_API AIsle : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AIsle();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void SpawnPlayers(int p_Winner);

	UFUNCTION()
	void OnEventReceived(FString p_Event);

	UPROPERTY()
	UEventComponent* EventComponent;

	UPROPERTY()
	UDataComponent* DataComponent;

	UPROPERTY(EditAnywhere)
	int IsleID;

	UPROPERTY(EditAnywhere)
	/// <summary>
	/// Key : Team ID
	/// Value : Average spawn positions
	/// </summary>
	TMap<int, FVector> Spawns;


};
