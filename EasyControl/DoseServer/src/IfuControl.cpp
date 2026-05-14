//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Implementation of module IfuControl
///         Underfeeding control
///
/// @file   IfuControl.cpp
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
#include "IfuControl.h"
#include "DoseDataLib/include/DoseData.h"
#include "Logger/include/Logger.h"
#include "IfsMacros.hxx"


//*************************************************************************************
//*************************************************************************************
void CIfuControl::Restart()
{
    if ( ( m_eSubStep == eSubStep::eMaxOverflowReached ) || (m_eSubStep == eSubStep::eHighOverflowReached ) )
    {
        if ( ! IsMinSwitchSet())
        {
            SetNormalSetpoint();
            Dose_EXPBSetPBLineStart();
            EnterInitReached();
        }
    }
}
//*************************************************************************************
//*************************************************************************************
void CIfuControl::EnterLowReached()
{
    CBaseClass::EnterLowReached();

    uint32_t lTimeOut = 0L;
    Dose_DSVGetIfsStepTimeGain(m_sID, &lTimeOut);
    if (lTimeOut > 0)
    {
        IncreaseSetpoint();
    }
    m_t0 = m_st;
}
//*************************************************************************************
//*************************************************************************************
void CIfuControl::OnLowReached()
{
    uint32_t lTimeOut = 0L;
    Dose_DSVGetIfsStepTimeGain(m_sID, &lTimeOut);
    BOOL bStepTime = (lTimeOut > 0) && (m_st >= m_t0 + lTimeOut);
    if (bStepTime)
    {
        m_t0 = m_st;
        IncreaseSetpoint();
    }
}
//*************************************************************************************
//*************************************************************************************
void CIfuControl::EnterMinReached()
{
    CBaseClass::EnterMinReached();

    uint32_t lTimeOut = 0L;
    Dose_DSVGetIfsStepTimeReduce(m_sID, &lTimeOut);
    BOOL bStepTime = (lTimeOut > 0);
    if (bStepTime)
    {
        ReduceSetpoint();
    }
    m_t0 = m_st;
}
//*************************************************************************************
//*************************************************************************************
void CIfuControl::OnMinReached()
{
    uint32_t lTimeOut = 0L;
    Dose_DSVGetIfsStepTimeReduce(m_sID, &lTimeOut);
    BOOL bStepTime = (lTimeOut > 0) && (m_st >= m_t0 + lTimeOut);
    if (bStepTime)
    {
        m_t0 = m_st;
        ReduceSetpoint();
    }
}
//*************************************************************************************
//*************************************************************************************
void CIfuControl::EnterMaxReached()
{
    CBaseClass::EnterMaxReached();

    uint32_t lTimeOut = 0L;
    Dose_DSVGetIfsStepTimeReduce(m_sID, &lTimeOut);
    if ( lTimeOut > 0 )
    {
        ReduceSetpoint();
    }
    m_t0 = m_tEF = m_st;
}
//*************************************************************************************
//*************************************************************************************
void CIfuControl::OnMaxReached()
{
    // Teste, ob schon lange auf High -> Dosierer Überlauf
    uint32_t lTimeOut = 0L;
    Dose_DSVGetIfsFeederOverflowTimeOut(m_sID, &lTimeOut);
    BOOL bOverflowReached = (lTimeOut > 0) && (m_st > m_tEF + lTimeOut);
    if (bOverflowReached)
    {
        EnterMaxOverflowReached();
    }
    else
    {
        Dose_DSVGetIfsStepTimeReduce(m_sID, &lTimeOut);
        BOOL bStepTime = (lTimeOut > 0) && (m_st >= m_t0 + lTimeOut);
        if (bStepTime)
        {
            m_t0 = m_st;
            ReduceSetpoint();
        }
    }
}
//*************************************************************************************
//*************************************************************************************
void CIfuControl::EnterHighReached()
{
    uint32_t lTimeOut = 0L;
    Dose_DSVGetIfsStepTimeReduce(m_sID, &lTimeOut);
    if (lTimeOut > 0)
    {
        ReduceSetpoint();
    }
    m_t0 = m_tEF = m_st;
}
//*************************************************************************************
//*************************************************************************************
void CIfuControl::OnHighReached()
{
    // Teste, ob schon lange auf High -> Dosierer Überlauf
    uint32_t lTimeOut = 0L;
    Dose_DSVGetIfsFeederOverflowTimeOut(m_sID, &lTimeOut);
    BOOL bOverflowReached = (lTimeOut > 0) && (m_st > m_tEF + lTimeOut);
    if (bOverflowReached)
    {
        EnterHighOverflowReached();
    }
    else
    {
        Dose_DSVGetIfsStepTimeReduce(m_sID, &lTimeOut);
        BOOL bStepTime = (lTimeOut > 0) && (m_st >= m_t0 + lTimeOut);
        if (bStepTime)
        {
            m_t0 = m_st;
            ReduceSetpoint();
        }
    }
}
//*************************************************************************************
//*************************************************************************************
void CIfuControl::EnterEmptyReached()
{
    CBaseClass::EnterEmptyReached();

    uint32_t lTimeOut = 0L;
    Dose_DSVGetIfsStepTimeGain(m_sID, &lTimeOut);
    if (lTimeOut > 0)
    {
        IncreaseSetpoint();
    }
    m_t0 = m_tEF = m_st;
}
//*************************************************************************************
//*************************************************************************************
void CIfuControl::OnEmptyReached()
{
    // No Min/Max/Low
    // Teste, ob schon lange unter Min -> Dosierer Leer
    uint32_t lTimeOut = 0L;
    Dose_DSVGetIfsFeederEmptyTimeOut(m_sID, &lTimeOut);
    BOOL bEmptyFeederReached = (lTimeOut > 0) && (m_st >= m_tEF + lTimeOut);
    if (bEmptyFeederReached)
    {
        EnterEmptyOverflowReached();
    }
    else
    {
        Dose_DSVGetIfsStepTimeGain(m_sID, &lTimeOut);
        BOOL bStepTime = (lTimeOut > 0) && (m_st >= m_t0 + lTimeOut);
        if (bStepTime)
        {
            m_t0 = m_st;
            IncreaseSetpoint();
        }
    }
 }
//*************************************************************************************
//*************************************************************************************
void CIfuControl::EnterInitReached()
{
    CBaseClass::EnterInitReached();
    m_t0 = m_st;
}
//*************************************************************************************
//*************************************************************************************
void CIfuControl::StepInitReached()
{
    // Warte erst eine Zeit ab bevor der nächste Schritt gemacht wird
    BOOL bStep = TRUE;
    uint32_t lTimeOutGain = 0L;
    Dose_DSVGetIfsStepTimeGain(m_sID, &lTimeOutGain);
    uint32_t lTimeOutReduce = 0L;
    Dose_DSVGetIfsStepTimeReduce(m_sID, &lTimeOutReduce);
    uint32_t lTimeOut = __max(lTimeOutGain, lTimeOutReduce);
    if (lTimeOut > 0)
    {
        bStep = (m_st >= m_t0 + lTimeOut);
    }
    if (bStep)
    {
        CBaseClass::StepInitReached();
    }
}
//*************************************************************************************
//*************************************************************************************
void CIfuControl::EnterEmptyOverflowReached()
{
    CBaseClass::EnterEmptyOverflowReached();

    uint32_t lTimeOut = 0L;
    Dose_DSVGetIfsStepTimeGain(m_sID, &lTimeOut);
    if (lTimeOut > 0)
    {
        IncreaseSetpoint();
    }
    m_t0 = m_st;
}
//*************************************************************************************
//*************************************************************************************
void CIfuControl::OnEmptyOverflowReached()
{
    uint32_t lTimeOut = 0L;
    Dose_DSVGetIfsStepTimeGain(m_sID, &lTimeOut);
    BOOL bStepTime = (lTimeOut > 0) && (m_st >= m_t0 + lTimeOut);
    if (bStepTime)
    {
        m_t0 = m_st;
        IncreaseSetpoint();
    }
}
//*************************************************************************************
//*************************************************************************************
void CIfuControl::EnterMaxOverflowReached()
{
    CBaseClass::EnterMaxOverflowReached();
    SetStopSetpoint();
}
//*************************************************************************************
//*************************************************************************************
void CIfuControl::OnMaxOverflowReached()
{
    if ( ! IsMinSwitchSet())
    {
        m_aAlarm.ClearAlarm(base::eAlarmError::ERROR_IFS_INFEEDHOPPER_OVERFLOW);
        SetNormalSetpoint();
        EnterInitReached();
    }
    else
    {
        BOOL bSetAlarm = IsMaxSwitchSet();
        if (bSetAlarm)
        {
            if ( ! m_aAlarm.IsAlarm(base::eAlarmError::ERROR_IFS_INFEEDHOPPER_OVERFLOW))
            {
                m_aAlarm.SetAlarm(m_st, base::eAlarmError::ERROR_IFS_INFEEDHOPPER_OVERFLOW, TRUE, base::eAlarmClass::eALARMTYP);
            }
        }
        else
        {
            m_aAlarm.ClearAlarm(base::eAlarmError::ERROR_IFS_INFEEDHOPPER_OVERFLOW);
        }
    }
}
//*************************************************************************************
//*************************************************************************************
void CIfuControl::EnterHighOverflowReached()
{
    CBaseClass::EnterHighOverflowReached();
    SetStopSetpoint();
}
//*************************************************************************************
//*************************************************************************************
void CIfuControl::OnHighOverflowReached()
{
    if ( ! IsMinSwitchSet())
    {
        m_aAlarm.ClearAlarm(base::eAlarmError::ERROR_IFS_INFEEDHOPPER_OVERFLOW);
        SetNormalSetpoint();
        EnterInitReached();
    }
    else
    {
        BOOL bSetAlarm = IsHighSwitchSet();
        if (bSetAlarm)
        {
            if (!m_aAlarm.IsAlarm(base::eAlarmError::ERROR_IFS_INFEEDHOPPER_OVERFLOW))
            {
                m_aAlarm.SetAlarm(m_st, base::eAlarmError::ERROR_IFS_INFEEDHOPPER_OVERFLOW, TRUE, base::eAlarmClass::eALARMTYP);
            }
        }
        else
        {
            m_aAlarm.ClearAlarm(base::eAlarmError::ERROR_IFS_INFEEDHOPPER_OVERFLOW);
        }
    }
}







