// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameCore/Player/Ability.h"
#include "GameCore/Player/AbilityInitializer.h"
#include "EventComponent.h"
#include "InventoryComponent.h"
#include "AbilityComponent.generated.h"

USTRUCT()
struct GAMECORE_API FAbilityModel {

	GENERATED_BODY()

public:

	int AbilityId = 0;

	int MaxStack = 1;

	int CurrentStack = MaxStack;

	bool UseInitializer = true;

	TSubclassOf<AAbilityInitializer> AbilityInitializerClass;

	TSubclassOf<AAbility> AbilityClass;

	bool UseDelay = true;

	float ReloadDelay = 1;

	FString ReloadEvent = FString("Reload");

	void Init(int p_AbilityId, int p_MaxStack, int p_CurrentStack, bool p_UseInitializer, TSubclassOf<AAbilityInitializer> p_AbilityInitializerClass, TSubclassOf<AAbility> p_AbilityClass, bool p_UseReloadDelay, float p_ReloadDelay, FString p_ReloadEvent) {
		AbilityId = p_AbilityId;
		MaxStack = p_MaxStack;
		CurrentStack = p_CurrentStack;
		UseInitializer = p_UseInitializer;
		AbilityInitializerClass = p_AbilityInitializerClass;
		AbilityClass = p_AbilityClass;
		UseDelay = p_UseReloadDelay;
		ReloadDelay = p_ReloadDelay;
		ReloadEvent = p_ReloadEvent;
	}

};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class GAMECORE_API UAbilityComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UAbilityComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UPROPERTY()
		UEventComponent* EventComponent;

	TArray<FAbilityModel> m_Abilities;

public:

	const FAttachmentTransformRules s_AttachmentRules = FAttachmentTransformRules(EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, false);

	AActor* SpawnAbility(int p_AbilityIndex);

	void SetAbility(TArray<FAbilityModel> p_Abilities);

	void UseAbility(int p_AbilityID, UInventoryComponent* p_Inventory);

	AActor* UseAbility(int p_AbilityId, USceneComponent* p_ToAttach);

	AActor* UseAbility(int p_AbilityId, AActor* p_ToAttach);

	AActor* UseAbility(int p_AbilityId, FVector p_Position, FRotator p_Rotation);

};