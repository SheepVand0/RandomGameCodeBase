// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Animation.generated.h"

UCLASS()
class GAMECORE_API UAnimationKey : public UObject {

	GENERATED_BODY()

public:

	UAnimationKey() {};

	UAnimationKey(float p_Time, float p_Key) {
		Time = p_Key;
		Key = p_Time;
	};
	
	UPROPERTY()
		float Time;

	UPROPERTY()
		float Key;
};

UCLASS()
class GAMECORE_API UAnimationCurve : public UObject {

	GENERATED_BODY()

public:

	UAnimationCurve() {
	};

	UPROPERTY(BlueprintReadWrite)
		TArray<UAnimationKey*> Keys;

	UFUNCTION(BlueprintCallable)
	float Length() {
		float l_Length = 0;
		float l_LastTime = 0;
		for (int l_i = 0; l_i < Keys.Num(); l_i++) {
			float l_Time = Keys[l_i]->Time;
			l_Length += l_Time - l_LastTime;
			l_LastTime = l_Time;
		}
		return l_Length;
	};

	UFUNCTION(BlueprintCallable)
		void SortKeysArray();

};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class GAMECORE_API UAnimation : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UAnimation();

private:

	float StartTime;

	float Duration;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnUpdate, float, Value);

	UPROPERTY()
		bool IsPlaying;

	UPROPERTY()
		UAnimationCurve* Curve;

	UPROPERTY()
		UAnimationCurve* TempCurve;

	UPROPERTY()
		bool HasPendingCurve;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void SetCurve(UAnimationCurve* p_Curve);

	UFUNCTION(BlueprintCallable)
		void Play();

	UFUNCTION(BlueprintCallable)
		void Stop();

	UPROPERTY(BlueprintAssignable)
		FOnUpdate OnUpdate;
};
