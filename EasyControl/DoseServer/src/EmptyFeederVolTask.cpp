//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Implementation of module EmptyFeederVolTask
///
/// @file   EmptyFeederVolTask.cpp
///
///
/// @coypright Ing.büro Hafer
///            Branderweg 8A
///            D-91058 Erlangen
///
/// @author    Detlef Hafer
///
//------------------------------------------------------------------------------------
#include "EmptyFeederVolTask.h"
#include "DriveControl.h"
#include "WeightControl.h"
#include "AlarmManager.h"
#include "DoseDataLib/include/DoseData.h"


using namespace base;

//**************************************************************************************************
//**************************************************************************************************
void CEmptyFeederVolTask::registerDriveCtrl(CDriveControl& encoderCtrl)
{
	m_pDriveCtrl = &encoderCtrl;
}
//**************************************************************************************************
//**************************************************************************************************
void CEmptyFeederVolTask::registerAlarmManager(CAlarmManager& rAlarm)
{
	m_pAlarm = &rAlarm;
}
//**************************************************************************************************
//**************************************************************************************************
BOOL CEmptyFeederVolTask::CheckErrorState(void)
{
	assert(m_pDriveCtrl);
	assert(m_pAlarm);

	eAlarmError errorid = eAlarmError::ERROR_MAX;
	{
		BOOL bIOReleaseInput = FALSE;
		Dose_DSVGetIOReleaseInput(m_sID, &bIOReleaseInput);
		BOOL bServiceMode = FALSE;
		Dose_EXGetServiceMode(&bServiceMode);
		if (( ! bIOReleaseInput) && (!bServiceMode))
		{
			errorid = eAlarmError::ERROR_EMPTYFEEDER_RELEASEFAILED;
		}
		else if (m_st > m_ElapsedTime)
		{
			errorid = eAlarmError::ERROR_EMPTYFEEDER_TIMEOUT;
		}
	}
	BOOL error = (errorid != eAlarmError::ERROR_MAX);
	if (error)
	{
		m_pAlarm->SetAlarm(m_st, errorid, TRUE, base::eAlarmClass::eALARMTYP);
	}
	return error;
}
//**************************************************************************************************
//**************************************************************************************************
BOOL CEmptyFeederVolTask::Start(const uint32_t t)
{
	auto result = CBaseClass::Start(t);
	if ( result )
	{
		assert(m_pDriveCtrl);

		SetState(TRUE);
		m_pDriveCtrl->Start(m_st);
		StoreAlarms();
		StoreRefillReleaseState();
		InitDrivecommand();
		uint32_t totalTime = 0;
		Dose_DSVGetEmptyFeederRuntime(m_sID, &totalTime); // Save AlarmDriveCommandHigh value
		m_ElapsedTime = m_st + totalTime;
		m_Counter = 0;
		m_LastTime = _U32(-1);
		m_sStep = eEmptyFeederVolSteps::eEmptyFeederRunning;
	}
	return result;
}
//**************************************************************************************************
//**************************************************************************************************
BOOL CEmptyFeederVolTask::Stop()
{ 
	assert(m_pDriveCtrl);
	assert(m_pAlarm);

	auto result = CBaseClass::IsStarted();
	if ( result)
	{
#if _DEBUG
		BOOL bState = FALSE;
		Dose_DSVGetEmptyFeederState(m_sID, &bState);
		assert(bState);
#endif
		m_pDriveCtrl->Stop();
		RestoreRefillReleaseState();
		RestoreAlarms();
		SetState(FALSE);

		m_pAlarm->CheckWarningLevel(eAlarmError::ERROR_EMPTYFEEDER_RELEASEFAILED);
		m_pAlarm->CheckWarningLevel(eAlarmError::ERROR_EMPTYFEEDER_TIMEOUT);
		m_pAlarm->CheckWarningLevel(eAlarmError::ERROR_EMPTYFEEDER_GENERAL);
		m_sStep = eEmptyFeederVolSteps::eEmptyFeederInit;
		result = CBaseClass::Stop();
	}
	return result;
}
//**************************************************************************************************
//**************************************************************************************************
void CEmptyFeederVolTask::SetState(const BOOL bState)
{
	m_AdsClient.SetEmptyFeeder(bState);
}
//**************************************************************************************************
//**************************************************************************************************
void CEmptyFeederVolTask::StoreAlarms()
{
	Dose_DSVGetAlarmDriveCommandHigh(m_sID, &m_AlarmDriveCommandHigh); // Save AlarmDriveCommandHigh value
	Dose_DSVSetAlarmDriveCommandHigh(m_sID, 1000U);					   // Don't trigger alarm
}
//**************************************************************************************************
//**************************************************************************************************
void CEmptyFeederVolTask::RestoreAlarms() const
{
	Dose_DSVSetAlarmDriveCommandHigh(m_sID, m_AlarmDriveCommandHigh); // restore old value
}
//**************************************************************************************************
//**************************************************************************************************
void CEmptyFeederVolTask::InitDrivecommand()
{
	float32_t fDriveCommand = 100.0F;
	Dose_DSVPopEmptyFeederSpeed(m_sID, &fDriveCommand);
	m_pDriveCtrl->UpdateDriveCommand(fDriveCommand / 100.0F);
}
//**************************************************************************************************
//**************************************************************************************************
void CEmptyFeederVolTask::CheckDrivecommand()
{
	BOOL bChanged = FALSE;
	Dose_DSVGetChangedEmptyFeederSpeed(m_sID, &bChanged);
	if (bChanged)
	{
		InitDrivecommand();
	}
}
//**************************************************************************************************
//**************************************************************************************************
void CEmptyFeederVolTask::StoreRefillReleaseState()
{
	Dose_DSVGetRefillReleaseState(m_sID, &m_RefillReleaseState);
	if (m_RefillReleaseState)
	{
		Dose_DSVSetSWRefillRelease(m_sID, FALSE);
	}
}
//**************************************************************************************************
//**************************************************************************************************
void CEmptyFeederVolTask::RestoreRefillReleaseState() const
{
	Dose_DSVSetSWRefillRelease(m_sID, m_RefillReleaseState);
}
//**************************************************************************************************
//**************************************************************************************************
void CEmptyFeederVolTask::CheckRefillReleaseState() const
{
	BOOL state = FALSE;
	Dose_DSVGetRefillReleaseState(m_sID, &state);
	if (state)
	{
		Dose_DSVSetSWRefillRelease(m_sID, FALSE);
	}
}
//**************************************************************************************************
//**************************************************************************************************
void CEmptyFeederVolTask::StepError()
{
	Stop();
}
//**************************************************************************************************
//**************************************************************************************************
void CEmptyFeederVolTask::StepRunning()
{
	assert(m_pDriveCtrl);
	if ( m_st != m_LastTime)
	{
		m_LastTime = m_st;
		auto error = CheckErrorState();
		if (!error)
		{
			CheckDrivecommand();
			CheckRefillReleaseState();
		}
	}
}
//*********************************************************************************************
//*********************************************************************************************
BOOL CEmptyFeederVolTask:: Update(const uint32_t t)
{
	auto result = CBaseClass::Update(t);
	if (result)
	{
		assert(m_pDriveCtrl);

		switch (m_sStep)
		{
		default:
			break;

		case eEmptyFeederVolSteps::eEmptyFeederRunning:
			StepRunning();
			break;

		case eEmptyFeederVolSteps::eEmptyFeederError:
			StepError();
			break;
		}
		if (m_pDriveCtrl->IsStarted())
		{
			m_pDriveCtrl->Update(m_st);
		}
	}
	return result;
}
//*********************************************************************************************
//*********************************************************************************************
BOOL CEmptyFeederVolTask::InitExecute()
{
	auto result = CBaseClass::InitExecute();
	if (result)
	{
		m_AdsClient.Init();
	}
	return result;
}
//*********************************************************************************************
//*********************************************************************************************
BOOL CEmptyFeederVolTask::ExitExecute()
{
	auto result = CBaseClass::IsInit();
	if (result)
	{
		m_AdsClient.Exit();
		result = CBaseClass::ExitExecute();
	}
	return result;
}

