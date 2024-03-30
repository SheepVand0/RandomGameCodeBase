// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "MotionControllerComponent.h"
#include "GameCore/Item/C_Item.h"
#include "Camera/CameraComponent.h"
#include "Structs/PlayerSettings.h"
#include "Perception/PawnSensingComponent.h"
#include "GameCore/UI/UI_Player.h"
#include "Navigation/PathFollowingComponent.h"
#include "GameCore/Components/HealthComponent.h"
#include "GameCore/Components/InventoryComponent.h"
#include "GameCore/Components/AbilityComponent.h"
#include "GameCore/Player/Abilities/FlashInitializer.h"
#include "GameCore/Player/Abilities/FlashAbility.h"
#include "UObject/ObjectMacros.h"
#include "UObject/WeakObjectPtr.h"
#include "Engine/EngineTypes.h"
#include "Components/ActorComponent.h"
#include "EngineDefines.h"
#include "AIController.h"
#include "GameCore/Player/C_BotController.h"
#include "GameCore/AI/MapPoint.h"
#include "GameCore/Core/GameManager.h"
#include "GameCore/Core/Constants.h"
#include "C_Player.generated.h"

UCLASS()
class GAMECORE_API UEnnemyTeam : public UObject {

	GENERATED_BODY()

public:

	UEnnemyTeam() {};

	UEnnemyTeam(int p_TeamMembersCount, int p_TeamIndex) {
		TeamMembersCount = p_TeamMembersCount;
		TeamIndex = p_TeamIndex;
	};

	UPROPERTY()
		int TeamMembersCount;

	UPROPERTY()
		int TeamIndex;
};

USTRUCT()
struct FEnnemiesNumberData {

	GENERATED_BODY()

public:
	TArray<UEnnemyTeam*> Ennemies;

};

UENUM()
enum class EMainHand {
	Right, Left
};

UCLASS()
class GAMECORE_API AC_Player : public ACharacter
{
	GENERATED_BODY()

private:

	UPROPERTY()
		float m_DefaultSpeed;

	UInputComponent* m_PlayerInputComponent;
protected:
	UPROPERTY()
		float m_SprintMultiplier;

	UPROPERTY()
		bool m_IsSprinting;

	UPROPERTY(EditAnywhere)
		FTransform m_HeadTransform;

	UPROPERTY(EditAnywhere)
		FTransform m_BodyTransform;

	UPROPERTY(EditAnywhere)
		UUI_Player* m_PlayerWidget;

	UFUNCTION()
		void UpdateMainPlayerWidget();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void LookX(float p_Value);

	virtual void LookY(float p_Value);

	virtual void MoveForward(float p_value);

	void MoveRight(float p_value);


public:
	// Sets default values for this character's properties
	AC_Player();

	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnBlueprintSetup);

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void OnConstruction(const FTransform& Transform) override;

	//////////////////////////////////////////////////////////////////

	UFUNCTION()
		AGameManager* GetGameManager();

	void ResetPlayer(bool p_ResetInventory, bool p_KeepShield);

	//////////////////////////////////////////////////////////////////

	UPROPERTY()
		bool IsInVr;

	//////////////////////////////////////////////////////////////////

	UPROPERTY(EditAnywhere)
		USceneComponent* HandInventoryItem;

	UPROPERTY(EditAnywhere)
		UMotionControllerComponent* RightVrHand;

	UPROPERTY(EditAnywhere)
		UMotionControllerComponent* LeftVrHand;

	UPROPERTY(EditAnywhere)
		USkeletalMeshComponent* RightHand;

	UPROPERTY(EditAnywhere)
		USkeletalMeshComponent* LeftHand;

	UPROPERTY(EditAnywhere)
		UCameraComponent* FpsCamera;

	UPROPERTY(EditAnywhere)
		UPawnSensingComponent* PawnSensing;

	UPROPERTY(EditAnywhere)
		USkeletalMeshComponent* PlayerMesh;

	UPROPERTY()
		UHealthComponent* HealthComponent;

	UPROPERTY()
		UInventoryComponent* InventoryComponent;

	UPROPERTY()
		UAbilityComponent* AbilityComponent;

	UPROPERTY(EditAnywhere)
		UEventComponent* EventComponent;

	UPROPERTY(EditAnywhere)
		UDataComponent* DataComponent;

	UPROPERTY(EditAnywhere)
		AC_Item* HandItem;

	UPROPERTY(EditAnywhere)
		EMainHand MainHand = EMainHand::Right;

	UPROPERTY(EditAnywhere)
		float HandsAndCameraHeight;

	UPROPERTY(EditAnywhere)
		float CapsuleHeight;

	UPROPERTY()
	    bool IsTriggerPressed = false;

	UPROPERTY()
		int TeamIndex;

	/*UPROPERTY()
		int Money;*/

	UPROPERTY()
		int SpawnIndex;

	UPROPERTY()
		int Kills;

	//////////////////////////////////////////////////////////////////

	UPROPERTY(BlueprintAssignable)
		FOnBlueprintSetup OnBlueprintSetupEvent;

	UFUNCTION()
		void OnLifeChange(int p_Life, int p_Shield);

	UFUNCTION(BlueprintCallable)
		void SetWidgetReference(UUI_Player* p_Widget);

	//////////////////////////////////////////////////////////////////

	UFUNCTION()
		void PlayerHitSomeone(AC_Player* p_Player);

	UFUNCTION()
		void PlayerHasBeenHit();

	UFUNCTION()
		void KillPlayer();

	//////////////////////////////////////////////////////////////////

	UPROPERTY(EditAnywhere)
		float Energy;

	UPROPERTY(EditAnywhere)
		float MaxEnergyLevel;

	UPROPERTY(EditAnywhere)
		float MinEnergyLevel;

	UFUNCTION(BlueprintCallable)
		void AddEnergyLevel(float p_Level);

	UFUNCTION(BlueprintCallable)
		void RemoveEnergyLevel(float p_Level);

	UFUNCTION(BlueprintCallable)
		void UpdateEnergy();

	UFUNCTION(BlueprintCallable)
		void MultiplyEnergyByTwo();

	UFUNCTION(BlueprintCallable)
		void DivideEnergyByTwo();

	UFUNCTION()
		void ResetEnergy();

	//////////////////////////////////////////////////////////////////

	UPROPERTY(BlueprintReadWrite)
		int PlayerMoney;

	UFUNCTION(BlueprintCallable)
		void DropCurrentItem();

	UFUNCTION(BlueprintCallable)
		void PickupItemOnScene();

	UFUNCTION(BlueprintCallable)
		void UseItem();

	UFUNCTION(BlueprintCallable)
		void StopUseItem();

	UFUNCTION(BlueprintCallable)
		void GoToUpperItem();

	UFUNCTION(BlueprintCallable)
		void GoToLowerItem();

	//////////////////////////////////////////////////////////////////

	UFUNCTION()
		void ShowWidgetAmmoBox();

	UFUNCTION()
		void HideWidgetAmmoBox();

	UFUNCTION()
		void SetWidgetMagazineAmmo(int p_Value);

	UFUNCTION()
		void SetWidgetMaxMagazineAmmo(int p_Value);

	UFUNCTION()
		void SetWidgetOtherAmmo(int p_Value);

	//////////////////////////////////////////////////////////////////

	UFUNCTION()
		void OnItemAdded(FItemProperties p_Item, int p_Index);

	UFUNCTION()
		void OnItemRemovedAndNeedChange();

	UFUNCTION()
		void OnItemSelected(FItemProperties p_Item);

	//////////////////////////////////////////////////////////////////

	void InvItem0();
	void InvItem1();
	void InvItem2();
	void InvItem3();
	void InvItem4();
	void InvItem5();

	//void Ability1();

	//////////////////////////////////////////////////////////////////

	UPROPERTY()
		APawn* AttackingPawn;

	UFUNCTION()
		bool IsBot();

	UFUNCTION()
		void OnBotSawPawn(APawn* p_Pawn);

	UFUNCTION()
		AC_BotController* GetBotController();

	UFUNCTION()
		void AttackPawn(APawn* p_Pawn);

	UFUNCTION()
		virtual void OnPreAttackPawn(APawn* p_Pawn, bool p_IsPlayer) {};

	UFUNCTION()
		virtual void OnPostAttackPawn(APawn* p_Pawn, bool p_IsPlayer) {};

	UFUNCTION()
		void OnBotMovementEnd();

	UFUNCTION()
		void MoveToSafePlace();

	UFUNCTION()
		void AskForHelpToMates();

	UFUNCTION()
		virtual void OnMateAskedForHelp(AC_Player* p_Mate) { };

	UFUNCTION()
		void AskForHelp(AC_Player* p_Mate);
		
	//////////////////////////////////////////////////////////////////

	UFUNCTION()
		void NotifyReload();

	//////////////////////////////////////////////////////////////////

	UFUNCTION()
		void OnPlayerDidKill();

	UFUNCTION()
		void OnEventReceived(FString p_Event);

	//////////////////////////////////////////////////////////////////

	UPROPERTY()
		TArray<FVector> KnownEnnemiesPositions;

	UFUNCTION()
		int GetTeammatesAlive();

	UFUNCTION()
		FEnnemiesNumberData GetEnnemiesAlive();

	UFUNCTION()
		TArray<AC_Player*> GetMates();

	UFUNCTION()
		AMapPoint* GetCloserMapPoint(FVector p_Position);

	UFUNCTION()
		void JoinMates();

	void(AC_Player::*PostKillAction)();
};
