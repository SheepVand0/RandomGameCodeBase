// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameCore/Utils/Enums/GameCoreEnums.h"
#include "GameFramework/Character.h"
#include "Components/BoxComponent.h"
#include "C_Item.generated.h"

class AC_Item;

USTRUCT()
struct FItemProperties
{
	GENERATED_BODY()

public:
	FItemProperties();

	UPROPERTY()
		TSubclassOf<AC_Item> ItemClass = TSubclassOf<AC_Item>{};

	UPROPERTY()
		FString ItemName;

	UPROPERTY()
		FString Category;	

	UPROPERTY()
		FTransform HandedTransform;

	UPROPERTY()
		bool Stackable;

	UPROPERTY()
		UTexture2D* ItemIcon;

	UPROPERTY()
		bool RemovableFromInventory;

	static FItemProperties GetNullItem();

	static FItemProperties CreateItemProperties(TSubclassOf<AC_Item> p_ItemClass, FString p_ItemName, FString p_Category, FTransform p_HandedTransform, bool p_Stackable, UTexture2D* ItemIcon);

	static bool Equal(FItemProperties p_Properties1, FItemProperties p_Properties2);

	static bool IsNullItem(FItemProperties p_Item);
};

UCLASS()
class GAMECORE_API AC_Item : public AActor
{
	GENERATED_BODY()

private:
	UPROPERTY()
	bool m_IsUsing = false;

	UPROPERTY()
	UStaticMeshComponent* m_RootStaticMesh;

	UPROPERTY()
	USkeletalMeshComponent* m_RootSkeletalMesh;

	UPROPERTY()
	USceneComponent* m_RootComponent;

public:
	// Sets default values for this actor's properties
	AC_Item();

	UFUNCTION()
		void SetChildsHasCollision(ECollisionEnabled::Type p_Enabled);

protected:
	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

	void Construct(FItemProperties p_Properties, FTransform p_CollisionTransform,
	               UStaticMeshComponent* p_RootStaticMesh,
	               USkeletalMeshComponent* p_RootSkeletalMesh,
	               bool p_SimulatePhysicsOnDrop,
	               FItemRarity p_Rarity);


	FItemProperties m_ItemProperties;

	UPROPERTY()
	bool m_SimulatePhysicsOnDrop = true;

	UPROPERTY(BlueprintReadOnly)
	FItemRarity m_ItemRarity;

	UPROPERTY()
	UBoxComponent* m_ItemCollision;
public:

	UPROPERTY()
		ACharacter* m_ParentPlayer;

	UFUNCTION()
		void Use();

	UFUNCTION()
		void StopUse();

	UFUNCTION(BlueprintCallable)
		virtual void OnUse();

	UFUNCTION(BlueprintCallable)
		virtual void OnStopUse();

	UFUNCTION(BlueprintCallable)
		virtual bool IsUsing();

	UFUNCTION(BlueprintCallable)
		const FColor GetRarityColor(FItemRarity p_Rarity);

	UFUNCTION(BlueprintCallable)
		virtual void Select(ACharacter* p_ParentPlayer, USceneComponent* p_AttachObject);

	UFUNCTION(BlueprintCallable)
		void UnSelect();

	FItemProperties GetPropertiesCopy();

	UFUNCTION()
		void SetItemProperties(FItemProperties p_Properties);

	UFUNCTION()
		void SetItemSimulatePhysics(bool p_IsSimulating);

	UFUNCTION()
		void Drop();

	UFUNCTION(BlueprintCallable)
		virtual void OnItemSelected();

	UFUNCTION(BlueprintCallable)
		virtual void OnItemUnselected();

	UFUNCTION(BlueprintCallable)
		virtual void NotifyEvent(FString p_Name);

private:
	UPROPERTY()
	TMap<FItemRarity, FColor> m_RarityColor;

	FColor* m_DefaultRarityColor = new FColor(0.7, 0.7, 0.7);
};
