// Fill out your copyright notice in the Description page of Project Settings.

#include "WeaponProjectile.h"
#include "Components/SphereComponent.h"
#include "Async/Async.h"
#include "Components/TimelineComponent.h"

// Sets default values
AWeaponProjectile::AWeaponProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ConstructorHelpers::FObjectFinder<UStaticMesh>l_Sphere(TEXT("StaticMesh'/Game/StarterContent/Shapes/Shape_Sphere.Shape_Sphere'"));
	ConstructorHelpers::FObjectFinder<UMaterialInstance>l_Mat(TEXT("/Script/Engine.MaterialInstanceConstant'/Game/Assets/Materials/M_Projectile_Orange.M_Projectile_Orange'"));

	m_Speed = 10000;
	m_GravityScale = 0.f;
	ProjectileMaterial = l_Mat.Object;

	ProjectileCollision = CreateDefaultSubobject<USphereComponent>(TEXT("Projectile Collision"));
	SetRootComponent(ProjectileCollision);
	ProjectileCollision->SetSphereRadius(0.2f);
	//ProjectileCollision->SetCollisionObjectType(ECC_WorldStatic);
	ProjectileCollision->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	ProjectileCollision->SetCollisionResponseToAllChannels(ECR_Block);

	ProjectileMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Projectile Mesh"));
	ProjectileMesh->SetupAttachment(ProjectileCollision);
	ProjectileMesh->SetStaticMesh(l_Sphere.Object);
	ProjectileMesh->SetRelativeScale3D(FVector(0.04f, 0.8f, 0.04f));
	ProjectileMesh->SetRelativeRotation(FRotator::MakeFromEuler(FVector(0, 0, 90)));
	ProjectileMesh->SetMaterial(0, ProjectileMaterial);
	ProjectileMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);

	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Projectile Movement"));
	ProjectileMovement->InitialSpeed = m_Speed;
	ProjectileMovement->ProjectileGravityScale = m_GravityScale;
	ProjectileMovement->OnProjectileStop.AddDynamic(this, &AWeaponProjectile::DefaultOnProjectileStop);

	ConstructorHelpers::FObjectFinder<UParticleSystem>l_TrailParticle(TEXT("/Script/Engine.ParticleSystem'/GameCore/Particles/P_Trail.P_Trail'"));

	TrailComponent = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Projectile Trail"));
	TrailComponent->SetupAttachment(ProjectileMesh);
	TrailComponent->Template = l_TrailParticle.Object;
}

// Called when the game starts or when spawned
void AWeaponProjectile::BeginPlay()
{
	Super::BeginPlay();

	TrailComponent->BeginTrails(TEXT("TrailStart"), TEXT("TrailEnd"), ETrailWidthMode_FromSecond, 1);
}

void AWeaponProjectile::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);

	if (m_SpawnTime == -1) {
		m_SpawnTime = GetWorld()->GetTimeSeconds();
	}

	if (10 < GetWorld()->GetTimeSeconds() - m_SpawnTime) {
		Destroy();
	}
}

void AWeaponProjectile::SetPlayerOwner(ACharacter* p_Player) {
	m_OwnerPlayer = p_Player;
}

void AWeaponProjectile::SetDamages(int p_Damages) {
	m_Damages = p_Damages;
}

void AWeaponProjectile::DefaultOnProjectileStop(const FHitResult& p_Results) {
	OnHit(p_Results);
	Destroy();
}

void AWeaponProjectile::OnHit(FHitResult p_Result) {
	if (m_OwnerPlayer == nullptr) {
		GEngine->AddOnScreenDebugMessage(1, 1.0f, FColor::Red, FString("Weapon projectile need to have a parent player / bot"), true, FVector2D(5, 5));
		return;
	}

	//l_HitPlayer->Destroy();

	UActorComponent* l_HitComponent = p_Result.Component.Get();

	if (p_Result.GetActor() == nullptr) return;

	UHealthComponent* l_HealthComponent = Cast<UHealthComponent>(p_Result.GetActor()->GetComponentByClass(TSubclassOf<UHealthComponent>()));

	if (l_HealthComponent == nullptr) return;

	//GEngine->AddOnScreenDebugMessage(2, 1.0f, FColor::Green, l_HitComponent == nullptr ? "True" : "False");
	if (l_HitComponent == nullptr) {
		l_HealthComponent->ApplyDamages(m_Damages);
		return;
	}

	if (l_HitComponent->ComponentTags.Contains(FName("HeadComponent"))) {
		//GEngine->AddOnScreenDebugMessage(1, 1.0f, FColor::Green, FString("Head hitted"));
		l_HealthComponent->ApplyDamages(m_HeadDamages);
		return;
	}
	else {
		l_HealthComponent->ApplyDamages(m_Damages);
		return;
	}
}
