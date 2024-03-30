// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "C_BotController.generated.h"

/**
 * 
 */
UCLASS()
class GAMECORE_API AC_BotController : public AAIController
{
	GENERATED_BODY()

public:

	AC_BotController();

public:
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnBotFinishedMovement);

	void OnMovementEnd(FAIRequestID p_RequestID, const FPathFollowingResult& p_Result);

	UPROPERTY()
		FOnBotFinishedMovement OnBotFinishedMovementEvent;

};
