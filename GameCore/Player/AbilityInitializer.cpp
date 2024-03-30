// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilityInitializer.h"

// Sets default values
AAbilityInitializer::AAbilityInitializer()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	m_Root = CreateDefaultSubobject<USceneComponent>(TEXT("Initializer Root"));
	SetRootComponent(m_Root);
}

// Called when the game starts or when spawned
void AAbilityInitializer::BeginPlay()
{
	Super::BeginPlay();

	m_Ability = GetAbilityClass();
}

// Called every frame
void AAbilityInitializer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	UpdateMaxDistanceMeshLocation();
}

void AAbilityInitializer::CreateMesh() {
	if (m_MeshComponent == nullptr) {
		m_MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Initializer Mesh"));
		m_MeshComponent->AttachToComponent(GetRootComponent(), s_InitializerAttachmentRules);
	}
}

UStaticMeshComponent* AAbilityInitializer::SetStaticMesh(UStaticMesh* p_Mesh) {
	CreateMesh();
	m_MeshComponent->SetStaticMesh(p_Mesh);
	return m_MeshComponent;
}

UStaticMeshComponent* AAbilityInitializer::SetMaterial(UMaterialInterface* p_Material) {
	CreateMesh();
	m_MeshComponent->SetMaterial(0, p_Material);
	return m_MeshComponent;
}

UStaticMeshComponent* AAbilityInitializer::SetMeshForMaxDistance(UStaticMesh* p_Mesh, float p_MaxDistance) {
	CreateMesh();
	m_MeshComponent->SetStaticMesh(p_Mesh);
	m_MaxDistance = p_MaxDistance;
	m_UseMeshDistance = true;
	return m_MeshComponent;
}

void AAbilityInitializer::UpdateMaxDistanceMeshLocation() {

	if (m_MeshComponent == nullptr || m_MeshComponent->GetStaticMesh() == nullptr || !m_UseMeshDistance) return;

	FHitResult l_TraceResult;
	TArray<AActor*, FDefaultAllocator> l_ToIgnore;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(),TSubclassOf<AAbility>().Get(), l_ToIgnore);

	FVector l_End = GetActorLocation() + GetActorForwardVector() * m_MaxDistance;

	UKismetSystemLibrary::LineTraceSingle(GetWorld(), GetActorLocation(), l_End,
		TraceTypeQuery1, true, l_ToIgnore, EDrawDebugTrace::None, l_TraceResult, true);

	if (l_TraceResult.bBlockingHit == true) {
		m_MeshComponent->SetWorldLocation(l_TraceResult.ImpactPoint);
	}
	else {
		m_MeshComponent->SetWorldLocation(l_End);
	}
}

void AAbilityInitializer::OnUse() {
	FRotator l_Rotation = GetActorRotation();
	FVector l_Location = GetActorLocation() + (GetActorForwardVector() * 150);
	AAbility* l_Actor = Cast<AAbility>(GetWorld()->SpawnActor(m_Ability.Get(), &l_Location, &l_Rotation));
	//l_Actor->SetActorRotation(l_Rotation);
	//l_Actor->SetActorLocation(l_Location);
	l_Actor->OnUse();
	Destroy();
}

void AAbilityInitializer::SetAbility(TSubclassOf<AAbility> p_Ability) {
	m_Ability = p_Ability; 
}

void AAbilityInitializer::SetScale(FVector p_Scale) {
	m_MeshComponent->SetRelativeScale3D(p_Scale);
}