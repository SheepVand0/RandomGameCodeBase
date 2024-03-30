// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine.h"
#include "GameCore/Item/C_Item.h"
#include "Camera/CameraComponent.h"
#include "GameCore/Item/Weapons/WeaponProjectile.h"
#include "Components/AudioComponent.h"
#include "Components/TimelineComponent.h"
#include "GameCore/Components/DataComponent.h"
#include "GameCore/Player/C_Player.h"
#include "GameCore/Public/GameCore.h"
#include "GameCore/Core/Constants.h"
#include "C_Weapon.generated.h"

UENUM()
enum class EWeaponSound {
	Fire
};

/**
 *
 */
UCLASS()
class GAMECORE_API AC_Weapon : public AC_Item
{
	GENERATED_BODY()

protected:

///////////////////////////////////////////////////////
//////////////////////////////////////////////////////

	virtual void OnConstruction(const FTransform& Transform) override;

	virtual void OnUse() override;

	virtual void OnStopUse() override;

	virtual void OnItemSelected() override;

	virtual void NotifyEvent(FString p_EventName) override;

	virtual bool IsUsing() override;

	virtual void Tick(float DeltaTime) override;

	virtual void ConstructWeapon(FItemProperties p_Properties,
		USkeletalMesh* p_WeaponMesh, UPhysicsAsset* p_PhysicAsset,
		UMaterialInstance* p_WeaponMaterial, FTransform p_WeaponMeshTransform, int p_Damages, int p_HeadDamagess, int p_Ammo,
		int p_MagazineAmmo, int p_MaxMagazineAmmo, int p_MaxAmmo, float p_FireDuration, bool p_Spammable, float p_SprayMultiplier, float p_SprayTransitionDuration);

	void PostUse();

	void SetMuzzleSocketName(FString p_Name);

	void SetProjectileClass(TSubclassOf<AWeaponProjectile> p_Class);

	///////////////////////////////////////////////////////
	//////////////////////////////////////////////////////
	/// Sound managing

protected:

	UPROPERTY()
	USoundAttenuation* m_WeaponSoundAttenuation1;

	UPROPERTY()
	USoundBase* m_FireSound;

	UPROPERTY()
	UAudioComponent* WeaponAudioComponent;

	void SetSoundPack(USoundBase* p_FireSound);

	void PlayWeaponSound(EWeaponSound p_SoundType);

	///////////////////////////////////////////////////////
	//////////////////////////////////////////////////////
	/// Spray managing

	UPROPERTY()
	UTimelineComponent* m_SprayTimeline;

	UPROPERTY()
	UCurveFloat* m_SprayTimelineCurve;

	FOnTimelineFloat m_OnSprayTimelineCallback;

	FOnTimelineEventStatic m_OnSprayTimelineFinished;

	UPROPERTY()
	int m_SprayIndex = 0;

	UPROPERTY()
	float m_LastSprayTimelineValue;

	UPROPERTY()
	float SprayMutltiplier;

	UPROPERTY()
	float m_SprayTransitionDuration;

	void PostUseForSpray();

	void ResetSprayIndex();

	void AddPitchToOwner(bool p_Reverse = false);

	float GetPitchToAddFromSprayValues(int p_SprayIndexChangesCount, int p_BaseSpray);

	UFUNCTION()
	void SprayTimelineCallback(float p_Val);

	///////////////////////////////////////////////////////
	//////////////////////////////////////////////////////
	/// Ammo Managing

	UPROPERTY(EditAnywhere)
	int m_MaxMagazineAmmo;

	UPROPERTY(EditAnywhere)
	int m_OtherAmmo;

	UPROPERTY(EditAnywhere)
	int m_MagazineAmmo;

	void Reload();

	void OnReloadFinished();

	void OnNeedToUpdatePlayerWidgetAmmo();


public:

	AC_Weapon();

	///////////////////////////////////////////////////////
	//////////////////////////////////////////////////////

	UPROPERTY(EditAnywhere)
		int m_Damages;

	UPROPERTY()
		bool m_Spammable;

	UPROPERTY(EditAnywhere)
		float m_FireDuration;

	UPROPERTY(EditAnywhere)
		bool m_WeaponPaused;

	UPROPERTY()
		int m_HeadDamages;

	UPROPERTY()
		TSubclassOf<AWeaponProjectile> ProjectileClass;

	UPROPERTY(EditAnywhere)
		FTransform m_WeaponMeshTransform;

	///////////////////////////////////////////////////////
	//////////////////////////////////////////////////////

	UPROPERTY(EditAnywhere)
		USkeletalMeshComponent* m_WeaponMesh;

	UPROPERTY(EditAnywhere)
		FName m_MuzzleSocketName;

	///////////////////////////////////////////////////////
	//////////////////////////////////////////////////////

	inline static UStaticMesh* s_Sphere = nullptr;

	///////////////////////////////////////////////////////
	//////////////////////////////////////////////////////

	UFUNCTION()
		AC_Player* GetParentAsPlayer();
	
	/*UFUNCTION()
		void OnSprayTimelineFinished();*/

};
