//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Declaration of module AdsDualStateContainer
///
/// @file   AdsDualStateContainer.h
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
#include <vector>
#include "AdsWrapperControl.h"

namespace AdsSensor
{
	//----------------------------------------------------------------------------------------------------------
	//----------------------------------------------------------------------------------------------------------
	template < class T = CAdsStateControl>
	class CAdsDualStateContainer : public CAdsWrapperControl
	{
		using CBaseClass = CAdsWrapperControl;

	protected:
		std::vector< T*> m_Vector;

	public:
		CAdsDualStateContainer() = default;

		CAdsDualStateContainer(const std::vector< T *>& rVector) : CBaseClass(0)
			, m_Vector{ rVector }
		{}

		virtual ~CAdsDualStateContainer() = default;

		void Init() override;
		void Exit() override;

		BOOL GetState(const int32_t index) const;
		BOOL GetEnable(const int32_t index) const;
		BOOL GetEnable() const;

		void add(T* pItem)
		{	m_Vector.push_back(pItem);		}

		void add(const std::vector< T*>& rVector)
		{	m_Vector += rVector;	}

		void clear()
		{	m_Vector.clear();	}

		uint32_t size() const
		{ _U32(m_Vector.size());		}

	};

	//-----------------------------------------------------------------------------------------------------------
	//-----------------------------------------------------------------------------------------------------------
	template <class T>
	inline BOOL CAdsDualStateContainer<T>::GetState(const int32_t index) const
	{
		assert(CBaseClass::IsInit());
		assert(index >= 0 && index < _S32(m_Vector.size()));
		return m_Vector[index]->GetState();
	}
	//-----------------------------------------------------------------------------------------------------------
	//-----------------------------------------------------------------------------------------------------------
	template <class T>
	inline BOOL CAdsDualStateContainer<T>::GetEnable(const int32_t index) const
	{
		assert(CBaseClass::IsInit());
		assert(index >= 0 && index < _S32(m_Vector.size()));
		return m_Vector[index]->GetEnable();
	}
	//-----------------------------------------------------------------------------------------------------------
	//-----------------------------------------------------------------------------------------------------------
	template <class T>
	inline BOOL CAdsDualStateContainer<T>::GetEnable() const
	{
		assert(CBaseClass::IsInit());
		BOOL bEnable = FALSE;
		for (auto& rItem : m_Vector)
		{
			bEnable |= rItem->GetEnable();
		}
		return bEnable;
	}
	//-----------------------------------------------------------------------------------------------------------
	//-----------------------------------------------------------------------------------------------------------
	template <class T>
	inline void CAdsDualStateContainer<T>::Init()
	{
		assert(! CBaseClass::IsInit());
		CBaseClass::Init();

		for (auto& rItem : m_Vector)
		{
			rItem->Init();
		}
	}
	//-----------------------------------------------------------------------------------------------------------
	//-----------------------------------------------------------------------------------------------------------
	template <class T>
	inline void CAdsDualStateContainer<T>::Exit()
	{
		assert(CBaseClass::IsInit());

		for (auto& rItem : m_Vector)
		{
			rItem->Exit();
		}
		CBaseClass::Exit();
	}
};

