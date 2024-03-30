// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
class GAMECORE_API Other
{
public:
	Other();
	~Other();

	template<class t_DataType>
	static TArray<t_DataType> AddRangeToArray(TArray<t_DataType> p_Base, TArray<t_DataType> p_Other) {
		TArray<t_DataType> l_Base = p_Base;
		for (int l_i = 0; l_i < p_Other.Num(); l_i++) {
			l_Base.Add(p_Other[l_i]);
		}
		return l_Base;
	}
};
