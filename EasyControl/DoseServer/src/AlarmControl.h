//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Declaratior of module CAlarmControl
///
/// @file   AlarmControl.h
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

#include "BASE/types.h"
#include "BASE/Base.def"
#include "AlarmManager.h"
#include "BASE/include/Statemachine.h"



class CAlarmControl : public base::CStatemachine< int32_t >
{
protected:
    CAlarmManager* m_pAlarm;
    uint32_t m_tStart;
    uint32_t m_delay;

public:
    CAlarmControl(int32_t id) : CStatemachine{ id }
        , m_pAlarm{ nullptr }
        , m_tStart{ 0 }
        , m_delay{ 0 }
    {}

    virtual ~CAlarmControl() = default;
    CAlarmControl(const CAlarmControl&) = delete;
    CAlarmControl& operator=(const CAlarmControl&) = delete;

    void SetDelay(const uint32_t iDelay)
    {  m_delay = iDelay;   }

    BOOL Start(uint32_t t) override
    {
        auto result = CStatemachine::Start(t);
        if (result)
        {
            m_tStart = t;
        }
        return result;
    }

    BOOL Stop(void) override
    {
        auto result = CStatemachine::IsStarted();
        if (result)
        {
            m_tStart = 0;
            result = CStatemachine::Stop();
        }
        return result;
    }

    void registerAlarmManager(CAlarmManager& rAlarm)
    {    m_pAlarm = &rAlarm;    }
};


