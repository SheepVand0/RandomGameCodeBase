// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "EventComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class GAMECORE_API UEventComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UEventComponent();

protected:

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnEventSent, FString, p_EventName);

	UPROPERTY(BlueprintReadWrite)
		TMap<FString, FString> AdditionalValues;

public:

	FString EventName;

	UPROPERTY(BlueprintAssignable)
		FOnEventSent OnEventSent;

	

	void SendEvent(FString p_Event);

	FString GetValue(FString p_Name);

	void SetValue(FString p_Name, FString p_Value);
};
