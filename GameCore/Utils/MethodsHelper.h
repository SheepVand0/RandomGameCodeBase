// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
template<class t_ReturnType, class... t_Params>
class GAMECORE_API SMethodPtrBase {

	virtual t_ReturnType Invoke(t_Params...) = 0;

};

template<class t_ClassType, class t_ReturnType, class... t_Params>
class GAMECORE_API SMethodPtr : SMethodPtrBase<t_ReturnType, t_Params...> {

public:

	typedef t_ReturnType(t_ClassType::* MethodType)(t_Params...);

	virtual t_ReturnType Invoke(t_Params...) override {
		return m_Method(t_Params...);
	}

protected:
	MethodType m_Method;

};