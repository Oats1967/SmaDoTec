//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Implementation of module NoiseAlarmControl
///
/// @file   NoiseAlarmControl.cpp
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
#include "DoseDataLib/include/DoseData.h"
#include "NoiseAlarmControl.h"

using namespace base;


//--------------------------------------------------------------------------------------------
BOOL CNoiseAlarmControl::Start(const uint32_t t)
{
    assert(m_pAlarm);
    auto result = CAlarmControl::Start(t);
    m_t0 = t;
    m_bAlarmTriggered = FALSE;
    m_pAlarm->SetStart(eAlarmError::ERROR_DOSE_LC_INVALIDWEIGHT, t);
    Dose_DSVSetPidGatefilterState(m_sID, eGatefilter::GATEFILTER_NONE);
    return result;
}
//--------------------------------------------------------------------------------------------
BOOL CNoiseAlarmControl::Stop()
{
    m_t0 = 0;
    m_bAlarmTriggered = FALSE;
    return CAlarmControl::Stop();
}
//-------------------------------------------------------------------------------------
BOOL CNoiseAlarmControl::Update(const uint32_t t)
{
    assert(m_pAlarm);

    float32_t fAlarmNoiseLimit = 0.0f;
    float32_t fRauschGroesse = 0.0f;

    assert(IsStarted());

    if ( ! IsStarted())
    {
        return FALSE;
    }
    eEncoderState	sEncoderState = eEncoderState::ENCODERSTATE_OFF;
    Dose_DSVGetEncoderState(m_sID, &sEncoderState);
    if (sEncoderState == eEncoderState::ENCODERSTATE_OFF)
    {
        m_pAlarm->SetAlarm(t, eAlarmError::ERROR_DOSE_LC_INVALIDWEIGHT, FALSE);
        Dose_DSVSetPidGatefilterState(m_sID, eGatefilter::GATEFILTER_NONE);
        return TRUE;
    }
    BOOL gravvolumetric = FALSE;
    BOOL volumetric = FALSE;

    Dose_DSVGetVolumetricState(m_sID, &volumetric);  // True -> volumetric dosing unit
    Dose_DSVGetGravVolState(m_sID, &gravvolumetric);   // True -> forced volumetric
    volumetric |= gravvolumetric;
    if ( ! volumetric)
    {
        Dose_DSVGetActualNoise(m_sID, &fRauschGroesse);
        Dose_DSVGetAlarmNoiseLimit(m_sID, &fAlarmNoiseLimit);
        if (fRauschGroesse * 100 >= fAlarmNoiseLimit)
        {
            if (! m_pAlarm->IsAlarm(base::eAlarmError::ERROR_DOSE_LC_INVALIDWEIGHT))
            {
                // wait 5 seconds
                if (! m_bAlarmTriggered)
                {
                    m_bAlarmTriggered = TRUE;
                    m_t0 = t;
                    Dose_DSVSetPidGatefilterState(m_sID, eGatefilter::GATEFILTER_YELLOW);
                }
                else
                {
                    if (t > m_t0 + 5U)
                    {
                        m_pAlarm->SetAlarm(t, eAlarmError::ERROR_DOSE_LC_INVALIDWEIGHT, TRUE, eAlarmClass::eWARNTYP);
                        Dose_DSVSetPidGatefilterState(m_sID, eGatefilter::GATEFILTER_RED);
                    }
                }
            }
        }
        else
        {
            m_pAlarm->SetAlarm(t, eAlarmError::ERROR_DOSE_LC_INVALIDWEIGHT, FALSE);
            Dose_DSVSetPidGatefilterState(m_sID, eGatefilter::GATEFILTER_GREEN);
            m_bAlarmTriggered = FALSE;
        }
    }
    else
    {
        // volumetric
        m_bAlarmTriggered = FALSE;
        if (m_pAlarm->IsAlarm(eAlarmError::ERROR_DOSE_LC_INVALIDWEIGHT))
        {
            m_pAlarm->ClearAlarm(eAlarmError::ERROR_DOSE_LC_INVALIDWEIGHT);
        }
        Dose_DSVSetPidGatefilterState(m_sID, (gravvolumetric) ? eGatefilter::GATEFILTER_NONE : eGatefilter::GATEFILTER_GREEN);
    }
    m_pAlarm->CheckWarningLevel(m_sID, eAlarmError::ERROR_DOSE_LC_INVALIDWEIGHT);
    return TRUE;
}





