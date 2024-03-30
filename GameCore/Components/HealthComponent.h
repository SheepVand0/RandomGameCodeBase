// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HealthComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class GAMECORE_API UHealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UHealthComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UPROPERTY()
		int m_Life;

	UPROPERTY()
		int m_BaseLife;

	UPROPERTY()
		int m_Shield;

	UPROPERTY()
		int m_BaseShield;

public:
	// Called every frame
	//virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDeath);

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnLifeChange, int, Life, int, Shield);

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnDamages, int, Life, int, Shield);

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnHeal, int, Life, int, Shield);

	UPROPERTY(BlueprintAssignable)
		FOnDeath OnDeath;

	UPROPERTY(BlueprintAssignable)
		FOnLifeChange OnLifeChange;

	UPROPERTY(BlueprintAssignable)
		FOnDamages OnDamages;

	UPROPERTY(BlueprintAssignable)
		FOnHeal OnHeal;

	UFUNCTION(BlueprintCallable)
		bool ApplyDamages(int p_Life);

	UFUNCTION(BlueprintCallable)
		void SetLife(int p_Life, int p_Shield);

	UFUNCTION(BlueprintCallable)
		void Heal(int p_Life);

	UFUNCTION(BlueprintCallable)
		bool VerifyLife();

	UFUNCTION(BlueprintCallable)
		void SetBaseLife(int p_BaseLife);

	UFUNCTION(BlueprintCallable)
		void SetBaseShield(int p_BaseShield);

	UFUNCTION(BlueprintCallable)
		int GetLife();

	UFUNCTION(BlueprintCallable)
		int GetShield();

	UFUNCTION(BlueprintCallable)
		int GetBaseLife();

	UFUNCTION(BlueprintCallable)
		int GetBaseShield();

};
