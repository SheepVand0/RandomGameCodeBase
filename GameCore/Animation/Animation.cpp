// Fill out your copyright notice in the Description page of Project Settings.

#include "Animation.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"

void UAnimationCurve::SortKeysArray() {

	TArray<UAnimationKey*> l_NewArray;

	for (int l_i = 0; l_i < Keys.Num(); l_i++) {
		if (l_i == 0) {
			l_NewArray.Add(Keys[l_i]);
			continue;
		}

		UAnimationKey* l_CurrentKey = Keys[l_i];
		if (l_CurrentKey->Time <= l_NewArray[0]->Time) {
			l_NewArray.Insert(l_CurrentKey, 0);
			continue;
		}
		else if (l_CurrentKey->Time >= l_NewArray.Last()->Time) {
			l_NewArray.Add(l_CurrentKey);
			continue;
		}
		else {
			for (int l_i1 = 0; l_i1 < l_NewArray.Num() - 1; l_i1++) {
				if (l_CurrentKey->Time <= l_NewArray[l_i1 + 1]->Time && l_CurrentKey->Time >= l_NewArray[l_i1]->Time) {
					l_NewArray.Insert(l_CurrentKey, l_i1 + 1);
					break;
				}

				continue;
			}
		}
	}

}

// Sets default values for this component's properties
UAnimation::UAnimation()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UAnimation::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UAnimation::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (!IsPlaying) return;

	float l_CurrentTime = UKismetSystemLibrary::GetGameTimeInSeconds(GetWorld());

	if (l_CurrentTime > StartTime + Duration) {
		Stop();
	}

	for (int l_i = 0; l_i < Curve->Keys.Num(); l_i++) {

		float l_CurrentKeyTime = Curve->Keys[l_i]->Time;

		if (l_CurrentKeyTime > l_CurrentTime - StartTime) {
			if (l_i < Curve->Keys.Num() - 1) {
				if (l_CurrentTime - StartTime > Curve->Keys[l_i + 1]->Time)
					continue;
			}
			else
				continue;

			UAnimationKey* l_Start = Curve->Keys[l_i];
			
			UAnimationKey* l_End = Curve->Keys[l_i + 1];

			float l_CurrentAnimationTime = l_CurrentKeyTime - StartTime - l_Start->Time;

			float l_Prct = l_CurrentAnimationTime / (l_End->Time - l_Start->Time);

			float l_Value = (l_End->Key - l_Start->Key) * l_Prct;

			OnUpdate.Broadcast(l_Value);

			break;
		}
	}
}

void UAnimation::SetCurve(UAnimationCurve* p_Curve) {
	if (IsPlaying) {
		HasPendingCurve = true;
		TempCurve = p_Curve;
	}
	else {
		Curve = p_Curve;
	}
}

void UAnimation::Play() {
	Curve->SortKeysArray();
	Duration = Curve->Length();
	StartTime = UKismetSystemLibrary::GetGameTimeInSeconds(GetWorld());
	IsPlaying = true;
}

void UAnimation::Stop() {
	IsPlaying = false;
	if (HasPendingCurve) {
		Curve = TempCurve;
		HasPendingCurve = false;
	}
}
