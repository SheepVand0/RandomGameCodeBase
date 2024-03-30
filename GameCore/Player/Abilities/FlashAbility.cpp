// Fill out your copyright notice in the Description page of Project Settings.


#include "FlashAbility.h"

AFlashAbility::AFlashAbility() {

	ConstructorHelpers::FObjectFinder<UStaticMesh>l_Sphere(TEXT("/Script/Engine.StaticMesh'/Engine/BasicShapes/Sphere.Sphere'"));
	ConstructorHelpers::FObjectFinder<UMaterial>l_FlashMaterial(TEXT("/Script/Engine.Material'/GameCore/Materials/Abilities/M_Flash.M_Flash'"));

	SetStaticMesh(l_Sphere.Object);
	SetMaterial(l_FlashMaterial.Object);

	SetScale(FVector(0.4f, 0.4f, 0.4f));

	AddMovementComponent(500, 0.1f, 1);
}

void AFlashAbility::OnUse() {

}

void AFlashAbility::OnAbilityStop() {
	Destroy();
}