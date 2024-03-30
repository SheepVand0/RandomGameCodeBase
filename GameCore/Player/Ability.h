// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Ability.generated.h"

UCLASS()
class GAMECORE_API AAbility : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AAbility();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY()
		UStaticMeshComponent* m_MeshComponent;

	UPROPERTY()
		UProjectileMovementComponent* m_MovementComponent;

	UPROPERTY()
		int m_BounceCount;

	UPROPERTY()
		int m_MaxBounceCount;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void OnUse();

	void CreateMesh();

	UStaticMeshComponent* SetStaticMesh(UStaticMesh* p_Mesh);

	UStaticMeshComponent* SetMaterial(UMaterialInterface* p_Material);

	UProjectileMovementComponent* AddMovementComponent(float p_Speed, float p_Gravity, int p_BounceCount);

	UFUNCTION()
		void OnAbilityBounce(const FHitResult& p_ImpactResult, const FVector& p_ImpactVelocity);

	virtual void OnAbilityStop();

	void SetScale(FVector p_Scale);

};
