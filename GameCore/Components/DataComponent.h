// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "DataComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class GAMECORE_API UDataComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UDataComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	TMap<FString, FVector> m_Vectors;

	TMap<FString, float> m_Floats;

	TMap<FString, FString> m_Strings;

	TMap<FString, UObject*> m_Objects;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	FVector GetVector(FString p_Name);
	
	float GetFloat(FString p_Name);

	FString GetString(FString p_Name);
	
	void AddVector(FString p_Name, FVector p_Value);

	void AddFloat(FString p_Name, float p_Value);

	void AddString(FString p_Name, FString p_Value);
	
	void AddObject(FString p_Name, UObject* p_Object);

	template<typename t_ObjectType = UObject>
	t_ObjectType* GetObject(FString p_Name) {
		if (!m_Objects.Contains(p_Name)) return nullptr;
		UObject* l_Object = m_Objects[p_Name];
		if (!l_Object->IsA<t_ObjectType>()) return nullptr;
		return (t_ObjectType*)l_Object;
	}
};
