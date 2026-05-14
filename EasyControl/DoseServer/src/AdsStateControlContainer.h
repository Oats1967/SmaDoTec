//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Declaration of module AdsSensorContainer
///
/// @file   AdsSensorContainer.h
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

#include <cassert>
#include "AdsDualStateContainer.h"

namespace AdsSensor
{
	//----------------------------------------------------------------------------------------------------------
	//----------------------------------------------------------------------------------------------------------
	template <class T = CAdsStateControl>
	class CAdsStateControlContainer : public CAdsDualStateContainer<T>
	{
		using CBaseClass = CAdsDualStateContainer<T>;

	public:
		CAdsStateControlContainer() = default;

		CAdsStateControlContainer(const std::vector< T*>& rVector) : CBaseClass(rVector)
		{}

		virtual ~CAdsStateControlContainer() = default;

		virtual BOOL UpdateState();
		virtual BOOL UpdateState(const int32_t index);
	};

	//-----------------------------------------------------------------------------------------------------------
	//-----------------------------------------------------------------------------------------------------------
	template <class T>
	inline BOOL CAdsStateControlContainer<T>::UpdateState()
	{
		assert(CBaseClass::IsInit());

		BOOL bEnable = FALSE;
		for (auto& rItem : CBaseClass::m_Vector)
		{
			bEnable |= rItem->UpdateState();
		}
		return bEnable;
	}
	//-----------------------------------------------------------------------------------------------------------
	//-----------------------------------------------------------------------------------------------------------
	template <class T>
	inline BOOL CAdsStateControlContainer<T>::UpdateState(const int32_t index)
	{
		assert(CBaseClass::IsInit());
		return CBaseClass::m_Vector[index]->UpdateState();
	}
		
};

