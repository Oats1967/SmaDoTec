//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Declaration of module AdsButton
///
/// @file   AdsButton.h
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
	//----------------------------------------------------------------------------------------------------------
	//----------------------------------------------------------------------------------------------------------
	template <typename T>
	class CAdsButton : public CAdsDualState<T>
	{
		using CBaseClass = CAdsDualState<T>;

	public:
		CAdsButton(const int32_t id) : CBaseClass(id)
		{}

		virtual ~CAdsButton() = default;
		virtual BOOL UpdateState(const T bState) = 0;
		virtual void SetValue(const T b) = 0;
	};
};

