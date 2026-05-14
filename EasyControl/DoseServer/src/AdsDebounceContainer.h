//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Declaration of module AdsDebounceContainer
///
/// @file   AdsDebounceContainer.h
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
	template <class T = CAdsDebounceDoseSensor>
	class CAdsDebounceContainer : public CAdsDualStateContainer<T>
	{
		using CBaseClass = CAdsDualStateContainer< T>;

	public:
		CAdsDebounceContainer() : CBaseClass()
		{}
		CAdsDebounceContainer(const std::vector<T*>& pV) : CBaseClass(pV)
		{}

		virtual ~CAdsDebounceContainer() = default;

		void SetDebounceTime(const int32_t index, uint32_t value);
		uint32_t GetDebounceTime(const int32_t index) const;
		void Update(uint32_t t);
	};
	//-----------------------------------------------------------------------------------------------------------
	//-----------------------------------------------------------------------------------------------------------
	template <class T>
	inline void CAdsDebounceContainer<T>::SetDebounceTime(const int32_t index, uint32_t value)
	{
		assert(CBaseClass::IsInit());
		assert(index >= 0 && index < _S32(CBaseClass::m_Vector.size()));
		return CBaseClass::m_Vector[index]->SetDebounceTime(value);
	}
	//-----------------------------------------------------------------------------------------------------------
	//-----------------------------------------------------------------------------------------------------------
	template <class T>
	inline uint32_t CAdsDebounceContainer<T>::GetDebounceTime(const int32_t index) const
	{
		assert(CBaseClass::IsInit());
		assert(index >= 0 && index < _S32(CBaseClass::m_Vector.size()));
		return CBaseClass::m_Vector[index]->GetDebounceTime();
	}
	//-----------------------------------------------------------------------------------------------------------
	//-----------------------------------------------------------------------------------------------------------
	template <class T>
	inline void CAdsDebounceContainer<T>::Update(uint32_t t)
	{
		assert(CBaseClass::IsInit());
		for (auto& rItem : CBaseClass::m_Vector)
		{
			rItem->Update(t);
		}
	}

};

