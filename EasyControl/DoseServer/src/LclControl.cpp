//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Implementation of module LclControl
///
/// @file   LclControl.cpp
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
#include "LclControl.h"
#include "DoseDataLib/include/DoseData.h"
#include "AlarmManager.h"
#include "BASE/Base.def"

using namespace base;

#define DREHGEBERINTERVALL     5


//*********************************************************************************************
//*********************************************************************************************
CLclControl::CLclControl(int32_t s, loadcell::ILCModuleInterface& rlc) : CBaseControl{ s }
	, m_TareTask{ s }
	, m_WeightCtrl(s)
	, m_bMinReached{ FALSE }
    , m_bMaxReached{ FALSE }
	, m_bMinReachedLast{ FALSE }
	, m_bMaxReachedLast{ FALSE }

{
	m_TareTask.registerAlarmManager(this->m_aAlarm);
	m_TareTask.registerWeightCtrl(m_WeightCtrl);
	m_WeightCtrl.registerAlarmManager(this->m_aAlarm);
	m_WeightCtrl.registerLoadcell(rlc);
}
//*********************************************************************************************
//*********************************************************************************************
BOOL CLclControl :: InitExecute ( void)
{
	auto result = CBaseControl :: InitExecute ();
	if (result)
	{
		m_bMinReached		= FALSE;
		m_bMaxReached		= FALSE;
		m_bMinReachedLast	= FALSE;
		m_bMaxReachedLast	= FALSE;

		m_WeightCtrl.InitExecute();
		Dose_DSVSetLclWeightMaxLevelActive(m_sID, FALSE);
		Dose_DSVSetLclWeightMinLevelActive(m_sID, FALSE);
		m_WeightCtrl.Start(m_st);
		m_aAlarm.Start(m_st);
	}
	return result;
}
//*********************************************************************************************
//*********************************************************************************************
BOOL CLclControl :: ExitExecute ( void)
{
	BOOL result = CBaseControl::IsInit();
	if (result)
	{
		// Stop();
		m_aAlarm.Stop();
		m_WeightCtrl.Stop();
		m_WeightCtrl.ExitExecute();
		result = CBaseControl::ExitExecute();;
	}
	return result;
}
//*********************************************************************************************
//*********************************************************************************************
void CLclControl::CheckAlarm()
{
	if (m_bMinReached)
	{
		if ( ! m_bMinReachedLast)
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
//*********************************************************************************************
//*********************************************************************************************
BOOL CLclControl::Control(void)
{
	auto result = m_WeightCtrl.Update(m_st);
	if (result)
	{
		float32_t fWeight = 0.0F;
		Dose_DSVGetLCActualWeight(m_sID, &fWeight);
		if (m_bMinReached || m_bMaxReached)
		{
			const float32_t fHysteresis = 0.1F;
			if (m_bMinReached)
			{
				float32_t fMinLevel = 0.0F;
				Dose_DSVGetLclWeightMinLevel(m_sID, &fMinLevel);
				if (fWeight > fMinLevel + fHysteresis)
				{
					m_bMinReached = FALSE;
				}
				m_bMaxReached = FALSE;
			}
			else
			{
				float32_t fMaxLevel = 0.0F;
				Dose_DSVGetLclWeightMaxLevel(m_sID, &fMaxLevel);
				if (fWeight < fMaxLevel - fHysteresis)
				{
					m_bMaxReached = FALSE;
				}
				m_bMinReached = FALSE;
			}
		}
		else
		{
			float32_t fMinLevel = 0.0F;
			Dose_DSVGetLclWeightMinLevel(m_sID, &fMinLevel);
			if (fWeight < fMinLevel)
			{
				m_bMinReached = TRUE;
			}
			else
			{
				float32_t fMaxLevel = 0.0F;
				Dose_DSVGetLclWeightMaxLevel(m_sID, &fMaxLevel);
				if (fWeight > fMaxLevel)
				{
					m_bMaxReached = TRUE;
				}
			}
		}
		Dose_DSVSetLclWeightMaxLevelActive(m_sID, m_bMinReached);
		Dose_DSVSetLclWeightMinLevelActive(m_sID, m_bMaxReached);
	}
	CheckAlarm();
	return result;
}
//*********************************************************************************************
//*********************************************************************************************
BOOL CLclControl :: Execute ( void)
{
	auto result = CBaseControl::Execute();
	if (result && (m_st > 0))
	{
		//--------------------------------------
		switch (GetOperatingMode())
		{
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
					Control();
				}
			}
			break;
		}
	}
	return result;
}


