//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Implementation of module RefillControl
///
/// @file   RefillControl.cpp
///
///
/// @coypright Ing.büro Hafer
///            Branderweg 8A
///            D-91058 Erlangen
///
/// @author    Detlef Hafer
///
//------------------------------------------------------------------------------------
#include "DoseDataLib/include/DoseData.h"
#include "RefillControl.h"
#include "AlarmManager.h"
#include "Logger/include/Logger.h"


using namespace base;


//**************************************************************************************************
//**************************************************************************************************
CRefillControl::CRefillControl(int32_t id) : CBaseClass(id)
, m_pAlarm{ nullptr }
, m_RefillSignalCtrl{ id }
, m_AdsClient{ id }
, m_t0(0)
{
}
//**************************************************************************************************
//**************************************************************************************************
void CRefillControl::registerAlarmManager(CAlarmManager& rAlarm)
{
	m_pAlarm = &rAlarm;
}
//*****************************************************************************************************
//*****************************************************************************************************
int32_t CRefillControl::SetRefillRequest(const BOOL value)
{
	if (value)
	{
		if ( ! m_RefillSignalCtrl.IsStarted())
		{
			m_RefillSignalCtrl.Start(m_st);
		}
	}
	else
	{
		if (m_RefillSignalCtrl.IsStarted())
		{
			m_RefillSignalCtrl.Stop();
		}
	}
	m_RefillBits.flags.RefillState = value;
	return 0;
}
//*****************************************************************************************************
//*****************************************************************************************************
void CRefillControl::ClearAlarm(const base::eAlarmError lError) 
{
	assert(m_pAlarm);
	if ((lError == eAlarmError::ERROR_DOSE_REFILL_TIMEOUT) || (lError == eAlarmError::ERROR_MAX))
	{
		m_pAlarm->ClearAlarm(eAlarmError::ERROR_DOSE_REFILL_TIMEOUT);
	}
}
//*****************************************************************************************************
//*****************************************************************************************************
void CRefillControl::SetRefillControlEnable(const BOOL bEnable)
{
	m_RefillBits.flags.ControlEnable = bEnable;
	Dose_DSVSetRefillControlEnable(m_sID, bEnable);
}
//*****************************************************************************************************
//*****************************************************************************************************
BOOL CRefillControl::GetRefillControlEnable() const
{
	BOOL bEnable =
		m_RefillBits.flags.PBRefillCfg ||
		m_RefillBits.flags.SWRefillCfg ||
		m_AdsClient.Enable();

	return bEnable;
}
//*****************************************************************************************************
//*****************************************************************************************************
void CRefillControl::UpdateRefillControlEnable()
{
	SetRefillControlEnable(GetRefillControlEnable());
}
//*****************************************************************************************************
//*****************************************************************************************************
void CRefillControl::UpdateRefillBits()
{
	{
		BOOL bRelease = FALSE;
		Dose_DSVGetIORefillReleaseInput(m_sID, &bRelease);
		//m_AdsClient.AdsGetRefillRelease(bRelease);
		if (bRelease || (bRelease != BOOL(m_RefillBits.flags.IORefillSWRelease)))
		{
			m_RefillBits.flags.IORefillReleaseStart = bRelease;
			m_RefillBits.flags.IORefillReleaseStop  = !bRelease;
			m_RefillBits.flags.IORefillSWRelease = bRelease;
		}
	}
	{
		BOOL bStart = FALSE;
		Dose_DSVGetIORefillRequestInput(m_sID, &bStart);
		//m_AdsClient.AdsGetRefillRequest(bStart);
		if (bStart || ( BOOL(m_RefillBits.flags.IORefillSWStart) != bStart))
		{
			m_RefillBits.flags.IORefillStart	= bStart;
			m_RefillBits.flags.IORefillSWStart	= bStart;
		}
	}
	{
		BOOL bStop = FALSE;
		Dose_DSVGetIORefillStopInput(m_sID, &bStop);
		//m_AdsClient.AdsGetRefillStop(bStop);
		if (bStop || (BOOL(m_RefillBits.flags.IORefillSWStop) != bStop))
		{
			m_RefillBits.flags.IORefillStop = bStop;
			m_RefillBits.flags.IORefillSWStop = bStop;
		}
	}
	{
		BOOL bStart = FALSE;
		Dose_DSVGetIORefillInput(m_sID, &bStart);
		//m_AdsClient.AdsGetRefill(bStart);
		if (bStart || (BOOL(m_RefillBits.flags.IORefillSWStartStop) != bStart))
		{
			m_RefillBits.flags.IORefillStart = bStart;
			m_RefillBits.flags.IORefillStop = !bStart;
			m_RefillBits.flags.IORefillSWStartStop = true;
		}
	}
	{
		BOOL bChange = FALSE;
		Dose_DSVPBGetPBRefillStart(m_sID, &bChange);
		if (bChange)
		{
			m_RefillBits.flags.PBRefillStart = true;
			m_RefillBits.flags.PBRefillCfg = true;
		}
	}
	{
		BOOL bChange = FALSE;
		Dose_DSVPBGetPBRefillStop(m_sID, &bChange);
		if (bChange)
		{
			m_RefillBits.flags.PBRefillStop = true;
			m_RefillBits.flags.PBRefillCfg = true;
		}
	}
	{
		BOOL bChange = FALSE;
		Dose_DSVGetChangedSWRefillRelease(m_sID, &bChange);
		if (bChange)
		{
			BOOL bRelease = FALSE;
			Dose_DSVPopSWRefillRelease(m_sID, &bRelease);
			m_RefillBits.flags.SWRefillReleaseStart = bRelease;
			m_RefillBits.flags.SWRefillReleaseStop = !bRelease;
			m_RefillBits.flags.SWRefillCfg = true;
		}
	}
	{
		BOOL bChange = FALSE;
		Dose_DSVGetChangedRefillReleaseTrigger(m_sID, &bChange);
		if (bChange)
		{
			BOOL bReleaseTrigger = FALSE;
			Dose_DSVPopRefillReleaseTrigger(m_sID, &bReleaseTrigger);
			if (bReleaseTrigger)
			{
				BOOL bReleaseState = FALSE;
				Dose_DSVGetRefillReleaseState(m_sID, &bReleaseState);
				m_RefillBits.flags.SWRefillReleaseStop = bReleaseState;
				m_RefillBits.flags.SWRefillReleaseStart = !bReleaseState;
				m_RefillBits.flags.SWRefillCfg = true;
			}
		}
	}
	{
		auto result = IsWeightAboveMax();
		Dose_DSVSetRefillWeightAboveMax(m_sID, result);
	}
	{
		auto result = IsWeightBelowMin();
		Dose_DSVSetRefillWeightBelowMin(m_sID, result);
	}
	{
		auto result = IsFeederEmpty();
		Dose_DSVSetRefillFeederEmptyStarted(m_sID, result);
	}
	{
		auto result = IsFeederEmptyExit();
		Dose_DSVSetRefillFeederEmptyEnded(m_sID, result);
	}
	Dose_DSVPBResetPBRefillStop(m_sID);
	Dose_DSVPBResetPBRefillStart(m_sID);
	UpdateRefillControlEnable();
}
//*****************************************************************************************************
//*****************************************************************************************************
void CRefillControl :: CheckRefillRelease (void)
{
	BOOL bRefillReleaseState = FALSE;

	Dose_DSVGetRefillReleaseState(m_sID, &bRefillReleaseState);
	if (bRefillReleaseState)
	{
		BOOL bHardStop = GetHardStop();
		if (bHardStop)
		{
			LOGDEBUG(__FUNCTION__ << "Stopped refill reliease due to hard stop, id = " << m_sID << "\n");
			bRefillReleaseState = FALSE;
		}
		else
		{
			BOOL bEmptyfeederState = FALSE;
			BOOL bCalibrationState = FALSE;
			BOOL bTareState = FALSE;
			Dose_DSVGetEmptyFeederState(m_sID, &bEmptyfeederState);
			Dose_DSVGetCalibrationState(m_sID, &bCalibrationState);
			Dose_DSVGetTaraState(m_sID, &bTareState);
			if (bEmptyfeederState || bTareState || bCalibrationState || ((m_RefillBits.flags.SWRefillReleaseStop) || (m_RefillBits.flags.IORefillReleaseStop)))
			{
				bRefillReleaseState = FALSE;

				LOGDEBUG(__FUNCTION__ << "Stopped refill reliease, id = " << m_sID << 
					" bEmptyfeederState = " << bEmptyfeederState <<
					" bTareState = " << bTareState <<
					" bCalibrationState = " << bCalibrationState <<
					" m_RefillBits.flags.SWRefillReleaseStop = " << BOOL(m_RefillBits.flags.SWRefillReleaseStop) <<
					" m_RefillBits.flags.SWRefillReleaseStop = " << BOOL(m_RefillBits.flags.SWRefillReleaseStop) << "\n");
			}
		}
	}
	else 
	{
		if ((m_RefillBits.flags.IORefillReleaseStart) || (m_RefillBits.flags.SWRefillReleaseStart))
		{
			bRefillReleaseState = TRUE;
		}
	}
	Dose_DSVSetRefillReleaseState(m_sID, bRefillReleaseState);

	if ( ! bRefillReleaseState)
	{
		SetRefillAlarm(FALSE);
	}


	m_RefillBits.flags.IORefillReleaseStart = false;
	m_RefillBits.flags.IORefillReleaseStop = false;
	m_RefillBits.flags.SWRefillReleaseStart = false;
	m_RefillBits.flags.SWRefillReleaseStop = false;
}
//*****************************************************************************************************
//*****************************************************************************************************
BOOL CRefillControl :: CheckRefillStart (void)
{
	BOOL bRefillStart = FALSE;

	CheckRefillRelease();

	BOOL bRefillReleaseState = FALSE;
	Dose_DSVGetRefillReleaseState(m_sID, &bRefillReleaseState);
	if ( ( ! bRefillReleaseState) || ((m_RefillBits.flags.IORefillStop) || (m_RefillBits.flags.PBRefillStop)))
	{
		bRefillStart = FALSE;
	}
	else if (m_RefillBits.flags.IORefillStart || m_RefillBits.flags.PBRefillStart)
	{
		bRefillStart = TRUE;
	}
	else
	{}

	Dose_DSVSetRefillStartState(m_sID, bRefillStart);

	m_RefillBits.flags.IORefillStop  = false;
	m_RefillBits.flags.IORefillStart = false;
	m_RefillBits.flags.PBRefillStart = false;
	m_RefillBits.flags.PBRefillStop  = false;
	return bRefillStart;
}
//*****************************************************************************************************
//*****************************************************************************************************
BOOL CRefillControl::CheckRefillStop(void)
{
	BOOL bRefillStop = FALSE;

	CheckRefillRelease();

	BOOL bRefillReleaseState = FALSE;
	Dose_DSVGetRefillReleaseState(m_sID, &bRefillReleaseState);
	if ( (! bRefillReleaseState) || (m_RefillBits.flags.IORefillStop) || (m_RefillBits.flags.PBRefillStop))
	{
		bRefillStop = TRUE;
	}
	Dose_DSVSetRefillStopState(m_sID, bRefillStop);

	m_RefillBits.flags.IORefillStop = false;
	m_RefillBits.flags.IORefillStart = false;
	m_RefillBits.flags.PBRefillStart = false;
	m_RefillBits.flags.PBRefillStop = false;
	return bRefillStop;
}
//*****************************************************************************************************
//*****************************************************************************************************
BOOL CRefillControl::CheckRefillDone() 
{
	auto bRefillDone = CheckRefillStop();
	if ( ! bRefillDone)
	{
		bRefillDone = IsWeightAboveMax();
	}
	return bRefillDone;
}
//*****************************************************************************************************
//*****************************************************************************************************
void CRefillControl :: SetRefillAlarm (BOOL bAlarm)
{
	assert(m_pAlarm);
	if ( ! bAlarm)
	{
		if (m_pAlarm->IsAlarm(eAlarmError::ERROR_DOSE_REFILL_TIMEOUT))
		{
			m_pAlarm->SetAlarm(m_st, eAlarmError::ERROR_DOSE_REFILL_TIMEOUT, FALSE);
		}
	}
	else
	{
		if (!m_pAlarm->IsAlarm(eAlarmError::ERROR_DOSE_REFILL_TIMEOUT))
		{
			m_pAlarm->SetAlarm(m_st, eAlarmError::ERROR_DOSE_REFILL_TIMEOUT, TRUE, eAlarmClass::eALARMTYP);
		}
	}
}
//*****************************************************************************************************
//*****************************************************************************************************
void CRefillControl::CheckAlarmLimit()
{
	assert(m_pAlarm);

	BOOL bOperate = FALSE;
	Dose_DSVGetIOOperateOutput(m_sID, &bOperate);
#if 0
	BOOL bStarted = FALSE;
	Dose_DSVGetStart(m_sID, &bStarted);
	bOperate |= bStarted;
#endif
	if (bOperate)
	{
		if ( ! m_pAlarm->IsAlarm(eAlarmError::ERROR_LWF_REFILLALARMLIMIT))
		{
			auto bFeederEmpty = IsFeederEmpty();
			if (bFeederEmpty)
			{
				m_pAlarm->SetAlarm(m_st, eAlarmError::ERROR_LWF_REFILLALARMLIMIT, TRUE, eAlarmClass::eALARMTYP);
			}
		}
		else
		{
			auto bEmptyFeederExpired = IsFeederEmptyExit();
			if ( bEmptyFeederExpired )
			{
				m_pAlarm->SetAlarm(m_st, eAlarmError::ERROR_LWF_REFILLALARMLIMIT, FALSE);
			}
		}
		m_pAlarm->CheckWarningLevel(m_st, eAlarmError::ERROR_LWF_REFILLALARMLIMIT);
	}
}
//*****************************************************************************************************
//*****************************************************************************************************
void CRefillControl::CheckRefillTimeout()
{
	m_pAlarm->CheckWarningLevel(eAlarmError::ERROR_DOSE_REFILL_TIMEOUT);
}
//*****************************************************************************************************
//*****************************************************************************************************
void CRefillControl::CheckRefillDriveFeeder()
{
	BOOL  bRefillState = FALSE;

	Dose_DSVGetRefillState(m_sID, &bRefillState);
	if (bRefillState)
	{
		// Mindestens eine Refillanforderung muss sein
		BOOL bDriveErrorRefillFeeder = FALSE;
		Dose_DSVGetIODriveErrorRefillFeederInput(m_sID, &bDriveErrorRefillFeeder);
		if (bDriveErrorRefillFeeder)
		{
			if ( ! m_pAlarm->IsAlarm(eAlarmError::ERROR_DOSE_DRIVEERRORREFILLFEEDER))
			{
				m_pAlarm->SetAlarm(m_st, eAlarmError::ERROR_DOSE_DRIVEERRORREFILLFEEDER, TRUE, eAlarmClass::eALARMTYP);
			}
		}
	}
	m_pAlarm->CheckWarningLevel(m_st, eAlarmError::ERROR_DOSE_DRIVEERRORREFILLFEEDER);
}
//*****************************************************************************************************
//*****************************************************************************************************
void CRefillControl::CheckExtRefillAlarm()
{
	assert(m_pAlarm);
	BOOL bAlarm = FALSE;

	Dose_DSVGetIORefillAlarmInput(m_sID, &bAlarm);
	if (bAlarm)
	{
		if ( ! m_pAlarm->IsAlarm(eAlarmError::ERROR_DOSE_EXTREFILLALARM))
		{
			m_pAlarm->SetAlarm(m_st, eAlarmError::ERROR_DOSE_EXTREFILLALARM, TRUE, eAlarmClass::eALARMTYP);
		}
	}
	m_pAlarm->CheckWarningLevel(eAlarmError::ERROR_DOSE_EXTREFILLALARM);
}
//*****************************************************************************************************
//*****************************************************************************************************
void CRefillControl::CheckRefillAlarm(void)
{
	CheckAlarmLimit();
	CheckRefillTimeout();
}
//*****************************************************************************************************
//*****************************************************************************************************
void CRefillControl::AlertControl(void)
{
	CheckRefillDriveFeeder();
	CheckExtRefillAlarm();
	CheckRefillAlarm();
}
//*****************************************************************************************************
//*****************************************************************************************************
BOOL CRefillControl :: RefillExecute ( void)
{
	assert(m_pAlarm);

	switch (m_sStep)
	{
		default:
		case eRefillSteps::eRefillInit:
			{
				auto bRefill = CheckRefillStart();
				if ( bRefill)
				{
					auto bMaxReached = IsWeightAboveMax();
					if  (bMaxReached)
					{
						// Refill
						bRefill = FALSE;
						SetRefillAlarm(FALSE);
					}
				}
				Dose_DSVSetRefillState (m_sID, bRefill);
				SetRefillRequest(bRefill);
				if (  bRefill )
                {
					OnRefillStart();
					m_t0 = m_st;
					m_sStep  = eRefillSteps::eRefillStart;
				}
			}
			break;

		case eRefillSteps::eRefillStart:
			{
				auto bRefill = ! CheckRefillDone();
				if ( bRefill )
                {
					uint32_t lDelay = 0L;
					Dose_DSVGetRefillTime(m_sID, &lDelay) ;
					if (m_st > m_t0 + lDelay )
					{
						// Refill
						SetRefillAlarm(TRUE);
						m_sStep  = eRefillSteps::eRefillRunning;
					}
                }
				else
                {
					Dose_DSVSetRefillState(m_sID, FALSE) ;
					SetRefillRequest (FALSE);
					OnRefillStop();
					m_sStep = eRefillSteps::eRefillInit;
				}
			}
			break;

		case eRefillSteps::eRefillRunning:
			{
				auto bRefill = ! CheckRefillDone();
				if ( ! bRefill)
				{
					SetRefillAlarm(FALSE);
					Dose_DSVSetRefillState(m_sID, FALSE);
					SetRefillRequest(FALSE);
					OnRefillStop();
					m_sStep = eRefillSteps::eRefillInit;
				}
			}
			break;
	}
	return TRUE;
}
//*****************************************************************************************************
//*****************************************************************************************************
void CRefillControl::RefillInit(void)
{
	m_sStep = eRefillSteps::eRefillInit;
	Dose_DSVInitRefillStopState(m_sID, FALSE);
	Dose_DSVInitRefillStartState(m_sID, FALSE);
	Dose_DSVInitRefillState(m_sID, FALSE); 
	BOOL bTemp = FALSE;
	Dose_DSVGetModuleRefillEnable(m_sID, &bTemp);
	if (!bTemp)
	{
		Dose_DSVInitRefillReleaseState(m_sID, FALSE); // keep last state
	}
	Dose_DSVInitSWRefillRelease(m_sID, FALSE);
	SetRefillRequest(FALSE);
	Dose_DSVPBResetPBRefillStop(m_sID);
	Dose_DSVPBResetPBRefillStart(m_sID);
	Dose_DSVInitRefillReleaseTrigger(m_sID, 0);

	Dose_DSVInitRefillWeightAboveMax(m_sID, FALSE);
	Dose_DSVInitRefillWeightBelowMin(m_sID, FALSE);
	Dose_DSVInitRefillFeederEmptyStarted(m_sID, FALSE);
	Dose_DSVInitRefillFeederEmptyEnded(m_sID, FALSE);

	m_pAlarm->SetStart(eAlarmError::ERROR_DOSE_DRIVEERRORREFILLFEEDER, m_st);
	m_pAlarm->SetStart(eAlarmError::ERROR_DOSE_REFILL_TIMEOUT, m_st);
	m_pAlarm->SetStart(eAlarmError::ERROR_LWF_REFILLALARMLIMIT, m_st);
	m_pAlarm->SetStart(eAlarmError::ERROR_DOSE_EXTREFILLALARM, m_st);
}
//*****************************************************************************************************
//*****************************************************************************************************
BOOL CRefillControl :: Update (const uint32_t t)
{
	assert(IsInit());
	auto result = IsModuleEnabled();
	if (result)
	{
		result = CBaseClass::Update(t);
		if ( result)
		{
			m_AdsClient.UpdateState();
			UpdateRefillBits();
			AlertControl();
			RefillExecute();
			if (m_RefillSignalCtrl.IsStarted())
			{
				m_RefillSignalCtrl.Update(m_st);
			}
		}
	}
	return result;
}
//*****************************************************************************************************
//*****************************************************************************************************
BOOL CRefillControl :: InitExecute ( void )
{   
	auto result = CBaseClass::InitExecute();
	if ( result )
	{
		m_AdsClient.Init();

		BOOL bTemp = FALSE;
		Dose_DSVGetModuleRefillEnable(m_sID, &bTemp);
		m_RefillBits.allflags = 0;
		m_RefillBits.flags.ModuleEnable = bTemp;
		ClearAlarm(eAlarmError::ERROR_MAX);

		RefillInit();

		m_RefillSignalCtrl.InitExecute();
	}
    return result;    
}
//*****************************************************************************************************
//*****************************************************************************************************
BOOL CRefillControl :: ExitExecute ( void )
{   
	auto result = CBaseClass::IsInit();
	if (result)
	{
		ClearAlarm(eAlarmError::ERROR_MAX);
		m_RefillSignalCtrl.Stop();
		m_RefillSignalCtrl.ExitExecute();
		m_AdsClient.Exit();
		result = CBaseClass::ExitExecute();
	}
    return result;    
}
//*****************************************************************************************************
//*****************************************************************************************************
void CRefillControl::CheckRefill()
{
	auto result = IsModuleEnabled();
	if (result)
	{
		BOOL bRun = FALSE;
		Dose_DSVGetIOOperateOutput(m_sID, &bRun);
		if (bRun)
		{
			BOOL bA = FALSE;
			Dose_DSVGetRefillState(m_sID, &bA);
			if (!bA)
			{
				if (IsWeightBelowMin())
				{
					Dose_DSVPBSetPBRefillStart(m_sID);
				}
			}
		}
	}
}





