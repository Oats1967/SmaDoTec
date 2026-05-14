//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Declaration of module AdsSensorControl
///
/// @file   AdsSensorControl.h
///
///
/// @coypright Ing.büro Hafer
///            Branderweg 8A
///            D-91058 Erlangen
///
/// @author    Detlef Hafer
///
//------------------------------------------------------------------------------------
#pragma once

#include "AdsDualState.h"

namespace AdsSensor
{
	template < typename T>
	class CAdsSensor : public CAdsDualState<T>
	{
		using CBaseClass = CAdsDualState<T>;


	public:
		CAdsSensor(const int32_t id) : CBaseClass(id)
		{}

		CAdsSensor() : CBaseClass()
		{}

		virtual ~CAdsSensor() = default;
		virtual BOOL UpdateState(T&)	= 0;
		virtual void SetValue(const T)	= 0;
	};

};

