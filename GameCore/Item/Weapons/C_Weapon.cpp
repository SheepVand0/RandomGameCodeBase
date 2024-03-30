// Fill out your copyright notice in the Description page of Project Settings.

#include "C_Weapon.h"
#include <iostream>
#include "UObject/ConstructorHelpers.h"

AC_Weapon::AC_Weapon() {
	PrimaryActorTick.bCanEverTick = true;

	m_WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Weapon Mesh"));
	FAttachmentTransformRules l_AttachmentRules = FAttachmentTransformRules::FAttachmentTransformRules(EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, false);
	m_WeaponMesh->AttachToComponent(GetRootComponent(), l_AttachmentRules);

	WeaponAudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("Weapon Audio"));
	WeaponAudioComponent->AttachToComponent(GetRootComponent(), l_AttachmentRules);

	m_SprayTimeline = CreateDefaultSubobject<UTimelineComponent>(TEXT("Spray Timeline"));
	m_SprayTimeline->CreationMethod = EComponentCreationMethod::UserConstructionScript;
	m_SprayTimeline->SetNetAddressable();
	m_SprayTimeline->SetPropertySetObject(this);
	m_SprayTimeline->SetDirectionPropertyName(FName("TimelineDirection"));
	m_SprayTimeline->SetLooping(false);

	m_MuzzleSocketName = FName("Muzzle");

	//Construct(FItemProperties::CreateItemProperties(this->GetClass(), FString("Default Weapon"), false, nullptr), nullptr, m_WeaponMesh, true, FItemRarity::Normal);
	ConstructorHelpers::FObjectFinder<UStaticMesh>l_Sphere(TEXT("StaticMesh'/Game/StarterContent/Shapes/Shape_Sphere.Shape_Sphere'"));
	ConstructorHelpers::FObjectFinder<USoundAttenuation>l_Attenuation1(TEXT("/Script/Engine.SoundAttenuation'/GameCore/Weapons/Sounds/A_Weapon1.A_Weapon1'"));
	s_Sphere = l_Sphere.Object;
	m_WeaponSoundAttenuation1 = l_Attenuation1.Object;

	WeaponAudioComponent->AttenuationSettings = m_WeaponSoundAttenuation1;

	m_OnSprayTimelineCallback.BindUFunction(this, FName{ TEXT("SprayTimelineCallback") });
	//m_OnSprayTimelineFinished.BindUFunction(this, FName{ TEXT("OnSprayTimelineFinished") });

	m_SprayTimeline->RegisterComponent();
}

void AC_Weapon::OnConstruction(const FTransform& Transform) {
	m_WeaponMesh->SetRelativeTransform(m_WeaponMeshTransform);
}

void AC_Weapon::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);

	if (m_SprayTimeline != nullptr) {
		m_SprayTimeline->TickComponent(DeltaTime, ELevelTick::LEVELTICK_TimeOnly, nullptr);
	}
}

void AC_Weapon::ConstructWeapon(FItemProperties p_Properties,
	USkeletalMesh* p_WeaponMesh, UPhysicsAsset* p_PhysicAsset,
	UMaterialInstance* p_WeaponMaterial, FTransform p_WeaponMeshTransform, int p_Damages, int p_HeadDamages, int p_Ammo, int p_MagazineAmmo,
	int p_MaxMagazineAmmo, int p_MaxAmmo, float p_FireDuration, bool p_Spammable, float p_SprayMultiplier, float p_SprayTransitionDuration) {
	///--------------------------------------------------------
	m_WeaponMesh->SetupAttachment(GetRootComponent());
	m_WeaponMesh->SetSkeletalMesh(p_WeaponMesh);
	m_WeaponMesh->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
	m_WeaponMesh->SetRelativeTransform(p_WeaponMeshTransform, false);
	m_WeaponMesh->SetPhysicsAsset(p_PhysicAsset, false);
	m_WeaponMesh->SetCollisionObjectType(ECC_WorldDynamic);
	m_WeaponMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Block);
	m_WeaponMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Block);
	m_ItemCollision->AttachToComponent(m_WeaponMesh, FAttachmentTransformRules::FAttachmentTransformRules(EAttachmentRule::KeepRelative, true));
	if (p_WeaponMaterial != nullptr)
		m_WeaponMesh->SetMaterial(0, p_WeaponMaterial);
	m_Damages = p_Damages;
	m_HeadDamages = p_HeadDamages;
	m_MaxMagazineAmmo = p_MaxMagazineAmmo;
	m_OtherAmmo = p_MaxAmmo;
	m_MagazineAmmo = p_MagazineAmmo;
	FTransform l_Transform;
	l_Transform.TransformPosition(FVector(0, 0, 0));
	l_Transform.TransformRotation(FQuat::MakeFromEuler(FVector(0, 0, 0)));
	l_Transform.SetScale3D(FVector(1, 4, 1));
	Construct(p_Properties, l_Transform, nullptr, m_WeaponMesh, false, FItemRarity::Normal);
	m_FireDuration = p_FireDuration;
	m_Spammable = p_Spammable;
	SprayMutltiplier = p_SprayMultiplier;
	m_SprayTransitionDuration = p_SprayTransitionDuration;

	UCurveFloat* l_FloatCurve = CreateDefaultSubobject<UCurveFloat>(TEXT("Curve"));
	//l_FloatCurve->FloatCurve = l_RichCurve;
	m_SprayTimelineCurve = l_FloatCurve;
	m_SprayTimeline->AddInterpFloat(l_FloatCurve, m_OnSprayTimelineCallback);
	m_SprayTimeline->SetTimelineFinishedFunc(m_OnSprayTimelineFinished);
}

////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////

void AC_Weapon::SetProjectileClass(TSubclassOf<AWeaponProjectile> p_Class) {
	ProjectileClass = p_Class;
}

void AC_Weapon::SetMuzzleSocketName(FString p_Value) {
	m_MuzzleSocketName = *p_Value;
}

AC_Player* AC_Weapon::GetParentAsPlayer() {
	return Cast<AC_Player>(m_ParentPlayer);
}

float AC_Weapon::GetPitchToAddFromSprayValues(int p_SprayChangesCount, int p_BaseSpray) {
	float l_Value = 0;
	for (int l_i = p_BaseSpray; l_i < p_SprayChangesCount; l_i++) {
		if (l_i == 0 || l_i  == 4) continue;

		l_Value += SprayMutltiplier * (l_i) / 8;
	}

	return l_Value;
}

////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////

void AC_Weapon::OnUse() {
	if (m_WeaponPaused) return;

	if (m_MagazineAmmo == 0) { Reload(); return; }

	m_SprayTimeline->Stop();

	UCameraComponent* l_PlayerCam = m_ParentPlayer->GetComponentByClass<UDataComponent>()->GetObject<UCameraComponent>(FString("FPSCamera"));

	const USkeletalMeshSocket* l_Socket = m_WeaponMesh->GetSocketByName(m_MuzzleSocketName);
	if (l_Socket == nullptr) {
		GEngine->AddOnScreenDebugMessage(1, 2.0f, FColor(1, 0, 0), FString("No muzzle for weapon : ") + FString(m_ItemProperties.ItemName));
		return;
	}

	FTransform l_TargetObjectTransform = (m_WeaponMesh->GetSocketByName(m_MuzzleSocketName) != nullptr) ?
		m_WeaponMesh->GetSocketTransform(m_MuzzleSocketName) : GetActorTransform();

	m_WeaponPaused = true;

	PlayWeaponSound(EWeaponSound::Fire);

	TArray<AActor*, FDefaultAllocator> l_Actors;
	l_Actors.Add(m_ParentPlayer);
	l_Actors.Add(this);
	FHitResult l_Result;
	UKismetSystemLibrary::LineTraceSingle(GetWorld(), 
		l_PlayerCam->GetComponentLocation(), 
		l_PlayerCam->GetComponentLocation() + l_PlayerCam->GetForwardVector() * 2000, 
		ETraceTypeQuery::TraceTypeQuery1, true, l_Actors, EDrawDebugTrace::None, l_Result, true);

	FRotator l_RotationCameraToTarget = UKismetMathLibrary::FindLookAtRotation(l_TargetObjectTransform.GetLocation(),
		l_Result.bBlockingHit ? l_Result.ImpactPoint : l_Result.TraceEnd);
	
	float l_Velocity = ((m_ParentPlayer->GetVelocity().X + m_ParentPlayer->GetVelocity().Y) / 2) + (m_ParentPlayer->GetCharacterMovement()->IsFalling() ? 20 : 0);

	l_RotationCameraToTarget.Add(FMath::RandRange(FMath::Clamp(-l_Velocity, -15, 15), FMath::Clamp(l_Velocity, -15, 15)), FMath::RandRange(FMath::Clamp(-l_Velocity, -15, 15), FMath::Clamp(l_Velocity, -15, 15)), 0);

	l_RotationCameraToTarget.Add(FMath::RandRange(FMath::Clamp(-m_SprayIndex, -7, 7), FMath::Clamp(m_SprayIndex, -7, 7)), FMath::RandRange(FMath::Clamp(-m_SprayIndex / 2, -7, 7), FMath::Clamp(m_SprayIndex / 2, -7, 7)), 0);

	AWeaponProjectile::CreateProjectile(ProjectileClass, GetWorld(), static_cast<AC_Player*>(m_ParentPlayer),
		m_Damages, m_HeadDamages,
		l_TargetObjectTransform.GetLocation(), l_RotationCameraToTarget);

	m_MagazineAmmo -= 1;
	GetParentAsPlayer()->SetWidgetMagazineAmmo(m_MagazineAmmo);
	
	FTimerHandle l_TimeHandle;
	GetWorldTimerManager().SetTimer(l_TimeHandle, this, &AC_Weapon::PostUse, m_FireDuration);

	AddPitchToOwner();

	if (m_SprayIndex < 4)
		m_SprayIndex += 1;
}

void AC_Weapon::PostUse() {
	m_WeaponPaused = false;

	if (m_Spammable == true) {
		if (Super::IsUsing())
			Use();
	}

	FTimerHandle l_TimeHandle2;
	GetWorldTimerManager().SetTimer(l_TimeHandle2, this, &AC_Weapon::PostUseForSpray, m_FireDuration + m_SprayTransitionDuration);
}

void AC_Weapon::PostUseForSpray() {
	if (m_Spammable) {
		if (Super::IsUsing()) return;

		AddPitchToOwner(true);
	}
	else {
		if (m_WeaponPaused) return;

		AddPitchToOwner(true);
	}
}

void AC_Weapon::OnStopUse() {
	//m_WeaponPaused = true;
}

////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////

void AC_Weapon::AddPitchToOwner(bool p_Reverse) {
	if (m_SprayIndex == 0 || (!p_Reverse && m_SprayIndex == 4)) return;

	m_SprayTimeline->SetTimelineLength(m_SprayTransitionDuration);
	FRichCurve l_Curve;
	l_Curve.UpdateOrAddKey(0, 0);
	l_Curve.UpdateOrAddKey((p_Reverse) ? m_SprayTransitionDuration * 8 : m_SprayTransitionDuration, 
		p_Reverse ? ( -1 * GetPitchToAddFromSprayValues(m_SprayIndex, 0)) : (SprayMutltiplier * m_SprayIndex / 8));
	m_SprayTimeline->SetTimelineLengthMode(ETimelineLengthMode::TL_LastKeyFrame);
	m_SprayTimeline->SetPlaybackPosition(0.0f, true, false);
	
	m_SprayTimelineCurve->FloatCurve = l_Curve;
	m_LastSprayTimelineValue = 0;

	m_SprayTimeline->PlayFromStart();
	if (p_Reverse)
		m_SprayIndex = 0;
}

void AC_Weapon::SprayTimelineCallback(float p_Val) {
	float l_LastSprayTimelineValue;
	if (p_Val >= 0) {
		l_LastSprayTimelineValue = -1 * m_LastSprayTimelineValue;
	}
	else {
		l_LastSprayTimelineValue = m_LastSprayTimelineValue;
	}
	m_ParentPlayer->AddControllerPitchInput(((-1*p_Val) - l_LastSprayTimelineValue));
	m_LastSprayTimelineValue = p_Val;
}

void AC_Weapon::ResetSprayIndex() {
	GEngine->AddOnScreenDebugMessage(1, 1, FColor(255, 0, 0), FString("Reseting spray"));
	if (IsUsing()) return;
	GEngine->AddOnScreenDebugMessage(2, 1, FColor(255, 0, 0), FString("Sucess"));
	AddPitchToOwner(true);
	m_SprayIndex = 0;
}

void AC_Weapon::OnItemSelected() {
	AC_Player* l_Player = Cast<AC_Player>(m_ParentPlayer);
	if (l_Player != nullptr) {
		l_Player->ShowWidgetAmmoBox();
		OnNeedToUpdatePlayerWidgetAmmo();
	}
}

void AC_Weapon::Reload() {
	/*UPhysicsAsset* l_PhysicsAsset = m_WeaponMesh->GetPhysicsAsset();
	if (l_PhysicsAsset != nullptr) {
		int l_BoneIndex = m_WeaponMesh->GetBoneIndex("Magazine");

		TArray<FBodyInstance*> l_Bodies = m_WeaponMesh->Bodies;
		for (int l_i = 0; l_i < l_Bodies.Num();l_i++) {
			TObjectPtr<USkeletalBodySetup> l_Current = l_PhysicsAsset->SkeletalBodySetups[l_i];
			FString l_Name;
			l_Current->BoneName.ToString(l_Name);
			GEngine->AddOnScreenDebugMessage(1, 1.0f, FColor::Green, l_Name);
			if (l_Current->BoneName == "Magazine") {
				l_Bodies[l_i]->SetInstanceSimulatePhysics(true);
			}
		}
	}*/

	if (m_OtherAmmo + m_MagazineAmmo < m_MaxMagazineAmmo) {
		m_MagazineAmmo += m_OtherAmmo;
		m_OtherAmmo = 0;

		OnNeedToUpdatePlayerWidgetAmmo();
		return;
	}

	int l_AmmoToRemove = m_MaxMagazineAmmo - m_MagazineAmmo;
	m_MagazineAmmo = m_MaxMagazineAmmo;
	m_OtherAmmo -= l_AmmoToRemove;

	OnNeedToUpdatePlayerWidgetAmmo();
}

void AC_Weapon::OnReloadFinished() {

}

void AC_Weapon::OnNeedToUpdatePlayerWidgetAmmo() {
	AC_Player* l_Player = GetParentAsPlayer();
	if (l_Player == nullptr) return;

	l_Player->SetWidgetMagazineAmmo(m_MagazineAmmo);
	l_Player->SetWidgetMaxMagazineAmmo(m_MaxMagazineAmmo);
	l_Player->SetWidgetOtherAmmo(m_OtherAmmo);
}

void AC_Weapon::NotifyEvent(FString p_EventName) {

	if (p_EventName == RELOAD_EVENT) {
		Reload();
	}
}

void AC_Weapon::SetSoundPack(USoundBase* p_FireSound) {

	if (p_FireSound != nullptr)
		m_FireSound = p_FireSound;
}

void AC_Weapon::PlayWeaponSound(EWeaponSound p_SoundType) {
	switch (p_SoundType)
	{
		case EWeaponSound::Fire:
			if (m_FireSound == nullptr) return;
			WeaponAudioComponent->Sound = m_FireSound;
			WeaponAudioComponent->SetPitchMultiplier(FMath::RandRange(0.95f, 1.05f));
			break;
		default: return;
	}
	WeaponAudioComponent->Play();
}

bool AC_Weapon::IsUsing() {
	bool l_Using = Super::IsUsing();
	return l_Using && !m_WeaponPaused;
}