// Fill out your copyright notice in the Description page of Project Settings.


#include "FlashInitializer.h"

AFlashInitializer::AFlashInitializer() {

	ConstructorHelpers::FObjectFinder<UStaticMesh>l_Sphere(TEXT("/Script/Engine.StaticMesh'/Game/StarterContent/Architecture/Wall_Door_400x300.Wall_Door_400x300'"));
	ConstructorHelpers::FObjectFinder<UMaterial>l_FlashMaterial(TEXT("/Script/Engine.Material'/GameCore/Materials/Abilities/M_Flash.M_Flash'"));

	SetStaticMesh(l_Sphere.Object);
	SetMaterial(l_FlashMaterial.Object);
	SetScale(FVector(0.4f, 0.4f, 0.4f));
	//SetAbility();
}