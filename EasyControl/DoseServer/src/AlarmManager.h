//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Declaration of module AlarmManager
///
/// @file   AlarmManager.h
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
#include <array>
#include "BASE/include/AlarmTypes.h"


class CAlarmManager
{
private:
    enum class eAlarmlevelActive : uint16_t
    {
        eNOWARNING = 0,
        eWARNINGLEVEL,
        eWARNINGLEVELSET,
        eALARMLEVEL,
        eALARMLEVELSET,
        eALARMMAX
    };
    static const int32_t eAlarmlevelMax = _S32(eAlarmlevelActive::eALARMMAX);

    struct sAlarm
    {
        uint32_t m_tStart = 0;
        uint32_t m_t = 0;
        eAlarmlevelActive m_WarnLevel = eAlarmlevelActive::eNOWARNING;
        base::eAlarmClass m_AlarmClass = base::eAlarmClass::eNOTYP;
        BOOL bAlarmConfigured = FALSE;

        void ClearAlarm()
        {
            m_t = 0;
            m_WarnLevel = eAlarmlevelActive::eNOWARNING;
            m_AlarmClass = base::eAlarmClass::eNOTYP;
        }

        void Init()
        {
            m_tStart = 0;
            ClearAlarm();
        }


    };

    std::array < sAlarm, base::eAlarmErrorMax>	m_AlarmItem;
    int32_t	    m_id;

private:
    BOOL IsAnyAlarmConfig(const base::eAlarmError ul) const;
    void ActivateAlarm(const base::eAlarmError ul) const;
    void InitAlarms();
    void ClearAlarms();
    static void SetFeederAlarm(int32_t index, BIT bValue);

public:
    CAlarmManager(int32_t s = -1);
    virtual ~CAlarmManager(void) = default;

    void Start(const uint32_t);
    void Stop() {}
    void SetStart(const base::eAlarmError ul, const uint32_t t);
    BOOL IsAlarm(const base::eAlarmError ul) const;

    void ClearAlarm                 (const base::eAlarmError ul);
    void ClearAllAlarms             ( void );
    void CheckWarningLevel          ( const uint32_t t, const base::eAlarmError ul);
    void CheckWarningLevel          ( const base::eAlarmError ul);
    void ClearWarningLevel	        ( const base::eAlarmError ul);
    void SetAlarm	                ( const uint32_t t, const base::eAlarmError al, const BOOL b, const base::eAlarmClass bAlarmTyp = base::eAlarmClass::eNOTYP);
};
