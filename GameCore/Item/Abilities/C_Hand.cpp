// Fill out your copyright notice in the Description page of Project Settings.


#include "C_Hand.h"
#include "Kismet/KismetSystemLibrary.h"
#include "GameCore/Player/C_Player.h"

AC_Hand::AC_Hand() {
	Construct(AC_Hand::GetPropertiesCopy(), FTransform(), nullptr, nullptr, false, FItemRarity::Legendary);
	m_ItemProperties.RemovableFromInventory = false;

	FRichCurve l_Curve = FRichCurve{};

	l_Curve.AddKey(0, 0);
	l_Curve.AddKey(0.25f, 100);
	l_Curve.AddKey(0.5f, 0);

	Curve = CreateDefaultSubobject<UCurveFloat>(TEXT("Curve"));
	Curve->FloatCurve = l_Curve;

	FOnTimelineFloat l_Update;
	l_Update.BindDynamic(this, &AC_Hand::AnimUpdate);

	Timeline.SetFloatCurve(Curve, FName("Value"));
	Timeline.AddInterpFloat(Curve, l_Update);

	Timeline.SetTimelineLength(0.5f);

}

void AC_Hand::OnUse() {
	FHitResult l_Results = FHitResult{};
	UKismetSystemLibrary::LineTraceSingle(
		GetWorld(),
		GetActorLocation(), GetActorLocation() + (GetActorForwardVector() * 500),
		TraceTypeQuery1,
		true,
		TArray<AActor*>{},
		EDrawDebugTrace::None,
		l_Results,
		true);

	OriginalHandLocation = Cast<AC_Player>(m_ParentPlayer)->RightHand->GetRelativeLocation();

	Timeline.PlayFromStart();

	AC_Player* l_Player = Cast<AC_Player>(l_Results.GetActor());
	if (l_Player == nullptr) return;

	l_Player->HealthComponent->ApplyDamages(20);
}

void AC_Hand::OnStopUse() {

}

void AC_Hand::AnimUpdate(float p_Value) {
	GEngine->AddOnScreenDebugMessage(3, 10, FColor::White, FString::SanitizeFloat(p_Value));
	Cast<AC_Player>(m_ParentPlayer)->RightHand->SetRelativeLocation(FVector(OriginalHandLocation.X, OriginalHandLocation.Y + p_Value, OriginalHandLocation.Y));
}