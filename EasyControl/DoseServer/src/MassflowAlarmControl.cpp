//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Implementation of module MassflowAlarmControl
///
/// @file   MassflowAlarmControl.cpp
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
#include "MassflowAlarmControl.h"

using namespace base;

//*************************************************************************************************
//*************************************************************************************************
BOOL CMassflowAlarmControl::Start(uint32_t t)
{
	assert(m_pAlarm);
	auto result = CAlarmControl::Start(t);
	m_pAlarm->SetStart(base::eAlarmError::ERROR_DOSE_MASSFLOW_HIGH, t);
	m_pAlarm->SetStart(base::eAlarmError::ERROR_DOSE_MASSFLOW_LOW, t);
	return result;
}
//*************************************************************************************************
//*************************************************************************************************
void CMassflowAlarmControl::Check(const uint32_t t)
{
	assert(m_pAlarm);
	assert(IsStarted());
	if (!IsStarted())
	{
		return;
	}
	BOOL bOperate = FALSE;
	Dose_DSVGetIOOperateOutput(m_sID, &bOperate);
	if (! bOperate)
	{
		return;
	}
	if (t > m_tStart + m_delay)
	{
		float32_t iMassflow = 0.0F;
		Dose_DSVGetMassflow(m_sID, &iMassflow);

		float32_t iSetpoint = 0.0F;
		Dose_DSVGetActualSetpoint(m_sID, &iSetpoint);

		BOOL	bGross = FALSE;
		BOOL	bKlein = FALSE;
		uint32_t	lHoch = 0L;
		uint32_t	lTief = 0L;
		Dose_DSVGetAlarmMassflowHigh(m_sID, &lHoch);
		const float32_t fAkt = iMassflow;
		const float32_t fMax = iSetpoint * (100L + lHoch) / 100.0f;
		if (fAkt > fMax)
		{
			bGross = TRUE;
		}
		else
		{
			Dose_DSVGetAlarmMassflowLow(m_sID, &lTief);
			const float32_t fMin = iSetpoint * (100L - lTief) / 100.0f;
			bKlein = (fAkt < fMin);
		}
		m_pAlarm->SetAlarm(t, eAlarmError::ERROR_DOSE_MASSFLOW_HIGH, bGross, eAlarmClass::eWARNTYP);
		m_pAlarm->SetAlarm(t, eAlarmError::ERROR_DOSE_MASSFLOW_LOW, bKlein, eAlarmClass::eWARNTYP);
	}
}
//*************************************************************************************************
//*************************************************************************************************
BOOL CMassflowAlarmControl::Update(const uint32_t t)
{
	auto result = CAlarmControl::Update(t);
	if (result)
	{
		Check(t);
		m_pAlarm->CheckWarningLevel(t, eAlarmError::ERROR_DOSE_MASSFLOW_HIGH);
		m_pAlarm->CheckWarningLevel(t, eAlarmError::ERROR_DOSE_MASSFLOW_LOW);
	}
	return result;
}





