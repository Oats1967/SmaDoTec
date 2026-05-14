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
CIflControl::CIflControl(int32_t id, loadcell::ILCModuleInterface& rlc) : CBaseClass(id)
, m_WeightCtrl{ id }
, m_TareTask{ id }
, m_bPidParameterChanged{ FALSE }
, m_bExternalSetpointChanged{ FALSE }
, m_bMinReached{ FALSE }
, m_bMaxReached{ FALSE }
, m_bMinReachedLast{ FALSE }
, m_bMaxReachedLast{ FALSE }
, m_fWeight{ 0.0F }
, m_fMinLevel{ 0.0F }
, m_fMaxLevel{ 0.0F }
, m_fNormLevel{ 0.0F }
, m_fSetpoint{ 0.0F }
, m_fPidPropGainGross{ 0.0F }
, m_SampleTime{ 1U }
, m_tNext{ 0 }
, m_eSubSteps { eSubSteps ::eDefault}

{
	m_WeightCtrl.registerAlarmManager(this->m_aAlarm);
	m_WeightCtrl.registerLoadcell(rlc);
	m_TareTask.registerAlarmManager(this->m_aAlarm);
	m_TareTask.registerWeightCtrl(m_WeightCtrl);
}
//*************************************************************************************
//*************************************************************************************
void CIflControl::SetLineSetpoint(float32_t fSetpoint)
{
	const float32_t c_epsilon = 1e-04f;

	float32_t fMinSetpoint;
	float32_t fMaxSetpoint;
	Dose_DSVGetIflMinLineSetpoint(m_sID, &fMinSetpoint);
	Dose_DSVGetIflMaxLineSetpoint(m_sID, &fMaxSetpoint);
	if (fabs(fMaxSetpoint - 0.0F) < c_epsilon)
	{
		fMaxSetpoint = FLT_MAX;
	}

	fSetpoint = RANGE(fSetpoint, fMinSetpoint, fMaxSetpoint);
	Dose_EXSetMBLineSetpoint(fSetpoint);
	Dose_DSVSetActualSetpoint(m_sID, fSetpoint);
	m_fSetpoint = fSetpoint;
	m_bPidParameterChanged = FALSE;
	m_bExternalSetpointChanged = FALSE;
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
BOOL CIflControl::GetPidParameter()
{
	BOOL bChanged = FALSE;
	Dose_DSVGetChangedPidPropGainGross(m_sID, &bChanged); // [0..1]
	if (bChanged)
	{
		Dose_DSVPopPidPropGainGross(m_sID, &m_fPidPropGainGross); // [0..1]
	}
	return bChanged;
}

//*************************************************************************************
//*************************************************************************************
BOOL CIflControl::GetWeight()
{
	auto result = m_WeightCtrl.Update(m_st);
	Dose_DSVGetLCActualWeight(m_sID, &m_fWeight);
	Dose_DSVGetLclWeightMinLevel(m_sID, &m_fMinLevel);
	Dose_DSVGetLclWeightMaxLevel(m_sID, &m_fMaxLevel);
	m_fNormLevel = (m_fMaxLevel + m_fMinLevel) / 2.0F;
	return result;
}
//*******************************************************************************************************
//*******************************************************************************************************
BOOL CIflControl::GetChangedSampleInterval(void)
{
	BOOL bChanged = FALSE;

	Dose_DSVGetChangedPidSampleInterval(m_sID, &bChanged); // [0..1]
	if (bChanged)
	{
		Dose_DSVPopPidSampleInterval(m_sID, &m_SampleTime); // [0..1]
		m_SampleTime = __max(m_SampleTime, 1U);
	}
	return bChanged;
}
//*************************************************************************************
//*************************************************************************************
BOOL CIflControl :: Start(const uint32_t t)
{
    auto result = CBaseClass::Start(t);
    if (result)
    {
		Dose_DSVPopPidSampleInterval(m_sID, &m_SampleTime); // [0..1]
		Dose_DSVPopPidPropGainGross(m_sID, &m_fPidPropGainGross); // [0..1]
		m_WeightCtrl.SetPriority(base::LC_PRIORITY::LC_PRIORITY_HIGH);
		EnterDefaultLevel();
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
		m_bMinReached = FALSE;
		m_bMaxReached = FALSE;
		m_bMinReachedLast = FALSE;
		m_bMaxReachedLast = FALSE;

		Dose_EXGetLineSetpoint(&m_fSetpoint);
		Dose_DSVSetActualSetpoint(m_sID, m_fSetpoint);
		Dose_DSVSetNominalSetpoint(m_sID, m_fSetpoint);
		Dose_DSVPopPidPropGainGross(m_sID, &m_fPidPropGainGross); // [0..1]
		Dose_DSVPopPidSampleInterval(m_sID, &m_SampleTime); // [0..1]
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
	if (m_bMinReached)
	{
		if (!m_bMinReachedLast)
		{
			if (!m_aAlarm.IsAlarm(base::eAlarmError::ERROR_DOSE_LC_MINWEIGHT))
			{
				m_aAlarm.SetAlarm(m_st, base::eAlarmError::ERROR_DOSE_LC_MINWEIGHT, TRUE, base::eAlarmClass::eWARNTYP);
			}
		}
	}
	else
	{
		m_aAlarm.ClearAlarm(base::eAlarmError::ERROR_DOSE_LC_MINWEIGHT);
	}
	m_bMinReachedLast = m_bMinReached;

	if (m_bMaxReached)
	{
		if (!m_bMaxReachedLast)
		{
			if (!m_aAlarm.IsAlarm(base::eAlarmError::ERROR_DOSE_LC_MAXWEIGHT))
			{
				m_aAlarm.SetAlarm(m_st, base::eAlarmError::ERROR_DOSE_LC_MAXWEIGHT, TRUE, base::eAlarmClass::eWARNTYP);
			}
		}
	}
	else
	{
		m_aAlarm.ClearAlarm(base::eAlarmError::ERROR_DOSE_LC_MAXWEIGHT);
	}

	m_bMaxReachedLast = m_bMaxReached;
	m_aAlarm.CheckWarningLevel(base::eAlarmError::ERROR_DOSE_LC_MAXWEIGHT);
	m_aAlarm.CheckWarningLevel(base::eAlarmError::ERROR_DOSE_LC_MINWEIGHT);
}
//*************************************************************************************
//*************************************************************************************
void CIflControl::EnterMinLevel()
{
	m_PidControl.Open(m_fPidPropGainGross * 2.0F, 0.0F, 0.01F);
	float32_t fError = (m_fNormLevel - m_fWeight);
	float32_t fD = m_PidControl.Start(m_st, fError);
	SetLineSetpoint(m_fSetpoint + fD);
	m_tNext = m_st + m_SampleTime;
	m_eSubSteps = eSubSteps::eMinLevel;
}
//*************************************************************************************
//*************************************************************************************
void CIflControl::RunMinLevel()
{
	float32_t fHysteresis = 0.0F;
	Dose_DSVGetLclWeightHysteresis(m_sID, &fHysteresis);

	if (m_fWeight > m_fMaxLevel)
	{
		EnterMaxLevel();
	}
	else if (m_fWeight > m_fMinLevel + fHysteresis)
	{
		EnterNormLevel();
	}
	else if (m_bExternalSetpointChanged)
	{
		m_bExternalSetpointChanged = FALSE;
		Dose_DSVGetNominalSetpoint(m_sID, &m_fSetpoint);
		EnterMinLevel();
	}
	else if (m_bPidParameterChanged)
	{
		EnterMinLevel();
	}
	else if (m_st >= m_tNext)
	{
		float32_t fError = (m_fNormLevel - m_fWeight);
		float32_t fD = m_PidControl.Update(m_st, fError);
		SetLineSetpoint(m_fSetpoint + fD);
		m_tNext = m_st + m_SampleTime;
	}
}

//*************************************************************************************
//*************************************************************************************
void CIflControl::EnterMaxLevel()
{
	SetLineSetpoint(0.0F);
	m_tNext = m_st + m_SampleTime;
	m_eSubSteps = eSubSteps::eMaxLevel;
}
//*************************************************************************************
//*************************************************************************************
void CIflControl::RunMaxLevel()
{
	float32_t fHysteresis = 0.0F;
	Dose_DSVGetLclWeightHysteresis(m_sID, &fHysteresis);
	if (m_fWeight < m_fMaxLevel - fHysteresis)
	{
		Dose_DSVGetNominalSetpoint(m_sID, &m_fSetpoint);
		EnterNormLevel();
	}
	else if (m_fWeight < m_fMinLevel)
	{
		EnterMinLevel();
	}
	else
	{
		// do nothing
	}
}
//*************************************************************************************
//*************************************************************************************
void CIflControl::EnterNormLevel()
{
	static const float32_t fPidIntegralGain = 0.00f;
	static const float32_t fDifferentialverstaerkung = 0.01f;

	float32_t fPidPropGainGross = 0.0F;
	Dose_DSVPopPidPropGainGross(m_sID, &fPidPropGainGross); // [0..1]
	fPidPropGainGross /= 100.0f;
	m_PidControl.Open(fPidPropGainGross, fPidIntegralGain, fDifferentialverstaerkung);
	float32_t fError = ( m_fNormLevel - m_fWeight );
	float32_t fD = m_PidControl.Start(m_st, fError);
	SetLineSetpoint(m_fSetpoint + fD);
	m_tNext = m_st + m_SampleTime;
	m_eSubSteps = eSubSteps::eNormLevel;
}
//*************************************************************************************
//*************************************************************************************
void CIflControl::RunNormLevel()
{
	if (m_fWeight < m_fMinLevel)
	{
		EnterMinLevel();
	}
	else if (m_fWeight > m_fMaxLevel)
	{
		EnterMaxLevel();
	}
	else if (m_bExternalSetpointChanged)
	{
		m_bExternalSetpointChanged = FALSE;
		Dose_DSVGetNominalSetpoint(m_sID, &m_fSetpoint);
		EnterNormLevel();
	}
	else if (m_bPidParameterChanged)
	{
		EnterNormLevel();
	}
	else if (m_st >= m_tNext)
	{
		float32_t fError = (m_fNormLevel - m_fWeight);
		float32_t fD = m_PidControl.Update(m_st, fError);
		SetLineSetpoint(m_fSetpoint + fD);
		m_tNext = m_st + m_SampleTime;
	}
}
//*************************************************************************************
//*************************************************************************************
void CIflControl::EnterDefaultLevel()
{
	m_bExternalSetpointChanged = FALSE;
	Dose_DSVGetNominalSetpoint(m_sID, &m_fSetpoint);
	m_eSubSteps = eSubSteps::eDefault;
}
//*************************************************************************************
//*************************************************************************************
void CIflControl::RunDefaultLevel()
{
	if (m_fWeight < m_fMinLevel)
	{
		EnterMinLevel();
	}
	else if (m_fWeight > m_fMaxLevel)
	{
		EnterMaxLevel();
	}
	else
	{
		EnterNormLevel();
	}
}
//*************************************************************************************
//*************************************************************************************
BOOL CIflControl :: Control ()
{
	switch (m_eSubSteps)
	{
		default:
		case eSubSteps::eDefault:
		{
			RunDefaultLevel();
		}
		break;

		case eSubSteps::eNormLevel:
		{
			RunNormLevel();
		}
		break;

		case eSubSteps::eMaxLevel:
		{
			RunMaxLevel();
		}
		break;

		case eSubSteps::eMinLevel:
		{
			RunMinLevel();
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
		// Gewicht holen
		GetWeight();

		GetChangedSampleInterval();

		BOOL bChanged = GetLineSetpoint();
		if (bChanged)
		{
			m_bExternalSetpointChanged = TRUE;
		}
		bChanged = GetPidParameter();
		if (bChanged)
		{
			m_bPidParameterChanged = TRUE;
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




