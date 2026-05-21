//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Implementation of module AlarmManager
///
/// @file   AlarmManager.cpp
///
///
/// @coypright Ing.büro Hafer
///            Branderweg 8A
///            D-91058 Erlangen
///
/// @author    Detlef Hafer
///
//------------------------------------------------------------------------------------
#include <type_traits>
#include <cassert>
#include <algorithm>
#include "BASE/Base.h"
#include "BASE/Utils/public/BitOp.h"
#include "DoseDataLib/include/DoseData.h"
#include "AlarmManager.h"


using namespace base;


//******************************************************************************************************************************
//******************************************************************************************************************************
CAlarmManager::CAlarmManager(int32_t s) : m_id(s)
{
    InitAlarms();
}
//******************************************************************************************************************************
//******************************************************************************************************************************
void CAlarmManager::InitAlarms()
{
    std::for_each(m_AlarmItem.begin(), m_AlarmItem.end(), [](auto& rItem) { rItem.Init(); });
}
//******************************************************************************************************************************
//******************************************************************************************************************************
void CAlarmManager::ClearAlarms()
{
    std::for_each(m_AlarmItem.begin(), m_AlarmItem.end(), [](auto& rItem) { rItem.ClearAlarm(); });
}
//******************************************************************************************************************************
//******************************************************************************************************************************
void CAlarmManager::Start(const uint32_t)
{
    ClearAllAlarms();
    InitAlarms();
}
//******************************************************************************************************************************
//******************************************************************************************************************************
BOOL CAlarmManager::IsAnyAlarmConfig(const base::eAlarmError ul) const
{
    BOOL bActive = FALSE;

    assert(_S32(ul) < _S32(base::eAlarmError::ERROR_MAX));

    DECLARE_ALARMCATEGORY(field);
    assert(_S32(field.size()) == _S32(base::eAlarmCategory::categoryMax));
    for (const auto& item : field)
    {
        Dose_DSVGetAlarmCfgBit(m_id, item, ul, &bActive);
        if (bActive)
        {
            break;
        }
    }
    return bActive;
}
//******************************************************************************************************************************
//******************************************************************************************************************************
void CAlarmManager ::  SetAlarm	( const uint32_t t, const base::eAlarmError ul, const BOOL b, const base::eAlarmClass  bAlarmTyp )
{
    using AlarmError_t = typename std::underlying_type< base::eAlarmError>::type;

	if ( b )
	{
        assert(_S32(ul) < _S32(base::eAlarmError::ERROR_MAX));
        auto& rItem = m_AlarmItem[_S32(ul)];
        assert(t >= rItem.m_tStart);
        rItem.bAlarmConfigured = IsAnyAlarmConfig(ul);
        if (rItem.bAlarmConfigured)
        {
            assert(bAlarmTyp == eAlarmClass::eWARNTYP || bAlarmTyp == eAlarmClass::eALARMTYP);
            if ( _S32(rItem.m_AlarmClass) < _S32(bAlarmTyp))
            {
                ClearWarningLevel(ul);
                rItem.m_AlarmClass  = bAlarmTyp;
                rItem.m_t           = t + 1;
                rItem.m_WarnLevel = ( bAlarmTyp == eAlarmClass::eWARNTYP) ? eAlarmlevelActive::eWARNINGLEVEL : eAlarmlevelActive::eALARMLEVEL;
            }
        }
	}
	else
	{
       ClearAlarm(ul);
	}
}
//******************************************************************************************************************************
//******************************************************************************************************************************
void CAlarmManager::SetFeederAlarm(const int32_t index, BIT bValue)
{
    // Bit pro Dosierer
    uint32_t feederAlarm = 0L;
    Dose_EXGetFeederAlarm(&feederAlarm);
    base::utils::setBit(feederAlarm, _U8(index), bValue);
    Dose_EXSetFeederAlarm(feederAlarm);
}
//******************************************************************************************************************************
//******************************************************************************************************************************
void CAlarmManager ::  ClearAllAlarms	( void)
{
    BOOL bAlarm     = FALSE;
    BOOL bWarning   = FALSE;
    Dose_DSVIsAnyAlarm(m_id, &bAlarm);
    Dose_DSVIsAnyWarning(m_id, &bWarning);
    if (bAlarm || bWarning)
    {
        Dose_DSVDeleteFieldAlarm(m_id);
        Dose_DSVDeleteFieldClearAlarm(m_id);
        Dose_DSVDeleteFieldWarning(m_id);
        Dose_DSVDeleteFieldClearWarning(m_id);
        Dose_DSVSetIOAlarmOutput(m_id, FALSE);
        SetFeederAlarm(m_id, FALSE);
        ClearAlarms();
    }
}
//******************************************************************************************************************************
//******************************************************************************************************************************
void CAlarmManager ::  ClearWarningLevel	(const base::eAlarmError ul)
{
    assert(_S32(ul) < _S32(base::eAlarmError::ERROR_MAX));
    auto& rWarnlevel = m_AlarmItem[_U32(ul)].m_WarnLevel;

    switch (rWarnlevel)
    {
        case eAlarmlevelActive::eWARNINGLEVEL:
        case eAlarmlevelActive::eWARNINGLEVELSET:
                        {
	                        Dose_DSVDeleteWarning            (m_id, ul);
                        }
                        break;

        case eAlarmlevelActive::eALARMLEVEL:
        case eAlarmlevelActive::eALARMLEVELSET :
                        {
                            BOOL bAlarm = FALSE;
                            Dose_DSVDeleteWarning            (m_id, ul);
                            Dose_DSVDeleteAlarm              (m_id, ul);
	                        Dose_DSVIsAnyAlarm               (m_id, &bAlarm);
	                        Dose_DSVSetIOAlarmOutput         (m_id, bAlarm);
                            SetFeederAlarm(m_id, BIT(bAlarm));
                        }
                        break;
            default:
                    break;
     }
    rWarnlevel = eAlarmlevelActive::eNOWARNING;
}
//******************************************************************************************************************************
//******************************************************************************************************************************
void CAlarmManager ::  ClearAlarm	(const base::eAlarmError ul)
{
    assert(_S32(ul) < _S32(base::eAlarmError::ERROR_MAX));
    if ( IsAlarm(ul) )
    {
        ClearWarningLevel(ul);
        m_AlarmItem[_U32(ul)].ClearAlarm();
    }
}
//******************************************************************************************************************************
//******************************************************************************************************************************
void CAlarmManager::ActivateAlarm(const base::eAlarmError ul) const
{
    BOOL bA = FALSE;
    BOOL bActive = FALSE;

    assert(_S32(ul) < _S32(base::eAlarmError::ERROR_MAX));
    Dose_DSVGetAlarmCfgBit(m_id, eAlarmCategory::categoryAM, ul, &bA);
    if (bA)
    {
        Dose_EXPBSetPBAlarm0(TRUE);
        bActive = TRUE;
    }
    // FlashLight
    Dose_DSVGetAlarmCfgBit(m_id, eAlarmCategory::categoryFL, ul, &bA);
    if (bA)
    {
        BOOL bAlarmExist = FALSE;
        Dose_DSVIsAlarm(m_id, ul, &bAlarmExist);
        if ( ! bAlarmExist)
        {
            BOOL bAnyDoseStarted = FALSE;
            Dose_EXGetAnyDoseStarted(&bAnyDoseStarted);
            if (bAnyDoseStarted)
            {
                Dose_EXPBSetPBAlarm1(TRUE);
            }
        }
        Dose_DSVSetIOAlarmOutput(m_id, TRUE);
        bActive = TRUE;
    }
    Dose_DSVGetAlarmCfgBit(m_id, eAlarmCategory::categoryS, ul, &bA);
    if (bA)
    {
        Dose_DSVPBSetPBDosiererStop(m_id);
        Dose_DSVSetAlarm(m_id, base::eAlarmError::ERROR_DOSE_ALARMSTOP);
        Dose_EXPBSetPBAlarm2(TRUE);
        bActive = TRUE;
    }
    Dose_DSVGetAlarmCfgBit(m_id, eAlarmCategory::categorySD, ul, &bA);
    if (bA)
    {
        Dose_EXPBSetPBLineShutDown();
        Dose_EXPBSetPBAlarm3(TRUE);
        bActive = TRUE;
    }
    Dose_DSVGetAlarmCfgBit(m_id, eAlarmCategory::categoryRA, ul, &bA);
    if (bA)
    {
        bActive = TRUE;
    }
    if (bActive)
    {
        Dose_DSVSetAlarm(m_id, ul);
        SetFeederAlarm(m_id, TRUE);
    }
}
//******************************************************************************************************************************
//******************************************************************************************************************************
void CAlarmManager :: CheckWarningLevel( const uint32_t t, const base::eAlarmError ul)
{
	uint32_t lS         = 0L;

    assert(_S32(ul) < _S32(base::eAlarmError::ERROR_MAX));

    if ( ! IsAlarm(ul) )
    {
        return;
    }

    BOOL bA = IsAnyAlarmConfig(ul);
    if ( ! bA)
    {
        ClearAlarm(ul);
        return;
    }

    auto& rItem = m_AlarmItem[_U32(ul)];
    switch (rItem.m_WarnLevel)
    {
        case eAlarmlevelActive::eWARNINGLEVEL :
                        {
	                        Dose_DSVSetWarning   (m_id, ul);
                            rItem.m_WarnLevel = eAlarmlevelActive::eWARNINGLEVELSET;
                        }
                        break;

        case eAlarmlevelActive::eWARNINGLEVELSET :
                        {
		                    Dose_DSVGetAlarmStartReactionDelay(m_id, &lS);
		                    if ( t > rItem.m_tStart + lS )
		                    {
			                    Dose_DSVGetAlarmReactionDelay(m_id, &lS);
			                    if ( t > rItem.m_t + lS )
			                    {
                                    ClearWarningLevel(ul);
                                    ActivateAlarm(ul);
                                    rItem.m_WarnLevel = eAlarmlevelActive::eALARMLEVELSET;
                                }
                            }
                        }
                        break;

        case eAlarmlevelActive::eALARMLEVEL :
                        {
                            ActivateAlarm(ul);
                            rItem.m_WarnLevel = eAlarmlevelActive::eALARMLEVELSET;
                        }
                        break;

        case eAlarmlevelActive::eALARMLEVELSET :
                        {
                        }
                        break;

        default :
                        assert(FALSE);
                        break;
	}
}
//******************************************************************************************************************************
//******************************************************************************************************************************
void CAlarmManager :: CheckWarningLevel(const base::eAlarmError ul)
{
    assert(_S32(ul) < _S32(base::eAlarmError::ERROR_MAX));

    if ( ! IsAlarm(ul) )
    {
        return;
    }

    BOOL bA = IsAnyAlarmConfig(ul);
    if (!bA)
    {
        
        ClearAlarm(ul);
        return;
    }

    auto& rItem = m_AlarmItem[_U32(ul)];
    switch (rItem.m_WarnLevel )
    {
        case eAlarmlevelActive::eWARNINGLEVEL :
                        {
	                        Dose_DSVSetWarning   (m_id, ul);
                            rItem.m_WarnLevel = eAlarmlevelActive::eWARNINGLEVELSET;
                        }
                        break;

        case eAlarmlevelActive::eWARNINGLEVELSET :
                        {
                        }
                        break;

        case eAlarmlevelActive::eALARMLEVEL :
                        {
                            ActivateAlarm(ul);
                            rItem.m_WarnLevel = eAlarmlevelActive::eALARMLEVELSET;
                         }
                         break;

        case eAlarmlevelActive::eALARMLEVELSET :
                        {
                        }
                        break;
        default :
                        assert(FALSE);
                        break;
	}
}
//*************************************************************************************************
//*************************************************************************************************
void CAlarmManager::SetStart(const base::eAlarmError ul, const uint32_t t)
{
    //assert(!IsAlarm(ul));
    ClearAlarm(ul);
    auto& cItem = m_AlarmItem[_U32(ul)];
    cItem.m_tStart = t;
}
//*************************************************************************************************
//*************************************************************************************************
BOOL CAlarmManager::IsAlarm(const base::eAlarmError ul) const
{
    assert(_S32(ul) < _S32(base::eAlarmError::ERROR_MAX));
    const auto& cItem = m_AlarmItem[_U32(ul)];
    BOOL bActive = (cItem.m_t != 0);
#if _DEBUG
    if (!bActive)
    {
        assert(cItem.m_WarnLevel == eAlarmlevelActive::eNOWARNING);
        assert(cItem.m_AlarmClass == base::eAlarmClass::eNOTYP);
        assert(cItem.m_t == 0);
    }
#endif
    return bActive;
}
