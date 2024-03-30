// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
//#include "GameCore/Player/C_Player.h"
#include <Delegates/Delegate.h>
#include <Kismet/KismetMathLibrary.h>
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/SphereComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "GameCore/Components/HealthComponent.h"
#include "WeaponProjectile.generated.h"

using namespace UE::Math;

UCLASS()
class GAMECORE_API AWeaponProjectile : public AActor
{
	GENERATED_BODY()

protected:

	UPROPERTY()
		float m_Speed;

	UPROPERTY()
		float m_GravityScale;

public:
	// Sets default values for this actor's properties
	AWeaponProjectile();

	UPROPERTY(BlueprintReadOnly)
	    UStaticMeshComponent* ProjectileMesh;

	UPROPERTY()
		UProjectileMovementComponent* ProjectileMovement;

	UPROPERTY()
		USphereComponent* ProjectileCollision;

	UPROPERTY()
		UParticleSystemComponent* TrailComponent;

	UPROPERTY(BlueprintReadOnly)
		ACharacter* m_OwnerPlayer;

	UPROPERTY(BlueprintReadOnly)
		int m_Damages;

	UPROPERTY(BlueprintReadOnly)
		int m_HeadDamages;

	UPROPERTY()
		UMaterialInstance* ProjectileMaterial;

	UFUNCTION()
		void DefaultOnProjectileStop(const FHitResult& p_Results);

	UFUNCTION()
		virtual void OnHit(FHitResult p_Result);

	UFUNCTION()
		void SetDamages(int p_Damages);

	UFUNCTION()
		void SetPlayerOwner(ACharacter* p_Player);


	void (*OnProjectileStop)(AWeaponProjectile*, ACharacter*, FHitResult);

	static AWeaponProjectile* CreateProjectile(TSubclassOf<AWeaponProjectile> ProjectileClass, UWorld* p_World, ACharacter* p_OwnerPlayer, int p_Damages, int p_HeadDamages, FVector p_Location, FRotator p_Rotation) {
		FActorSpawnParameters l_SpawnParams;
		l_SpawnParams.bNoFail = true;
		AWeaponProjectile* l_Projectile = Cast<AWeaponProjectile>(p_World->SpawnActor(ProjectileClass.Get(), &p_Location,  &p_Rotation, l_SpawnParams));
		if (p_OwnerPlayer != nullptr)
			l_Projectile->SetPlayerOwner(p_OwnerPlayer);
		l_Projectile->m_Damages = p_Damages;
		l_Projectile->m_HeadDamages = p_HeadDamages;
		return l_Projectile;
	}

protected:

	float m_SpawnTime = -1;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

};
