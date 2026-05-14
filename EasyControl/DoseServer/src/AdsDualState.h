//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Declaration of module AdsDualState
///
/// @file   AdsDualState.h
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
#include "AdsWrapperControl.h"

namespace AdsSensor
{
	class CAdsStateControl : public CAdsWrapperControl
	{
		using CBaseClass = CAdsWrapperControl;

	protected:
		BOOL m_bEnable;

	public:
		CAdsStateControl(const int32_t id) : CBaseClass(id)
			, m_bEnable{ FALSE }
		{}

		CAdsStateControl() : CBaseClass()
			, m_bEnable{ FALSE }
		{}

		virtual ~CAdsStateControl() = default;

		virtual BOOL GetEnable() const
		{	return m_bEnable;		}
		virtual void SetEnable(const BOOL bEnable)
		{	m_bEnable = bEnable;		}

	};
	//----------------------------------------------------------------------------------------------------------
	//----------------------------------------------------------------------------------------------------------
	template <typename T>
	class CAdsDualState : public CAdsStateControl
	{
		using CBaseClass = CAdsStateControl;

	protected:
		T	 m_state;

	protected:
		BOOL IsChanged(const T value) const
		{	return BOOL(value != m_state);		}

	public:
		CAdsDualState(const int32_t id) : CBaseClass(id)
			, m_state { T(0) }
		{}

		CAdsDualState() : CBaseClass()
			, m_state{ T(0) }
		{}

		virtual ~CAdsDualState() = default;

		virtual void SetState(const T bState)
		{	
			m_state = bState; 
		}
		virtual T GetState(void) const
		{	return m_state;	}
	};
};

