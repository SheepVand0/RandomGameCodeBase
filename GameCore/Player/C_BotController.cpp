// Fill out your copyright notice in the Description page of Project Settings.


#include "C_BotController.h"

AC_BotController::AC_BotController() {
	GetPathFollowingComponent()->OnRequestFinished.AddUObject(this, &AC_BotController::OnMovementEnd);
}

void AC_BotController::OnMovementEnd(FAIRequestID p_RequestID, const FPathFollowingResult& p_Result) {

}