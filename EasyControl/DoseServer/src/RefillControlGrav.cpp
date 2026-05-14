//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Implementation of module RefillControlGrav
///
/// @file   RefillControlGrav.cpp
///
///
/// @coypright Ing.büro Hafer
///            Branderweg 8A
///            D-91058 Erlangen
///
/// @author    Detlef Hafer
///
//------------------------------------------------------------------------------------
#include "BASE/include/LCType.h"
#include "BASE/Utils/public/WeightPair.h"
#include "DoseDataLib/include/DoseData.h"
#include "RefillControlGrav.h"
#include "WeightControl.h"
#include "Logger/include/Logger.h"


using namespace base;

#define HARDSTOPDELAY 3U

//**************************************************************************************************
//**************************************************************************************************
CRefillControlGrav::CRefillControlGrav(int32_t id) : CBaseClass(id)
, m_pWeightCtrl{  nullptr }
, m_t0(0)
, m_delay(0)
{
}
//*****************************************************************************************************
//*****************************************************************************************************
void CRefillControlGrav::UpdateWeight()
{
	assert(m_pWeightCtrl);
	m_pWeightCtrl->Update(m_st);

	if ( ! m_RefillBits.flags.ScaleStartInit)
	{
		m_RefillBits.flags.ScaleStartInit = m_pWeightCtrl->isLoadcellStartInit();
	}
	BOOL bScale = FALSE;
	if ( m_RefillBits.flags.ScaleStartInit )
	{
		if ( ! m_RefillBits.flags.HardStopEnable )
		{
			if ( ! m_RefillBits.flags.HardStopInit)
			{
				m_RefillBits.flags.HardStopInit = true;
				m_delay = m_st;
			}
			else if (m_st > m_delay + HARDSTOPDELAY)
			{
				m_RefillBits.flags.HardStopEnable = true;
			}
			else {}
		}
		m_RefillBits.flags.ScaleEnable = m_pWeightCtrl->isLoadcellEnabled();
		m_RefillBits.flags.ScaleActive = m_pWeightCtrl->isLoadcellActive();
		bScale = IsScale();
		if (bScale)
		{
			const auto& rWeight = m_pWeightCtrl->GetWeight();
			const float32_t fWeight = rWeight.m_fWeight;

			float32_t fWMin = 0.0f;
			float32_t fWMax = 0.0f;
			float32_t fWMinMin = 0.0f;
			Dose_DSVGetRefillMinMaxLimit(m_sID, &fWMin, &fWMax, &fWMinMin);
			m_RefillBits.flags.WeightAboveMax = (fWeight >= fWMax);
			m_RefillBits.flags.WeightBelowMin = (fWeight < fWMin);
			m_RefillBits.flags.FeederEmpty = (fWeight < fWMinMin);
			m_RefillBits.flags.LeftEmptyFeeder = (fWeight > 1.02F * fWMinMin);
		}
	}
	if ( ! bScale )
	{
		m_RefillBits.flags.WeightAboveMax = false;
		m_RefillBits.flags.WeightBelowMin = false;
		m_RefillBits.flags.FeederEmpty = false;
		m_RefillBits.flags.LeftEmptyFeeder = false;
	}
}

//*****************************************************************************************************
//*****************************************************************************************************
void CRefillControlGrav::CheckRefillAlarm(void)
{
	if ( ! IsScale())
	{
		assert(m_pAlarm);
		m_pAlarm->ClearAlarm(eAlarmError::ERROR_DOSE_REFILL_TIMEOUT);
		m_pAlarm->ClearAlarm(eAlarmError::ERROR_LWF_REFILLALARMLIMIT);
		return;
	}
	CBaseClass::CheckRefillAlarm();
}
//**************************************************************************************************
//**************************************************************************************************
BOOL CRefillControlGrav::GetRefillControlEnable() const
{
	auto result = CBaseClass::GetRefillControlEnable();
	result |= IsScale();
	return result;
}
//**************************************************************************************************
//**************************************************************************************************
void CRefillControlGrav::OnRefillStart()
{
	StoreLCPriority();
}
//**************************************************************************************************
//**************************************************************************************************
void CRefillControlGrav::OnRefillStop()
{
	RestoreLCPriority();
}
//**************************************************************************************************
//**************************************************************************************************
BOOL CRefillControlGrav::GetHardStop() const
{
	auto bBaseStop			= CBaseClass::GetHardStop();
	BOOL bScale				= IsScale();
	BOOL bHardStopEnable	= IsHardStopEnable();
	BOOL bStop				= bBaseStop || (bHardStopEnable && ( ! bScale ));
#if _DEBUG
	if (bStop)
	{
		LOGDEBUG(__FUNCTION__ << " id = " << m_sID << 
					" bBaseStop = " << bBaseStop <<
					" bStartInit = " << BOOL(m_RefillBits.flags.ScaleStartInit) <<
					" bHardStop = " << BOOL(m_RefillBits.flags.HardStopEnable) <<
					" m_RefillBits.flags.ScaleEnable = " << BOOL(m_RefillBits.flags.ScaleEnable) <<
					" m_RefillBits.flags.ScaleActive = " << BOOL(m_RefillBits.flags.ScaleActive) <<
					" bScale = " << bScale << "\n");
	}
#endif

	return bStop;
}
//**************************************************************************************************
//**************************************************************************************************
void CRefillControlGrav::registerAlarmManager(CAlarmManager& rAlarm)
{
	CBaseClass::registerAlarmManager(rAlarm);
	if (m_pWeightCtrl)
	{
		m_pWeightCtrl->registerAlarmManager(rAlarm);
	}
}
//**************************************************************************************************
//**************************************************************************************************
void CRefillControlGrav::registerWeightCtrl(CWeightControl& rWeightCtrl)
{
	m_pWeightCtrl = &rWeightCtrl;
}

//*****************************************************************************************************
//*****************************************************************************************************
BOOL CRefillControlGrav :: Update (const uint32_t t)
{
	auto result = IsModuleEnabled();
	if (result)
	{
		UpdateWeight();
		result = CBaseClass::Update(t);
	}
	return result;
}
//**************************************************************************************************
//**************************************************************************************************
void CRefillControlGrav::StoreLCPriority()
{
	if (IsScale())
	{
		assert(m_pWeightCtrl);
		if (m_pWeightCtrl->GetPriority() != base::LC_PRIORITY::LC_PRIORITY_HIGH)
		{
			m_pWeightCtrl->SetPriority(base::LC_PRIORITY::LC_PRIORITY_HIGH);
		}
	}
}
//**************************************************************************************************
//**************************************************************************************************
void CRefillControlGrav::RestoreLCPriority()
{
	if (IsScale())
	{
		BOOL bRun = FALSE;
		Dose_DSVGetIOOperateOutput(m_sID, &bRun);
		if ( ! bRun)
		{
			assert(m_pWeightCtrl);
			m_pWeightCtrl->SetPriority(base::LC_PRIORITY::LC_PRIORITY_NORMAL);
		}
#if _DEBUG
		else
		{
			assert(m_pWeightCtrl);
			assert(m_pWeightCtrl->GetPriority() == base::LC_PRIORITY::LC_PRIORITY_HIGH);
		}
#endif
	}
}
//**************************************************************************************************
//**************************************************************************************************
void CRefillControlGrav::CheckRefill(void)
{
	if (IsScale())
	{
		CBaseClass::CheckRefill();
	}
}





