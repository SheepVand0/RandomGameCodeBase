// Fill out your copyright notice in the Description page of Project Settings.


#include "Ability.h"

// Sets default values
AAbility::AAbility()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AAbility::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AAbility::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AAbility::OnUse() {}

void AAbility::OnAbilityStop() {}

void AAbility::CreateMesh() {
	if (m_MeshComponent == nullptr) {
		m_MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
		m_MeshComponent->SetupAttachment(GetRootComponent());
	}
}

UStaticMeshComponent* AAbility::SetStaticMesh(UStaticMesh* p_Mesh) {
	CreateMesh();
	m_MeshComponent->SetStaticMesh(p_Mesh);
	return m_MeshComponent;
}

UStaticMeshComponent* AAbility::SetMaterial(UMaterialInterface* p_Material) {
	CreateMesh();
	m_MeshComponent->SetMaterial(0, p_Material);
	return m_MeshComponent;
}

void AAbility::OnAbilityBounce(const FHitResult& p_ImpactResult, const FVector& p_ImpactVelocity) {
	m_BounceCount += 1;
	if (m_BounceCount > m_MaxBounceCount) {
		OnAbilityStop();
		Destroy();
	}
}

UProjectileMovementComponent* AAbility::AddMovementComponent(float p_Speed, float p_Gravity, int p_BounceCount) {
	if (m_MovementComponent != nullptr) return m_MovementComponent;
	UProjectileMovementComponent* l_ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Movement comp"));
	l_ProjectileMovement->InitialSpeed = p_Speed;
	l_ProjectileMovement->MaxSpeed = p_Speed;
	l_ProjectileMovement->ProjectileGravityScale = p_Gravity;
	l_ProjectileMovement->OnProjectileBounce.AddDynamic(this, &AAbility::OnAbilityBounce);
	m_MaxBounceCount = p_BounceCount;
	m_MovementComponent = l_ProjectileMovement;
	return m_MovementComponent;
}

void AAbility::SetScale(FVector p_Scale) {
	m_MeshComponent->SetRelativeScale3D(p_Scale);
}