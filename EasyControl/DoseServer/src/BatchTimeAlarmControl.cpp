//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Implementation of module BatchTimeAlarmControl.cpp
///
/// @file   BatchTimeAlarmControl.cpp
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
#include "BatchTimeAlarmControl.h"

using namespace base;

//*************************************************************************************************
//*************************************************************************************************
BOOL CBatchTimeAlarmControl::Start(uint32_t t)
{
	assert(m_pAlarm);
	auto result = CAlarmControl::Start(t);
	if (result)
	{
		m_pAlarm->SetStart(base::eAlarmError::ERROR_DOSE_BATCH_TIMEOUT, t);
	}
	return result;
}
//*************************************************************************************************
//*************************************************************************************************
void CBatchTimeAlarmControl::Check(const uint32_t t)
{
	assert(m_pAlarm);
	assert(IsStarted());

	BOOL bOperate = FALSE;
	Dose_DSVGetIOOperateOutput(m_sID, &bOperate);
	// assert(bOperate);
	if (! bOperate)
	{
		return;
	}
	uint32_t lDelayTime = 0;
	Dose_DSVGetStartupDelay(m_sID, &lDelayTime);
	uint32_t tStart = m_tStart + lDelayTime;

	uint32_t iBatchTime = 0;
	Dose_DSVGetAlarmMaxBatchTime(m_sID, &iBatchTime);
	BOOL bAlarm = (iBatchTime > 0) && (t > tStart + iBatchTime);
	if (bAlarm)
	{
		m_pAlarm->SetAlarm(t, eAlarmError::ERROR_DOSE_BATCH_TIMEOUT, TRUE, base::eAlarmClass::eALARMTYP);
	}

	Dose_DSVGetBatchTime(m_sID, &iBatchTime);
	BOOL bStop = (iBatchTime > 0) && (t > tStart + iBatchTime);
	if (bStop)
	{
		Dose_DSVPBSetPBDosiererStop(m_sID);
	}
}
//*************************************************************************************************
//*************************************************************************************************
BOOL CBatchTimeAlarmControl::Update(const uint32_t t)
{
	auto result = CAlarmControl::Update(t);
	if (result)
	{
		Check(t);
		assert(m_pAlarm);
		m_pAlarm->CheckWarningLevel(t, eAlarmError::ERROR_DOSE_BATCH_TIMEOUT);
	}
	return result;
}





