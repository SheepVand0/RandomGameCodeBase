// Fill out your copyright notice in the Description page of Project Settings.


#include "MapPoint.h"

// Sets default values
AMapPoint::AMapPoint()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AMapPoint::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMapPoint::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}