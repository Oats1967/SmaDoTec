//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Declaration of module AdsDebounceState
///
/// @file   AdsDebounceState.h
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

#include "AdsSensor.h"

namespace AdsSensor
{
	//----------------------------------------------------------------------------------------------------------
	//----------------------------------------------------------------------------------------------------------
	class CAdsDebounceState
	{
		uint32_t		m_DebounceTime;
		uint32_t		m_t;
		uint32_t		m_Counter;
		BOOL			m_LastValue;

    protected:
        virtual BOOL UpdateDebounceState(BOOL&) = 0;
        virtual void SetDebounceState(const BOOL) = 0;

	public:
        CAdsDebounceState(uint32_t _DebounceTime = 0);

		virtual ~CAdsDebounceState() = default;

		virtual void Update(uint32_t t);

		SETGET(uint32_t, DebounceTime)
	};
	

    //**********************************************************************************************
    //**********************************************************************************************
    inline CAdsDebounceState::CAdsDebounceState(uint32_t _DebounceTime) :
          m_DebounceTime{ _DebounceTime }
        , m_t{ 0 }
        , m_Counter{ 0 }
        , m_LastValue{ 0 }
    {}
    //**********************************************************************************************
    //**********************************************************************************************
    inline void CAdsDebounceState::Update(uint32_t t)
    {
        if (t != m_t)
        {
            m_t = t;
            BOOL value = FALSE;
            auto enable = UpdateDebounceState(value);
            if (enable)
            {
                if (value != m_LastValue)
                {
                    m_LastValue = value;
                    m_Counter = 1U;
                }
                else
                {
                    if (m_Counter < m_DebounceTime)
                    {
                        m_Counter++;
                    }
                }
                if (m_Counter >= m_DebounceTime)
                {
                    SetDebounceState(value);
                }
            }
        }
    }


};

