//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Implementation of module ModuleTotalizerPulseControl
///
/// @file   ModuleTotalizerPulseControl.cpp
///
///
/// @coypright Ing.büro Hafer
///            Branderweg 8A
///            D-91058 Erlangen
///
/// @author    Detlef Hafer
///
//------------------------------------------------------------------------------------
#include <cassert>
#include "ModuleTotalizerPulseControl.h"
#include "DoseDataLib/include/DoseData.h"


//*********************************************************************************************************************
//*********************************************************************************************************************
CModuleTotalizerPulseControl::CModuleTotalizerPulseControl(int32_t id) : CBaseClass(id)
, m_TriggerCount{ 0 }
, m_StartTotalizer{ 0.0F }
, m_LastTotalizer{ 0.0F }
, m_ImpulseStep{ 0.0F }
, m_Totalizer{ 0.0F }
{
}
//*********************************************************************************************************************
//*********************************************************************************************************************
BOOL CModuleTotalizerPulseControl::Start(const uint32_t t)
{
    auto result = CBaseClass::Start(t);
    if (result)
    {
        Dose_EXPopTotalizerPulseStep(&m_ImpulseStep);
        Dose_EXGetTotalizerAll(&m_Totalizer);
        m_LastTotalizer = m_StartTotalizer = m_Totalizer;
        m_TriggerCount = 0;
    }
    return result;
}
//*********************************************************************************************************************
//*********************************************************************************************************************
BOOL CModuleTotalizerPulseControl::IsAnyDoseStarted()
{
    BOOL bStarted = FALSE;
    Dose_EXGetAnyDoseStarted(&bStarted);
    return bStarted;
}
//*********************************************************************************************************************
//*********************************************************************************************************************
void CModuleTotalizerPulseControl::TriggerPulse(uint32_t t)
{
    BOOL bChanged = FALSE;
    Dose_EXGetChangedTotalizerPulseStep(&bChanged);
    if (bChanged)
    {
        Dose_EXPopTotalizerPulseStep(&m_ImpulseStep);
        assert(m_ImpulseStep > 0.0F);
        m_TriggerCount = 0;
    }
    BOOL bTotalizerChanged = FALSE;
    Dose_EXGetChangedTotalizerAll(&bTotalizerChanged);
    if (bTotalizerChanged)
    {
        Dose_EXPopTotalizerAll(&m_Totalizer);
        if (m_Totalizer < m_LastTotalizer)
        {
            // Totalizer nullen wurde gedrückt
            m_StartTotalizer = m_Totalizer;
            m_TriggerCount = 0;
        }
    }
    if (bTotalizerChanged && (m_ImpulseStep > 0.0F))
    {
        float32_t TotDiff = m_Totalizer - m_StartTotalizer;
        assert(TotDiff >= 0.0F);
        auto count = _U32(TotDiff / m_ImpulseStep);
        if (count > m_TriggerCount)
        {
            // Jetzt Trigger setzen
            assert(count >= m_TriggerCount + 1);
            m_TriggerCount = count;
            uint32_t duration = 0;
            Dose_EXGetTotalizerPulseDuration(&duration);
            m_PulseThread.TriggerPulse(duration);
        }
        m_LastTotalizer = m_Totalizer;
    }
}

//*********************************************************************************************************************
//*********************************************************************************************************************
BOOL CModuleTotalizerPulseControl::Execute()
{
    auto result = CBaseClass::Execute();
    if (result)
    {
        BOOL bAnyDoseStarted = IsAnyDoseStarted();
        if ( ! IsStarted())
        {
            if (bAnyDoseStarted)
            {
                Start(m_st);
            }
        }
        else 
        {
            if (bAnyDoseStarted)
            {
                TriggerPulse(m_st);
            }
            else
            {
                Stop();
            }
        }
    }
    return result;
}
//*********************************************************************************************************************
//*********************************************************************************************************************
BOOL CModuleTotalizerPulseControl::InitExecute()
{
    auto result = CBaseClass::InitExecute();
    if (result)
    {
        m_PulseThread.Open();
    }
    return result;
}
//*********************************************************************************************************************
//*********************************************************************************************************************
BOOL CModuleTotalizerPulseControl::ExitExecute()
{
    auto result = CBaseClass::IsInit();
    if (result)
    {
        m_PulseThread.Close();
        CBaseClass::ExitExecute();
    }
    return result;
}


