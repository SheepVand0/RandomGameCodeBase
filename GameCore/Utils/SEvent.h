// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameCore/Utils/MethodsHelper.h"


/**
 * 
 */
template<class... t_Params>
class GAMECORE_API SEvent
{
protected:

	typedef SMethodPtrBase<void, t_Params...> MethodType;

	TArray<SMethodPtrBase<void, t_Params...>> m_Functions;

public:

	SEvent() {
		 
	}

	SEvent<t_Params...>& operator +=(MethodType p_Method) {
		Bind(p_Method);
	}

	void Bind(MethodType p_Method) {
		if (m_Functions.Contains(p_Method)) return;

		m_Functions.Add(p_Method);
	}

	void Invoke(t_Params... p_Parameters) {
		for (int l_i = 0; l_i < m_Functions.Num(); l_i++) {
			m_Functions[l_i].Invoke(p_Parameters...);
		}
	}
};
