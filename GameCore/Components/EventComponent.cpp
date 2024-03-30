// Fill out your copyright notice in the Description page of Project Settings.


#include "EventComponent.h"

// Sets default values for this component's properties
UEventComponent::UEventComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}

void UEventComponent::SendEvent(FString p_EventName) {
	OnEventSent.Broadcast(p_EventName);
}

FString UEventComponent::GetValue(FString p_Name) {
	return AdditionalValues[p_Name];
}

void UEventComponent::SetValue(FString p_Name, FString p_Value) {
	if (AdditionalValues.Contains(p_Name)) {
		AdditionalValues[p_Name] = p_Value;
	}
	else {
		AdditionalValues.Add(TTuple<FString, FString>(p_Name, p_Value));
	}
}