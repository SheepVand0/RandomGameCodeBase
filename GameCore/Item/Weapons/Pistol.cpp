// Fill out your copyright notice in the Description page of Project Settings.


#include "Pistol.h"

APistol::APistol() {

	ConstructorHelpers::FObjectFinder<UStaticMesh>l_ProjectileMesh(TEXT("/Script/CoreUObject.Class'/Script/GameCore.WeaponProjectile'"));
	ConstructorHelpers::FObjectFinder<USkeletalMesh>l_PistolMesh(TEXT("/Script/Engine.SkeletalMesh'/GameCore/Weapons/Pistol/Mesh/SK_Pistol.SK_Pistol'"));
	ConstructorHelpers::FObjectFinder<UPhysicsAsset>l_PistolPhysics(TEXT("/Script/Engine.PhysicsAsset'/GameCore/Weapons/Pistol/Mesh/SK_Pistol_PhysicsAsset.SK_Pistol_PhysicsAsset'"));
	ConstructorHelpers::FObjectFinder<UMaterialInstance>l_PistolMaterial(TEXT("/Script/Engine.MaterialInstanceConstant'/GameCore/Weapons/Pistol/Materials/MI_Weapon_Pistol.MI_Weapon_Pistol'"));
	ConstructorHelpers::FObjectFinder<USoundWave>l_RifleSound(TEXT("/Script/Engine.SoundWave'/GameCore/Weapons/Sounds/FireSound2.FireSound2'"));

	FTransform l_HandedTransform = FTransform();
	FTransform l_WeaponMeshTransform = FTransform();

	SetMuzzleSocketName("Muzzle");

	ConstructWeapon(
		FItemProperties::CreateItemProperties(StaticClass(), FString("Pistol"), PISTOLS_CATEGORY, l_HandedTransform, false, nullptr),
		l_PistolMesh.Object, l_PistolPhysics.Object, l_PistolMaterial.Object
		, l_WeaponMeshTransform, 12, 96, 72, 18, 18, 72, 0.05f, false, 0.2f, 0.1f
	);

	SetProjectileClass(AWeaponProjectile::StaticClass());

	SetSoundPack(l_RifleSound.Object);
}