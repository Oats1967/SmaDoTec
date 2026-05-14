//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Implementation of module DosePerformanceAlarmControl
///
/// @file   DosePerformanceAlarmControl.cpp
///
///
/// @coypright Ing.büro Hafer
///            Branderweg 8A
///            D-91058 Erlangen
///
/// @author    Detlef Hafer
///
//------------------------------------------------------------------------------------
#include "DosePerformanceAlarmControl.h"
#include "DoseDataLib/include/DoseData.h"

using namespace base;


//*********************************************************************************************************
//*********************************************************************************************************
float32_t CDosePerformanceAlarmControl::GetDosePerformance() const
{
	base::calib::CalibType cfg;
	float32_t fMaxDosePerformance = 0.0F;

	Dose_DSVGetCalibCfg(m_sID, &cfg);
	for (uint32_t i = 0; i < cfg.count; i++)
	{
		fMaxDosePerformance = __max(fMaxDosePerformance, cfg.fDosePerformance[i]);
	}
	return fMaxDosePerformance;
}
//*********************************************************************************************************
//*********************************************************************************************************
BOOL CDosePerformanceAlarmControl::Start(const uint32_t t)
{
	assert(m_pAlarm);
	auto result = CAlarmControl::Start(t);
	m_bTriggeredOnce = FALSE;
	m_pAlarm->SetStart(eAlarmError::ERROR_DOSE_DOSEPERFORMANCE_HIGH, t);
	m_pAlarm->SetStart(eAlarmError::ERROR_DOSE_DOSEPERFORMANCE_LOW, t);
	return result;
}
//*********************************************************************************************************
//*********************************************************************************************************
BOOL CDosePerformanceAlarmControl::Stop()
{
	auto result = CAlarmControl::Stop();
	m_bTriggeredOnce = FALSE;
	return result;
}
//*********************************************************************************************************
//*********************************************************************************************************
BOOL CDosePerformanceAlarmControl::Update(const uint32_t t)
{
	assert(m_pAlarm);

	eEncoderState sEncoderState = eEncoderState::ENCODERSTATE_OFF;
	Dose_DSVGetEncoderState(m_sID, &sEncoderState);
	if (sEncoderState == eEncoderState::ENCODERSTATE_OFF)
	{
		m_pAlarm->ClearAlarm(eAlarmError::ERROR_DOSE_DOSEPERFORMANCE_HIGH);
		m_pAlarm->ClearAlarm(eAlarmError::ERROR_DOSE_DOSEPERFORMANCE_LOW);
		return TRUE;
	}
	assert(IsStarted());
	if (!IsStarted())
	{
		return FALSE;
	}
	uint32_t	lMaxfaktor = 0L;
	Dose_DSVGetAlarmDosePerformance(m_sID, &lMaxfaktor);
	if ((!m_bTriggeredOnce) && (lMaxfaktor > 0L))
	{
		float32_t   fDosePerformance = GetDosePerformance();;

		float32_t   fAktfaktor = 0.0f;
		Dose_DSVGetActualDoseperformance(m_sID, &fAktfaktor);

		const float32_t fMaxfaktor = lMaxfaktor * 0.01f;
		const float32_t fMax = fDosePerformance * (1.0f + fMaxfaktor);
		const BOOL bGross = fAktfaktor > fMax;
		const float32_t fMin = fDosePerformance * (1.0f - fMaxfaktor);
		const BOOL bKlein = fAktfaktor < fMin;

		const BOOL bAlarm = bGross || bKlein;
		m_bTriggeredOnce = bAlarm;
		// Alarm soll nur einmal auftreten

		m_pAlarm->SetAlarm(t, eAlarmError::ERROR_DOSE_DOSEPERFORMANCE_HIGH, bGross, eAlarmClass::eWARNTYP);
		m_pAlarm->SetAlarm(t, eAlarmError::ERROR_DOSE_DOSEPERFORMANCE_LOW, bKlein, eAlarmClass::eWARNTYP);
	}
	m_pAlarm->CheckWarningLevel(t, eAlarmError::ERROR_DOSE_DOSEPERFORMANCE_HIGH);
	m_pAlarm->CheckWarningLevel(t, eAlarmError::ERROR_DOSE_DOSEPERFORMANCE_LOW);
	return TRUE;
}






