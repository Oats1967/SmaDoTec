//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Implementation of module IflControl
///
/// @file   IflControl.cpp
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
#include "IflControl.h"
#include "BASE/include/LCType.h"
#include "DoseDataLib/include/DoseData.h"
#include "BASE/Base.def"


//*************************************************************************************
//*************************************************************************************
static float32_t ROUNDSETPOINT(const float32_t f)
{
	return (((f) < 10.0F) ? (f) : NEXTLONG(f));
}
//*************************************************************************************
//*************************************************************************************
CIflControl::CIflControl(int32_t id, loadcell::ILCModuleInterface& rlc) : CBaseClass(id)
, m_WeightCtrl{ id }
, m_TareTask{ id }
, m_bExternalSetpointChanged{ FALSE }
, m_fWeight{ 0.0F }
, m_fMinLevel{ 0.0F }
, m_fMaxLevel{ 0.0F }
, m_fAlarmLimit{ 0.0F }
, m_fSetpointMax{ 0.0F }
, m_fActSetpoint{ 0.0F }
, m_fActMaxSetpoint{ 0.0F }
, m_fActMinSetpoint{ 0.0F }
, m_SampleTime{ 1U }
, m_tWeightNext{ 0 }
, m_tNext{ 0 }
, m_tMinStart{ 0 }
, m_tMaxStart{ 0 }
, m_eSubSteps { eSubSteps ::eInit}

{
	m_WeightCtrl.registerAlarmManager(this->m_aAlarm);
	m_WeightCtrl.registerLoadcell(rlc);
	m_TareTask.registerAlarmManager(this->m_aAlarm);
	m_TareTask.registerWeightCtrl(m_WeightCtrl);
}
//*************************************************************************************
//*************************************************************************************
inline void CIflControl::SetLineSetpoint(float32_t fSetpoint)
{
	fSetpoint = ROUNDSETPOINT(fSetpoint);
	Dose_EXSetMBLineSetpoint(fSetpoint);
	Dose_DSVSetActualSetpoint(m_sID, fSetpoint);
}
//*************************************************************************************
//*************************************************************************************
BOOL CIflControl::GetLineSetpoint() const
{
	const float32_t c_epsilon = 1e-04F;

	float32_t fNewSetpoint = 0.0F;
	float32_t fSaveSetpoint = 0.0F;
	Dose_EXGetLineSetpoint(&fNewSetpoint);
	Dose_DSVGetActualSetpoint(m_sID, &fSaveSetpoint);
	BOOL bChanged = _F32(fabs(fNewSetpoint - fSaveSetpoint)) > c_epsilon;
	if (bChanged)
	{
		Dose_DSVSetNominalSetpoint(m_sID, fNewSetpoint);
		Dose_DSVSetActualSetpoint(m_sID, fNewSetpoint);
	}
	return bChanged;
}
//*************************************************************************************
//*************************************************************************************
void CIflControl::GetWeight()
{
	auto result = m_WeightCtrl.Update(m_st);
	if (result)
	{
		m_aLoadCell0 = m_WeightCtrl.GetWeight();
		m_fWeight = m_aLoadCell0.m_fWeight;
	}
	Dose_DSVGetLclWeightMinLevel(m_sID, &m_fMinLevel);
	Dose_DSVGetLclWeightMaxLevel(m_sID, &m_fMaxLevel);
	Dose_DSVGetLclWeightAlarmLimit(m_sID, &m_fAlarmLimit);
	Dose_DSVGetIflLineSetpointMax(m_sID, &m_fSetpointMax);
}
//*********************************************************************************************
//*********************************************************************************************
inline void CIflControl::InitWeight(void)
{
	const uint32_t t = __min(2U * m_SampleTime, 20U);
	m_aLoadCell1 = m_aLoadCell0;
	m_tWeightNext = m_st + t;
}
//*********************************************************************************************
//*********************************************************************************************
inline BOOL CIflControl::UpdateWeight(void)
{
	BOOL bWeightUpdate = (m_aLoadCell0.m_ulT != m_aLoadCell1.m_ulT) && (m_st >= m_tWeightNext);
	if (bWeightUpdate)
	{
		//assert(m_SampleTime >= 2u);
		m_tWeightNext = m_st + m_SampleTime;
		m_aLoadCell1 = m_aLoadCell0;
	}
	return bWeightUpdate;
}

//*******************************************************************************************************
//*******************************************************************************************************
void CIflControl::GetSampleTime(void)
{
	Dose_DSVPopPidSampleInterval(m_sID, &m_SampleTime); // [0..1]
	m_SampleTime = __max(m_SampleTime, 2U);
}
//*************************************************************************************
//*************************************************************************************
BOOL CIflControl :: Start(const uint32_t t)
{
    auto result = CBaseClass::Start(t);
    if (result)
    {
		m_WeightCtrl.SetPriority(base::LC_PRIORITY::LC_PRIORITY_HIGH);
		InitWeight();
		EnterInitLevel();
	}
	return result;
}
//*************************************************************************************
//*************************************************************************************
BOOL CIflControl :: Stop ( void)
{
    auto result = CBaseClass::IsStarted();
    if (result)
    {
		m_WeightCtrl.SetPriority(base::LC_PRIORITY::LC_PRIORITY_NORMAL);
		result = CBaseClass::Stop();
    }
    return result;
}
//*************************************************************************************
//*************************************************************************************
BOOL CIflControl :: InitExecute ( void)
{
    auto result = CBaseClass::InitExecute();
    if (result)
    {
		m_bExternalSetpointChanged = FALSE;
		Dose_EXGetLineSetpoint(&m_fActSetpoint);
		Dose_DSVSetActualSetpoint(m_sID, m_fActSetpoint);
		Dose_DSVSetNominalSetpoint(m_sID, m_fActSetpoint);
		Dose_DSVSetLclWeightMaxLevelActive(m_sID, FALSE);
		Dose_DSVSetLclWeightMinLevelActive(m_sID, FALSE);
		m_WeightCtrl.InitExecute();
		m_WeightCtrl.Start(m_st);
	}
    return result;
}
//*************************************************************************************
//*************************************************************************************
BOOL CIflControl :: ExitExecute ( void)
{
    auto result = CBaseClass::IsInit();
    if (result)
    {
		Stop();
		m_WeightCtrl.Stop();
        result = CBaseClass::ExitExecute();
    }
    return result;
}
//*********************************************************************************************
//*********************************************************************************************
void CIflControl::CheckAlarm()
{
	switch (m_eSubSteps)
	{
	case eSubSteps::eMinLevel:
	{
		if (!m_aAlarm.IsAlarm(base::eAlarmError::ERROR_DOSE_LC_MINWEIGHT))
		{
			m_aAlarm.SetAlarm(m_st, base::eAlarmError::ERROR_DOSE_LC_MINWEIGHT, TRUE, base::eAlarmClass::eWARNTYP);
		}
	}
	break;

	case eSubSteps::eAlarmLevel:
	{
		if (!m_aAlarm.IsAlarm(base::eAlarmError::ERROR_DOSE_LC_MAXWEIGHT))
		{
			m_aAlarm.SetAlarm(m_st, base::eAlarmError::ERROR_DOSE_LC_MAXWEIGHT, TRUE, base::eAlarmClass::eWARNTYP);
		}
	}
	break;

	case eSubSteps::eInit: 
	case eSubSteps::eMinMaxLevel:
	case eSubSteps::eMaxLevel:
	{
		m_aAlarm.ClearAlarm(base::eAlarmError::ERROR_DOSE_LC_MINWEIGHT);
		m_aAlarm.ClearAlarm(base::eAlarmError::ERROR_DOSE_LC_MAXWEIGHT);
	}
	break;
	}
	m_aAlarm.CheckWarningLevel(base::eAlarmError::ERROR_DOSE_LC_MAXWEIGHT);
	m_aAlarm.CheckWarningLevel(base::eAlarmError::ERROR_DOSE_LC_MINWEIGHT);
}
//*************************************************************************************
//*************************************************************************************
float32_t CIflControl::CalSetpoint(const float32_t fX) const
{
	const float32_t epsilon = 1e-06f;

	const float32_t fMin = m_fActMinSetpoint;
	const float32_t fMax = m_fActMaxSetpoint;
	const float32_t xMin = m_fMinLevel;
	const float32_t xMax = m_fMaxLevel;

	assert(xMin < xMax);
	assert(fMin > fMax);

	if (_F32(fabs(xMin - xMax) < epsilon))
	{
		return m_fActSetpoint;
	}
	auto m = (fMin - fMax) / (xMin - xMax);
	auto b = fMin - m * xMin;
	auto f = m * fX + b;
	return f;
}
//*************************************************************************************
//*************************************************************************************
inline void CIflControl::CalcMinMaxSetpoint(const float32_t fX)
{
	float32_t fGain = 0.0F;

	Dose_DSVGetPidPropGainGross(m_sID, &fGain);
	fGain = RANGE(fGain, 0, 100.0F);
	fGain /= 100.0F;
	m_fActMinSetpoint = ((1 + fGain) * fX);
	m_fActMaxSetpoint = ((1 - fGain) * fX);
}
//*************************************************************************************
//*************************************************************************************
void CIflControl::EnterMinLevel()
{
	float32_t fSetpoint = 0.0F;

	m_fActSetpoint = m_fActMinSetpoint;
	SetLineSetpoint(m_fActSetpoint);

	m_tMinStart = m_st;
	m_tNext = m_tMinStart + m_SampleTime;
	m_eSubSteps = eSubSteps::eMinLevel;
}
//*************************************************************************************
//*************************************************************************************
void CIflControl::RunMinLevel()
{
	const float32_t c_Hysterese = 0.02F;

	if (m_bExternalSetpointChanged)
	{
		m_bExternalSetpointChanged = FALSE;
		Dose_DSVGetNominalSetpoint(m_sID, &m_fActSetpoint);
		CalcMinMaxSetpoint(m_fActSetpoint);
		SetLineSetpoint(m_fActSetpoint);
	}
	else
	{
		if (m_st >= m_tNext)
		{
			if (m_fWeight < m_fMinLevel)
			{
				if (m_st >= m_tMinStart + 30u)
				{
					m_tMinStart = m_st;
					CalcMinMaxSetpoint(m_fActSetpoint);
					m_fActSetpoint = m_fActMinSetpoint;
					SetLineSetpoint(m_fActSetpoint);
				}
			}
			else if (m_fWeight >= m_fMaxLevel)
			{
				EnterMaxLevel();
			}
			else
			{
				// Mindestens 100g
				auto fDelta = __max(0.1F, m_fMinLevel * c_Hysterese);
				auto fHysterese = m_fMinLevel + fDelta;
				if (m_fWeight >= fHysterese)
				{
					// m_fMinLevel <= X <= m_fMaxLevel
					EnterMinMaxLevel();
				}
			}
			m_tNext = m_st + m_SampleTime;
		}
	}
}
//*************************************************************************************
//*************************************************************************************
void CIflControl::EnterMinMaxLevel()
{
	m_tNext = m_st + m_SampleTime;
	m_eSubSteps = eSubSteps::eMinMaxLevel;
}
//*************************************************************************************
//*************************************************************************************
void CIflControl::RunMinMaxLevel()
{
	if (m_bExternalSetpointChanged)
	{
		m_bExternalSetpointChanged = FALSE;
		Dose_DSVGetNominalSetpoint(m_sID, &m_fActSetpoint);
		CalcMinMaxSetpoint(m_fActSetpoint);
		SetLineSetpoint(m_fActSetpoint);
	}
	else
	{
		if (m_st >= m_tNext)
		{
			if (m_fWeight < m_fMinLevel)
			{
				EnterMinLevel();
			}
			else if (m_fWeight >= m_fAlarmLimit)
			{
				EnterAlarmLevel();
			}
			else if (m_fWeight >= m_fMaxLevel)
			{
				EnterMaxLevel();
			}
			else if (m_st >= m_tNext)
			{
				m_fActSetpoint = CalSetpoint(m_fWeight);
				SetLineSetpoint(m_fActSetpoint);
			}
			m_tNext = m_st + m_SampleTime;
		}
	}
}
//*************************************************************************************
//*************************************************************************************
void CIflControl::EnterMaxLevel()
{
	m_fActSetpoint = m_fSetpointMax;
	SetLineSetpoint(m_fActSetpoint);
	m_tMaxStart = m_st;
	m_tNext		= m_st + m_SampleTime;
	m_eSubSteps = eSubSteps::eMaxLevel;
}
//*************************************************************************************
//*************************************************************************************
void CIflControl::RunMaxLevel()
{
	if (m_st >= m_tNext)
	{
		if (m_fWeight < m_fMinLevel)
		{
			// Zeit stoppen
			m_fActSetpoint = (3600.0F / (m_st - m_tMaxStart) * (m_fMaxLevel - m_fMinLevel));
			m_fActSetpoint -= m_fSetpointMax;
			float32_t fMaxLeistung = 0.0f;
			Dose_EXGetMaxLeistung(&fMaxLeistung);
			m_fActSetpoint = RANGE(0, m_fActSetpoint, fMaxLeistung);
			CalcMinMaxSetpoint(m_fActSetpoint);
			EnterMinLevel();
		} 
		else if ((m_fSetpointMax > 0.0F) && (m_fWeight >= m_fAlarmLimit))
		{
			EnterAlarmLevel();
		}
		m_tNext = m_st + m_SampleTime;
	}
}
//*************************************************************************************
//*************************************************************************************
void CIflControl::EnterAlarmLevel()
{
	m_fActSetpoint = 0;
	SetLineSetpoint(m_fActSetpoint);
	m_tNext = m_st + m_SampleTime;
	m_eSubSteps = eSubSteps::eAlarmLevel;
}
//*************************************************************************************
//*************************************************************************************
void CIflControl::RunAlarmLevel()
{
	const float c_Hysterese = 0.02F; // 2 % vom Alarmlimit

	if (m_st >= m_tNext)
	{
		auto fDelta = __max(0.1F, m_fAlarmLimit * c_Hysterese);
		auto fHysterese = m_fAlarmLimit  - fDelta;
		if (m_fWeight < fHysterese)
		{
			EnterMaxLevel();
		}
		m_tNext = m_st + m_SampleTime;
	}
}
//*************************************************************************************
//*************************************************************************************
void CIflControl::EnterInitLevel()
{
	Dose_DSVGetNominalSetpoint(m_sID, &m_fActSetpoint);
	CalcMinMaxSetpoint(m_fActSetpoint);
	m_eSubSteps = eSubSteps::eInit;
}
//*************************************************************************************
//*************************************************************************************
void CIflControl::RunInitLevel()
{
	if (m_fWeight < m_fMinLevel)
	{
		EnterMinLevel();
	}
	else if (m_fWeight >= m_fAlarmLimit)
	{
		EnterAlarmLevel();
	}
	else if (m_fWeight >= m_fMaxLevel)
	{
		EnterMaxLevel();
	}
	else
	{
		EnterMinMaxLevel();
	}
}
//*************************************************************************************
//*************************************************************************************
BOOL CIflControl :: Control ()
{
	switch (m_eSubSteps)
	{
		default:
		case eSubSteps::eInit:
		{
			RunInitLevel();
		}
		break;

		case eSubSteps::eMinLevel:
		{
			RunMinLevel();
		}
		break;

		case eSubSteps::eMinMaxLevel:
		{
			RunMinMaxLevel();
		}
		break;

		case eSubSteps::eMaxLevel:
		{
			RunMaxLevel();
		}
		break;

		case eSubSteps::eAlarmLevel:
		{
			RunAlarmLevel();
		}
		break;
	}
	Dose_DSVSetLclWeightMaxLevelActive(m_sID, BOOL(m_eSubSteps == eSubSteps::eMaxLevel));
	Dose_DSVSetLclWeightMinLevelActive(m_sID, BOOL(m_eSubSteps == eSubSteps::eMinLevel));
	CheckAlarm();
	return TRUE;
}
//*************************************************************************************
//*************************************************************************************
BOOL CIflControl::Execute()
{
	assert(IsInit());

	// aktuelle Zeit holen
	auto result = CBaseControl::Execute();
	if (result)
	{
		GetSampleTime();

		// Gewicht holen
		GetWeight();

		(void)UpdateWeight();

		BOOL bChanged = GetLineSetpoint();
		if (bChanged)
		{
			m_bExternalSetpointChanged = TRUE;
		}
		// Freigabe
		BOOL bRelease = FALSE;
		Dose_DSVGetRelease(m_sID, &bRelease);

		//--------------------------------------
		switch (GetOperatingMode())
		{
			case eOperatingMode::RUNNING:
			{
				if ( ! bRelease)
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

			case eOperatingMode::TARING:
			{
				BOOL bTarierung = GetTaring();
				if (bTarierung)
				{
					m_TareTask.Update(m_st);
				}
				else
				{
					m_TareTask.Stop();
					m_TareTask.ExitExecute();
					SetOperatingMode(eOperatingMode::IDLE);
				}
			}
			break;

			case eOperatingMode::IDLE:
				default:
				{
					//--------------------------------------
					// Tarierung testen
					BOOL bTarierung = GetTaring();
					if (bTarierung)
					{
						m_TareTask.InitExecute();
						m_TareTask.Start(m_st);
						SetOperatingMode(eOperatingMode::TARING);
					}
					else
					{
						//--------------------------------------
						// sonst Freigabe erfolgt ?
						if (!bRelease)
						{
							Stop();
						}
						else
						{
							Start(m_st);
						}
					}
				}
				break;
		}
		SetAlarmOutput();
	}
	return result;
}




