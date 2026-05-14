//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Implementation of module IfsControl
///
/// @file   IfsControl.cpp
///
///
/// @coypright Ing.büro Hafer
///            Branderweg 8A
///            D-91058 Erlangen
///
/// @author    Detlef Hafer
///
//------------------------------------------------------------------------------------
#include <math.h>
#include "BASE/Base.def"
#include "IfsControl.h"
#include "DoseDataLib/include/DoseData.h"
#include "Logger/include/Logger.h"
#include "IfsMacros.hxx"


#define EDITITEM(_a, _func) 	 { _a, &CIfsControl::_func }

//*************************************************************************************
//*************************************************************************************
CIfsControl::CIfsControl(int32_t id) : CBaseClass(id)
, c_EditMap({
        EDITITEM(eSubStep::eInitReached,	        StepInitReached),
        EDITITEM(eSubStep::eEmptyReached,	        StepEmptyReached),
        EDITITEM(eSubStep::eLowReached,	            StepLowReached),
        EDITITEM(eSubStep::eMinReached,	            StepMinReached),
        EDITITEM(eSubStep::eMaxReached,	            StepMaxReached),
        EDITITEM(eSubStep::eHighReached,	        StepHighReached),
        EDITITEM(eSubStep::eMaxOverflowReached,	    StepMaxOverflowReached),
        EDITITEM(eSubStep::eHighOverflowReached,    StepHighOverflowReached),
        EDITITEM(eSubStep::eEmptyOverflowReached,	StepEmptyOverflowReached),
    })
    , m_AdsClient{ id }
    , m_eSubStep { eSubStep::eInitReached }
{}
//*************************************************************************************
//*************************************************************************************
BOOL CIfsControl::CheckInputs()
{
    LOGFUNCTION(m_eSubStep)

    GetLineSetpoint();
    SetLineMassflow();
    Dose_DSVSetIfsEmptyFeeder(m_sID, m_eSubStep == eSubStep::eEmptyOverflowReached);
    Dose_DSVSetIfsOverflowFeeder(m_sID, (m_eSubStep == eSubStep::eMaxOverflowReached) || (m_eSubStep == eSubStep::eHighOverflowReached));
    return TRUE;
}
//*************************************************************************************
//*************************************************************************************
void CIfsControl::IncreaseSetpoint()
{
    LOGFUNCTION(m_eSubStep)

    float32_t fMaxSetpoint = 0.0F;
    Dose_EXGetManualSetpoint(&fMaxSetpoint);
    float32_t fSetpoint = 0.0F;
    Dose_EXGetLineSetpoint(&fSetpoint);
    float32_t factor = 0.0F;
    Dose_DSVGetIfsGainFactor(m_sID, &factor);
    factor = __max(factor, 1.0F);
    fSetpoint += factor;
    fSetpoint = __min(fSetpoint, 2.0f * fMaxSetpoint);
    Dose_EXSetMBLineSetpoint(fSetpoint);
}
//*************************************************************************************
//*************************************************************************************
void CIfsControl::ReduceSetpoint()
{
    LOGFUNCTION(m_eSubStep)

    float32_t fMinSetpoint = 0.0F;
    Dose_EXGetManualSetpoint(&fMinSetpoint);
    fMinSetpoint = __max(fMinSetpoint / 2.0F, 0.1F);

    float32_t fSetpoint = 0.0F;
    Dose_EXGetLineSetpoint(&fSetpoint);
    float32_t factor = 0.0F;
    Dose_DSVGetIfsReduceFactor(m_sID, &factor);
    factor = __max(factor, 1.0F);
    fSetpoint -= factor;
    fSetpoint = __max(fSetpoint, fMinSetpoint);
    Dose_EXSetMBLineSetpoint(fSetpoint);
}
//*************************************************************************************
//*************************************************************************************
void CIfsControl::SetNormalSetpoint()
{
    LOGFUNCTION(m_eSubStep)

    float32_t newSetpoint = 0.0F;
    Dose_EXGetManualSetpoint(&newSetpoint);
    newSetpoint = __max(newSetpoint, 0.0F);
    Dose_EXSetMBLineSetpoint(newSetpoint);
}
//*************************************************************************************
//*************************************************************************************
void CIfsControl::GetLineSetpoint()
{
    //LOGFUNCTION(m_eSubStep)

    float32_t value = 0.0F;
    Dose_EXGetLineSetpoint(&value);
    Dose_DSVSetActualSetpoint(m_sID, value);
    Dose_DSVSetNominalSetpoint(m_sID, value);
}
//*************************************************************************************
//*************************************************************************************
void CIfsControl::SetLineMassflow()
{
    //LOGFUNCTION(m_eSubStep)

    float32_t value = 0.0F;
    Dose_EXGetLineMassflow(&value);
    Dose_DSVSetMassflow(m_sID, value);
}
//*************************************************************************************
//*************************************************************************************
void CIfsControl::SetStopSetpoint()
{
    LOGFUNCTION(m_eSubStep)

    float32_t value = 0.0F;
    Dose_DSVGetIfsSetpointOverflow(m_sID, &value);
    Dose_EXSetMBLineSetpoint(value);
    if (value == 0.0)
    {
        Dose_EXPBSetPBLineStop();
    }
}
//*************************************************************************************
//*************************************************************************************
void CIfsControl::EnterMaxOverflowReached()
{
    m_eSubStep = eSubStep::eMaxOverflowReached;
    LOGFUNCTIONSTEP(m_eSubStep)
    m_aAlarm.SetAlarm(m_st, base::eAlarmError::ERROR_IFS_INFEEDHOPPER_OVERFLOW, TRUE, base::eAlarmClass::eALARMTYP);
}
//*************************************************************************************
//*************************************************************************************
void CIfsControl::StepMaxOverflowReached()
{
    LOGFUNCTIONSTEP(m_eSubStep)

    if (IsHighSwitchSet())
    {
        EnterHighReached();
    }
    else
    {
        OnMaxOverflowReached();
    }
}
//*************************************************************************************
//*************************************************************************************
void CIfsControl::EnterHighOverflowReached()
{
    m_eSubStep = eSubStep::eHighOverflowReached;
    LOGFUNCTIONSTEP(m_eSubStep)

    m_aAlarm.SetAlarm(m_st, base::eAlarmError::ERROR_IFS_INFEEDHOPPER_OVERFLOW, TRUE, base::eAlarmClass::eALARMTYP);
}
//*************************************************************************************
//*************************************************************************************
void CIfsControl::StepHighOverflowReached()
{
    LOGFUNCTIONSTEP(m_eSubStep)
    OnHighOverflowReached();
}
//*************************************************************************************
//*************************************************************************************
void CIfsControl::EnterEmptyOverflowReached()
{
    m_eSubStep = eSubStep::eEmptyOverflowReached;
    LOGFUNCTIONSTEP(m_eSubStep)

    m_aAlarm.SetAlarm(m_st, base::eAlarmError::ERROR_IFS_INFEEDHOPPER_EMPTY, TRUE, base::eAlarmClass::eALARMTYP);
}
//*************************************************************************************
//*************************************************************************************
void CIfsControl::StepEmptyOverflowReached()
{
    LOGFUNCTIONSTEP(m_eSubStep)

    if (IsHighSwitchSet())
    {
        m_aAlarm.ClearAlarm(base::eAlarmError::ERROR_IFS_INFEEDHOPPER_EMPTY);
        EnterHighReached();
    }
    else if (IsMaxSwitchSet())
    {
        m_aAlarm.ClearAlarm(base::eAlarmError::ERROR_IFS_INFEEDHOPPER_EMPTY);
        EnterMaxReached();
    }
    else if (IsMinSwitchSet())
    {
        m_aAlarm.ClearAlarm(base::eAlarmError::ERROR_IFS_INFEEDHOPPER_EMPTY);
        EnterMinReached();
    }
    else if (IsLowSwitchSet())
    {
        m_aAlarm.ClearAlarm(base::eAlarmError::ERROR_IFS_INFEEDHOPPER_EMPTY);
        EnterLowReached();
    }
    else
    {
        OnEmptyOverflowReached();
    }
}
//*************************************************************************************
//*************************************************************************************
void CIfsControl::EnterHighReached()
{
    m_eSubStep = eSubStep::eHighReached;
    LOGFUNCTIONSTEP(m_eSubStep)
}
//*************************************************************************************
//*************************************************************************************
void CIfsControl::StepHighReached()
{
    LOGFUNCTIONSTEP(m_eSubStep)

    if ( IsHighSwitchSet())
    {
        OnHighReached();
    }
    else if (IsMaxSwitchSet())
    {
        EnterMaxReached();
    }
    else if (IsMinSwitchSet())
    {
        EnterMinReached();
    }
    else if (IsLowSwitchSet())
    {
        EnterLowReached();
    }
    else
    {
        EnterEmptyReached();
    }
}
//*************************************************************************************
//*************************************************************************************
void CIfsControl::EnterMaxReached()
{
    m_eSubStep = eSubStep::eMaxReached;
    LOGFUNCTIONSTEP(m_eSubStep)
}
//*************************************************************************************
//*************************************************************************************
void CIfsControl::StepMaxReached()
{
    LOGFUNCTIONSTEP(m_eSubStep)

    if (IsHighSwitchSet())
    {
        EnterHighReached();
    }
    else if (IsMaxSwitchSet())
    {
        OnMaxReached();
    }
    else if (IsMinSwitchSet())
    {
        EnterMinReached();
    }
    else if (IsLowSwitchSet())
    {
        EnterLowReached();
    }
    else
    {
        EnterEmptyReached();
    }
}
//*************************************************************************************
//*************************************************************************************
void CIfsControl::EnterMinReached()
{
    m_eSubStep = eSubStep::eMinReached;
    LOGFUNCTIONSTEP(m_eSubStep)
}
//*************************************************************************************
//*************************************************************************************
void CIfsControl::StepMinReached()
{
    LOGFUNCTIONSTEP(m_eSubStep)

    if (IsHighSwitchSet())
    {
        EnterHighReached();
    }
    else if (IsMaxSwitchSet())
    {
        EnterMaxReached();
    }
    else if (IsMinSwitchSet())
    {
        OnMinReached();
    }
    else if (IsLowSwitchSet())
    {
        EnterLowReached();
    }
    else
    {
        EnterEmptyReached();
    }
}
//*************************************************************************************
//*************************************************************************************
void CIfsControl::EnterLowReached()
{
    m_eSubStep = eSubStep::eLowReached;
    LOGFUNCTIONSTEP(m_eSubStep)
}
//*************************************************************************************
//*************************************************************************************
void CIfsControl::StepLowReached()
{
    LOGFUNCTIONSTEP(m_eSubStep)

    if (IsHighSwitchSet())
    {
        EnterHighReached();
    }
    else if (IsMaxSwitchSet())
    {
        EnterMaxReached();
    }
    else if (IsMinSwitchSet())
    {
        EnterMinReached();
    }
    else if (IsLowSwitchSet())
    {
        OnLowReached();
    }
    else
    {
        EnterEmptyReached();
    }
}
//*************************************************************************************
//*************************************************************************************
void CIfsControl::EnterEmptyReached()
{
    m_eSubStep = eSubStep::eEmptyReached;
    LOGFUNCTIONSTEP(m_eSubStep)
}
//*************************************************************************************
//*************************************************************************************
void CIfsControl::StepEmptyReached()
{
    LOGFUNCTIONSTEP(m_eSubStep)

    if (IsHighSwitchSet())
    {
        EnterHighReached();
    }
    else if (IsMaxSwitchSet())
    {
        EnterMaxReached();
    }
    else if (IsMinSwitchSet())
    {
        EnterMinReached();
    }
    else if (IsLowSwitchSet())
    {
        EnterLowReached();
    }
    else
    {
        OnEmptyReached();
    }
}
//*************************************************************************************
//*************************************************************************************
void CIfsControl::EnterInitReached()
{
    m_eSubStep = eSubStep::eInitReached;
    LOGFUNCTIONSTEP(m_eSubStep)
}
//*************************************************************************************
//*************************************************************************************
void CIfsControl::StepInitReached()
{
    LOGFUNCTIONSTEP(m_eSubStep)

    if (IsHighSwitchSet())
    {
        EnterHighReached();
    }
    else if (IsMaxSwitchSet())
    {
        EnterMaxReached();
    }
    else if (IsMinSwitchSet())
    {
        EnterMinReached();
    }
    else if (IsLowSwitchSet())
    {
        EnterLowReached();
    }
    else
    {
        EnterEmptyReached();
    }
}
//*********************************************************************************************
//*********************************************************************************************
void	CIfsControl::Control()
{
    auto result = IsStarted();
    if (result)
    {
        try
        {
            (this->*c_EditMap.at(m_eSubStep))();
        }
        catch (std::out_of_range)
        {
            assert(FALSE);
            LOGERROR("Error not found in map: " << _S32(m_eSubStep));
        }
    }
}

//*************************************************************************************
//*************************************************************************************
BOOL CIfsControl::Execute()
{
    assert(IsInit());

    auto result = CBaseClass::Execute();
    if (result)
    {
        m_AdsClient.Update(m_st);

        CheckInputs();
        // Freigabe
        BOOL bRelease = FALSE;
        Dose_DSVGetRelease(m_sID, &bRelease);

        //--------------------------------------
        switch (GetOperatingMode())
        {
        case eOperatingMode::RUNNING:
        {
            if (!bRelease)
            {
                Stop();
                SetOperatingMode(eOperatingMode::IDLE);
            }
            else
            {
                Control();
            }
        }
        break;

        case eOperatingMode::IDLE:
        default:
        {
            //--------------------------------------
            // sonst Freigabe erfolgt ?
            if ( ! bRelease)
            {
                if ( ! Stop())
                {
                    Restart();
                }
            }
            else
            {
                Start(m_st);
            }
        }
        break;
        }
        m_aAlarm.CheckWarningLevel(m_st, base::eAlarmError::ERROR_IFS_INFEEDHOPPER_EMPTY);
        m_aAlarm.CheckWarningLevel(m_st, base::eAlarmError::ERROR_IFS_INFEEDHOPPER_OVERFLOW);
        SetAlarmOutput();
    }
    return result;
}
//*************************************************************************************
//*************************************************************************************
BOOL CIfsControl::InitExecute(void)
{
    auto result = CBaseClass::InitExecute();
    if (result)
    {
        m_AdsClient.Init();
        Dose_DSVPBSetPBInline(m_sID);

        m_aAlarm.ClearAlarm(base::eAlarmError::ERROR_IFS_INFEEDHOPPER_EMPTY);
        m_aAlarm.ClearAlarm(base::eAlarmError::ERROR_IFS_INFEEDHOPPER_OVERFLOW);
    }
    return result;
}
//*************************************************************************************
//*************************************************************************************
BOOL CIfsControl::ExitExecute(void)
{
    auto result = CBaseClass::IsInit();
    if (result)
    {
        Stop();
        m_AdsClient.Exit();
        result = CBaseClass::ExitExecute();
    }
    return result;
}
//*************************************************************************************
//*************************************************************************************
BOOL CIfsControl::Start(const uint32_t t)
{
    auto result = CBaseClass::Start(t);
    if (result)
    {
        m_AdsClient.SetOperate(TRUE);
        EnterInitReached();
    }
    return result;
}
//*************************************************************************************
//*************************************************************************************
BOOL CIfsControl::Stop(void)
{
    auto result = CBaseClass::IsStarted();
    if (result)
    {
        m_AdsClient.SetOperate(FALSE);
        result = CBaseClass::Stop();
    }
    return result;
}





