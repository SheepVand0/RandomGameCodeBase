// Fill out your copyright notice in the Description page of Project Settings.


#include "DataComponent.h"

// Sets default values for this component's properties
UDataComponent::UDataComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UDataComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UDataComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

FVector UDataComponent::GetVector(FString p_Name) {
	if (!m_Vectors.Contains(p_Name)) return FVector();
	FVector l_Vector = m_Vectors[p_Name];
	return l_Vector;
}

float UDataComponent::GetFloat(FString p_Name) {
	if (!m_Floats.Contains(p_Name)) return 0;
	float l_Value = m_Floats[p_Name];
	return l_Value;
}

FString UDataComponent::GetString(FString p_Name) {
	if (!m_Strings.Contains(p_Name)) return FString();
	FString l_String = m_Strings[p_Name];
	return l_String;
}

void UDataComponent::AddVector(FString p_Name, FVector p_Value) {
	if (m_Vectors.Contains(p_Name)) {
		m_Vectors[p_Name] = p_Value;
	}
	else {
		m_Vectors.Add(p_Name, p_Value);
	}
}

void UDataComponent::AddFloat(FString p_Name, float p_Value) {
	if (m_Floats.Contains(p_Name)) {
		m_Floats[p_Name] = p_Value;
	}
	else {
		m_Floats.Add(p_Name, p_Value);
	}
}

void UDataComponent::AddString(FString p_Name, FString p_Value) {
	if (m_Strings.Contains(p_Name)) {
		m_Strings[p_Name] = p_Value;
	}
	else {
		m_Strings.Add(p_Name, p_Value);
	}
}

void UDataComponent::AddObject(FString p_Name, UObject* p_Value) {
	if (m_Objects.Contains(p_Name)) {
		m_Objects[p_Name] = p_Value;
	}
	else {
		m_Objects.Add(p_Name, p_Value);
	}
}