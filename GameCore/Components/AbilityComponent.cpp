// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilityComponent.h"

// Sets default values for this component's properties
UAbilityComponent::UAbilityComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	EventComponent = CreateDefaultSubobject<UEventComponent>(TEXT("Event component"));
}

// Called when the game starts
void UAbilityComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UAbilityComponent::SetAbility(TArray<FAbilityModel> p_Abilities) {
	for (int l_i = 0; l_i < p_Abilities.Num(); l_i++)
		p_Abilities[l_i].AbilityId = l_i;
	m_Abilities = p_Abilities;
}

AActor* UAbilityComponent::SpawnAbility(int p_AbilityIndex) {
	if (!m_Abilities.IsValidIndex(p_AbilityIndex)) return nullptr;
	if (m_Abilities[p_AbilityIndex].CurrentStack == 0) return nullptr;
	FAbilityModel l_Ability = m_Abilities[p_AbilityIndex];
	AActor* l_Actor = nullptr;
	if (l_Ability.UseInitializer) {
		l_Actor = GetWorld()->SpawnActor(l_Ability.AbilityInitializerClass.Get());
	}
	else {
		l_Actor = GetWorld()->SpawnActor(l_Ability.AbilityClass.Get());
	}
	return l_Actor;
}

void UAbilityComponent::UseAbility(int p_AbilityId, UInventoryComponent* p_Inventory) {
	if (!m_Abilities.IsValidIndex(p_AbilityId)) { GEngine->AddOnScreenDebugMessage(1, 1.0f, FColor::Red, FString("Not valid ability")); return; }
	FAbilityModel l_Model = m_Abilities[p_AbilityId];
	FTransform l_Transform;
	l_Transform.SetRotation(FQuat::MakeFromEuler(FVector(0, 0, 90)));
	p_Inventory->SelectInventoryItemFromProperties(FItemProperties::CreateItemProperties(l_Model.AbilityInitializerClass, FString("Ability"), FString("Ability"), l_Transform, false, nullptr));
}

AActor* UAbilityComponent::UseAbility(int p_AbilityId, USceneComponent* p_ToAttach) {
	AActor* l_AbilityActor = SpawnAbility(p_AbilityId);
	if (l_AbilityActor == nullptr) return nullptr;
	l_AbilityActor->AttachToComponent(p_ToAttach, s_AttachmentRules);
	return l_AbilityActor;
}

AActor* UAbilityComponent::UseAbility(int p_AbilityId, AActor* p_ToAttach) {
	AActor* l_AbilityActor = SpawnAbility(p_AbilityId);
	if (l_AbilityActor == nullptr) return nullptr;
	l_AbilityActor->AttachToActor(p_ToAttach, s_AttachmentRules);
	return l_AbilityActor;
}

AActor* UAbilityComponent::UseAbility(int p_AbilityId, FVector p_Location, FRotator p_Rotation) {
	AActor* l_AbilityActor = SpawnAbility(p_AbilityId);
	if (l_AbilityActor == nullptr) return nullptr;
	l_AbilityActor->SetActorLocation(p_Location);
	l_AbilityActor->SetActorRotation(p_Rotation);
	return l_AbilityActor;
}
