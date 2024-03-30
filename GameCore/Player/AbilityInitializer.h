// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Kismet/KismetSystemLibrary.h"
#include "GameCore/Item/C_Item.h"
#include "Kismet/GameplayStatics.h"
#include "Ability.h"
#include "AbilityInitializer.generated.h"

UCLASS()
class GAMECORE_API AAbilityInitializer : public AC_Item
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AAbilityInitializer();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	const FAttachmentTransformRules s_InitializerAttachmentRules = FAttachmentTransformRules(EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, false);

	UPROPERTY()
		float m_MaxDistance;

	UPROPERTY()
		USceneComponent* m_Root;

	UPROPERTY()
		UStaticMeshComponent* m_MeshComponent;

	void CreateMesh();

	void UpdateMaxDistanceMeshLocation();

	TSubclassOf<AAbility> m_Ability;

	bool m_UseMeshDistance = false;

public:

	void SetAbility(TSubclassOf<AAbility> p_Ability);

	virtual void OnUse() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UStaticMeshComponent* SetMeshForMaxDistance(UStaticMesh* p_Mesh, float p_Distance);

	UStaticMeshComponent* SetStaticMesh(UStaticMesh* p_Mesh);

	UStaticMeshComponent* SetMaterial(UMaterialInterface* p_Material);

	void SetScale(FVector p_Scale);

	virtual TSubclassOf<AAbility> GetAbilityClass() { return TSubclassOf<AAbility>(); }

};