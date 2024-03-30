// Fill out your copyright notice in the Description page of Project Settings.

#include "Rifle.h"
#include <UObject/ConstructorHelpers.h>

ARifle::ARifle() {
	ConstructorHelpers::FObjectFinder<USkeletalMesh>l_RifleMesh(TEXT("SkeletalMesh'/GameCore/Weapons/Rifle/Mesh/SK_Rifle.SK_Rifle'"));
	ConstructorHelpers::FObjectFinder<UMaterialInstanceConstant>l_RifleMaterial(TEXT("MaterialInstanceConstant'/GameCore/Weapons/Rifle/Materials/MI_Weapon_Rifle.MI_Weapon_Rifle'"));
	ConstructorHelpers::FObjectFinder<UPhysicsAsset>l_RiflePhysicsAsset(TEXT("PhysicsAsset'/GameCore/Weapons/Rifle/Mesh/SK_Rifle_PhysicsAsset.SK_Rifle_PhysicsAsset'"));
	ConstructorHelpers::FObjectFinder<USoundWave>l_RifleSound(TEXT("/Script/Engine.SoundWave'/GameCore/Weapons/Sounds/FireSound2.FireSound2'"));

	FTransform l_Transform;
	FTransform l_HandedTransform = *new FTransform();
	l_HandedTransform.SetRotation(FQuat::MakeFromEuler(FVector3d(0, 0, 0)));
	l_HandedTransform.SetScale3D(FVector3d(1, 1, 1));

	SetMuzzleSocketName("Muzzle");

	ConstructWeapon(FItemProperties::CreateItemProperties(StaticClass(),
		FString("Rifle"),
		FString(MAIN_WEAPONS_CATEGORY),
		l_HandedTransform,
		true,
		nullptr),
		l_RifleMesh.Object,
		l_RiflePhysicsAsset.Object,
		l_RifleMaterial.Object,
		l_Transform,
		25,
		174,
		54,
		27,
		27,
		258,
		0.1f,
		true,
		0.1f, 0.08f);

	SetProjectileClass(AWeaponProjectile::StaticClass());

	SetSoundPack(l_RifleSound.Object);
}
