//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Implementation of module WeightControl
///
/// @file   WeightControl.cpp
///
///
/// @coypright Ing.büro Hafer
///            Branderweg 8A
///            D-91058 Erlangen
///
/// @author    Detlef Hafer
///
//------------------------------------------------------------------------------------
#include "BASE/Base.h"
#include "WeightControl.h"

using namespace base;


#define ISCONFIGURED ( m_ControlCfg.flags.bConfigured )

//******************************************************************************************
//******************************************************************************************
void CWeightControl::registerLoadcell(loadcell::ILCModuleInterface& rlc)
{
    m_pLoadCell = &rlc;
}
//******************************************************************************************
//******************************************************************************************
void CWeightControl::SetTaraWeight(const float32_t fWeight)
{
    assert(m_pLoadCell);
    m_pLoadCell->SetTaraWeight(m_sID, fWeight);
}
//******************************************************************************************
//******************************************************************************************
base::LC_STATUS CWeightControl::GetLoadCellStatus() const
{
    assert(m_pLoadCell);
    return m_pLoadCell->GetStatus(m_sID);
}
//******************************************************************************************
//******************************************************************************************
void CWeightControl::UpdateTaraWeight()
{
    float32_t fWeight = 0.0F;
    assert(m_pLoadCell);
    m_pLoadCell->GetRawWeight(m_sID, &fWeight);
    SetTaraWeight(fWeight);
}
//******************************************************************************************
//******************************************************************************************
CWeightControl::CWeightControl(int32_t id) : CBaseClass{ id }
, m_pLoadCell{ nullptr }
, m_lLoadCellCounter{ 0 }
{
}
//******************************************************************************************
//******************************************************************************************
BOOL CWeightControl::InitExecute()
{
    auto result = CBaseClass::InitExecute();
    if (result)
    {
        m_lLoadCellCounter = 0;
        m_ControlCfg.allflags = 0;
        m_pAlarm->SetStart(eAlarmError::ERROR_DOSE_LC_TIMEOUT, m_st);
        m_pAlarm->SetStart(eAlarmError::ERROR_DOSE_LC_GENERALERROR, m_st);
        m_pAlarm->SetStart(eAlarmError::ERROR_DOSE_LC_OVERLOAD, m_st);
        m_pAlarm->SetStart(eAlarmError::ERROR_DOSE_LC_PROTOCOLERROR, m_st);
    }
    return result;
}
//******************************************************************************************
//******************************************************************************************
BOOL CWeightControl::ExitExecute()
{
    auto result = CBaseClass::IsInit();
    if (result)
    {
        m_lLoadCellCounter = 0;
        m_ControlCfg.allflags = 0;
        result = CBaseClass::ExitExecute();
    }
    return result;
}
//******************************************************************************************
//******************************************************************************************
base::LC_PRIORITY CWeightControl::SetPriority(const base::LC_PRIORITY s)
{
    assert(m_pLoadCell);
    base::LC_PRIORITY old = m_pLoadCell->GetPriority(m_sID);
    if (old != s)
    {
        m_pLoadCell->SetPriority(m_sID, s);
    }
    return old;
}
//******************************************************************************************
//******************************************************************************************
base::LC_PRIORITY CWeightControl::GetPriority(void) const
{
    assert(m_pLoadCell);
    return m_pLoadCell->GetPriority(m_sID);
}
//******************************************************************************************
//******************************************************************************************
BOOL CWeightControl::CheckLoadcellEnabled()
{
    assert(m_pLoadCell);
    m_ControlCfg.flags.bConfigured = m_pLoadCell->IsConfigured(m_sID);
    if ( ! m_ControlCfg.flags.bLoadcellStartInit)
    {
        m_ControlCfg.flags.bLoadcellStartInit = m_pLoadCell->IsStartInit(m_sID);
    }
    if ( m_ControlCfg.flags.bLoadcellStartInit )
    {
        m_ControlCfg.flags.bInitialized = m_pLoadCell->IsInitialized(m_sID);
        if (m_ControlCfg.flags.bInitialized)
        {
            m_ControlCfg.flags.bLoadcellEnabled = m_pLoadCell->IsEnabled(m_sID);
            assert(m_ControlCfg.flags.bLoadcellEnabled);
        }
        else
        {
            m_ControlCfg.flags.bLoadcellEnabled = false;
        }
        if ( ! m_ControlCfg.flags.bLoadcellEnabled)
        {
            m_ControlCfg.flags.bLoadcellActive = false;
        }
    }
    else
    {
        m_ControlCfg.flags.bLoadcellEnabled  = false;
        m_ControlCfg.flags.bLoadcellActive   = false;
        m_ControlCfg.flags.bInitialized      = false;
    }
    return BOOL(m_ControlCfg.flags.bLoadcellEnabled);
}
//******************************************************************************************
//******************************************************************************************
void CWeightControl::AlarmCheck()
{
    if (ISCONFIGURED)
    {
        auto eStatus = GetLoadCellStatus();
        switch (eStatus)
        {
        case base::LC_STATUS::LC_STATUS_SYNCERROR:
        case base::LC_STATUS::LC_STATUS_NOTAUTHORIZED:
        case base::LC_STATUS::LC_STATUS_NOTOPEN:
        case base::LC_STATUS::LC_STATUS_UNKNOWN:
        {
            if (IsUsed() || (! m_ControlCfg.flags.bAlarmSet))
            {
                m_ControlCfg.flags.bAlarmSet = true;
                m_pAlarm->SetAlarm(m_st, eAlarmError::ERROR_DOSE_LC_GENERALERROR, TRUE, eAlarmClass::eALARMTYP);
            }
        }
        break;

        case LC_STATUS::LC_STATUS_ERROR:
        {
            if (IsUsed() || (!m_ControlCfg.flags.bAlarmSet))
            {
                m_ControlCfg.flags.bAlarmSet = true;
                m_pAlarm->SetAlarm(m_st, eAlarmError::ERROR_DOSE_LC_GENERALERROR, TRUE, eAlarmClass::eALARMTYP);
            }
        }
        break;

        case LC_STATUS::LC_STATUS_OVERLOADED:
        {
            if (m_ControlCfg.flags.bLoadcellOverloaded)
            {
                if (IsUsed() || (!m_ControlCfg.flags.bAlarmSet))
                {
                    m_ControlCfg.flags.bAlarmSet = true;
                    m_pAlarm->SetAlarm(m_st, eAlarmError::ERROR_DOSE_LC_OVERLOAD, TRUE, eAlarmClass::eWARNTYP);
                }
            }
        }
        break;

        case LC_STATUS::LC_STATUS_PROTOCOLERROR:
        {
            if (IsUsed() || (!m_ControlCfg.flags.bAlarmSet))
            {
                m_ControlCfg.flags.bAlarmSet = true;
                m_pAlarm->SetAlarm(m_st, eAlarmError::ERROR_DOSE_LC_PROTOCOLERROR, TRUE, eAlarmClass::eWARNTYP);
            }
        }
        break;

        case LC_STATUS::LC_STATUS_OK:
        {
            m_pAlarm->ClearAlarm(eAlarmError::ERROR_DOSE_LC_GENERALERROR);
            m_pAlarm->ClearAlarm(eAlarmError::ERROR_DOSE_LC_OVERLOAD);
            m_pAlarm->ClearAlarm(eAlarmError::ERROR_DOSE_LC_TIMEOUT);
            m_pAlarm->ClearAlarm(eAlarmError::ERROR_DOSE_LC_PROTOCOLERROR);
            m_ControlCfg.flags.bAlarmSet = false;
        }
        break;

        case LC_STATUS::LC_STATUS_TIMEOUT:
        {
            if (IsUsed() || (!m_ControlCfg.flags.bAlarmSet))
            {
                m_ControlCfg.flags.bAlarmSet = true;
                m_pAlarm->SetAlarm(m_st, eAlarmError::ERROR_DOSE_LC_TIMEOUT, TRUE, eAlarmClass::eWARNTYP);
            }
        }
        break;
        }

        m_pAlarm->CheckWarningLevel(eAlarmError::ERROR_DOSE_LC_TIMEOUT);
        m_pAlarm->CheckWarningLevel(eAlarmError::ERROR_DOSE_LC_GENERALERROR);
        m_pAlarm->CheckWarningLevel(eAlarmError::ERROR_DOSE_LC_OVERLOAD);
        m_pAlarm->CheckWarningLevel(eAlarmError::ERROR_DOSE_LC_PROTOCOLERROR);
    }
}
//******************************************************************************************
//******************************************************************************************
BOOL CWeightControl :: Update ( const uint32_t t)
{
    auto bUpdate = CBaseClass::Update(t);
#if _DEBUG
    assert(bUpdate);
#endif
    auto bEnabled = CheckLoadcellEnabled();
    if (bEnabled)
    {
        assert(m_ControlCfg.flags.bInitialized);
        assert(m_pAlarm);
        assert(m_pLoadCell);
        assert(IsStarted());

        float32_t fRawWeight = 0.0f;
        (void)m_pLoadCell->GetRawWeight(m_sID, &fRawWeight);

        int16_t sWaageErrorCounter = 0;
        float32_t fWeight = 0.0f;
        uint64_t  ulTWeight = 0;
        auto eStatus = m_pLoadCell->GetWeight(m_sID, &fWeight, &ulTWeight, &sWaageErrorCounter);
        m_WeightPair    = utils::CWeightPair(ulTWeight, fWeight);
        m_RawWeightPair = utils::CWeightPair(ulTWeight, fRawWeight);

        switch (eStatus)
        {
            case LC_STATUS::LC_STATUS_UNKNOWN:
            {
                m_lLoadCellCounter = 0L;
                m_ControlCfg.flags.bLoadcellOverloaded  = false;
                m_ControlCfg.flags.bLoadcellActive      = false;
            }
            break;

            case LC_STATUS::LC_STATUS_ERROR:
            {
                m_lLoadCellCounter = 0L;
                m_ControlCfg.flags.bLoadcellOverloaded  = false;
                m_ControlCfg.flags.bLoadcellActive      = false;
            }
            break;

            case LC_STATUS::LC_STATUS_OVERLOADED:
            {
                if (m_lLoadCellCounter < 5U)
                {
                    m_lLoadCellCounter++;
                }
                else
                {
                    m_ControlCfg.flags.bLoadcellOverloaded = true;
                }
                m_ControlCfg.flags.bLoadcellActive = true;
            }
            break;

            case LC_STATUS::LC_STATUS_OK:
            case LC_STATUS::LC_STATUS_PROTOCOLERROR:
            case LC_STATUS::LC_STATUS_SYNCERROR:
            case LC_STATUS::LC_STATUS_NOTAUTHORIZED:
            {
                m_lLoadCellCounter = 0L;
                m_ControlCfg.flags.bLoadcellOverloaded  = false;
                m_ControlCfg.flags.bLoadcellActive      = true;
            }
            break;

            case LC_STATUS::LC_STATUS_TIMEOUT:
            {
                m_lLoadCellCounter = 0L;
                m_ControlCfg.flags.bLoadcellOverloaded  = false;
                m_ControlCfg.flags.bLoadcellActive      = false;
            }
            break;

            default:
                break;
        }
    }
    AlarmCheck();
    return bUpdate;
}



