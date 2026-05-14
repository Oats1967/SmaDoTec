//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Implementation of module EmptyFeederGravTask
///
/// @file   EmptyFeederGravTask.cpp
///
///
/// @coypright Ing.büro Hafer
///            Branderweg 8A
///            D-91058 Erlangen
///
/// @author    Detlef Hafer
///
//------------------------------------------------------------------------------------
#include "EmptyFeederGravTask.h"
#include "DriveControl.h"
#include "WeightControl.h"
#include "AlarmManager.h"
#include "DoseDataLib/include/DoseData.h"


using namespace base;

//**************************************************************************************************
//**************************************************************************************************
void CEmptyFeederGravTask::registerDriveCtrl(CDriveControl& encoderCtrl)
{
	m_pDriveCtrl = &encoderCtrl;
}
//**************************************************************************************************
//**************************************************************************************************
void CEmptyFeederGravTask::registerWeightCtrl(CWeightControl& weightCtrl)
{
	m_pWeightCtrl = &weightCtrl;
}
//**************************************************************************************************
//**************************************************************************************************
void CEmptyFeederGravTask::registerAlarmManager(CAlarmManager& rAlarm)
{
	m_pAlarm = &rAlarm;
}
//**************************************************************************************************
//**************************************************************************************************
BOOL CEmptyFeederGravTask::CheckErrorState(void)
{
	assert(m_pWeightCtrl);
	assert(m_pDriveCtrl);
	assert(m_pAlarm);

	eAlarmError errorid = eAlarmError::ERROR_MAX;
	BOOL bLoadCellError = ! m_pWeightCtrl->isLoadcellActive();
	if (bLoadCellError)
	{
		errorid = eAlarmError::ERROR_EMPTYFEEDER_LOADCELL;
	}
	else
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
BOOL CEmptyFeederGravTask::Start(const uint32_t t)
{
	auto result = CBaseClass::Start(t);
	if ( result )
	{
		assert(m_pWeightCtrl);
		assert(m_pDriveCtrl);

		SetState(TRUE);
		m_pDriveCtrl->Start(m_st);
		StoreLCPriority();
		StoreAlarms();
		StoreRefillReleaseState();
		InitWeight();
		InitDrivecommand();
		uint32_t totalTime = 0;
		Dose_DSVGetEmptyFeederRuntime(m_sID, &totalTime); // Save AlarmDriveCommandHigh value
		m_ElapsedTime = m_st + totalTime;
		m_Counter = 0;
		m_LastTime = _U32(-1);
		m_sStep = eEmptyFeederSteps::eEmptyFeederRunning;
	}
	return result;
}
//**************************************************************************************************
//**************************************************************************************************
BOOL CEmptyFeederGravTask::Stop()
{ 
	assert(m_pWeightCtrl);
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
		RestoreLCPriority();

		SetState(FALSE);

		m_pAlarm->CheckWarningLevel(eAlarmError::ERROR_EMPTYFEEDER_LOADCELL);
		m_pAlarm->CheckWarningLevel(eAlarmError::ERROR_EMPTYFEEDER_RELEASEFAILED);
		m_pAlarm->CheckWarningLevel(eAlarmError::ERROR_EMPTYFEEDER_TIMEOUT);
		m_pAlarm->CheckWarningLevel(eAlarmError::ERROR_EMPTYFEEDER_GENERAL);
		m_sStep = eEmptyFeederSteps::eEmptyFeederInit;
		result = CBaseClass::Stop();
	}
	return result;
}
//**************************************************************************************************
//**************************************************************************************************
void CEmptyFeederGravTask::SetState(const BOOL bState)
{
	m_AdsClient.SetEmptyFeeder(bState);
}
//**************************************************************************************************
//**************************************************************************************************
void CEmptyFeederGravTask::StoreAlarms()
{
	Dose_DSVGetAlarmDriveCommandHigh(m_sID, &m_AlarmDriveCommandHigh); // Save AlarmDriveCommandHigh value
	Dose_DSVSetAlarmDriveCommandHigh(m_sID, 1000U);					   // Don't trigger alarm
}
//**************************************************************************************************
//**************************************************************************************************
void CEmptyFeederGravTask::RestoreAlarms() const
{
	Dose_DSVSetAlarmDriveCommandHigh(m_sID, m_AlarmDriveCommandHigh); // restore old value
}
//**************************************************************************************************
//**************************************************************************************************
void CEmptyFeederGravTask::InitDrivecommand()
{
	float32_t fDriveCommand = 100.0F;
	Dose_DSVPopEmptyFeederSpeed(m_sID, &fDriveCommand);
	m_pDriveCtrl->UpdateDriveCommand(fDriveCommand / 100.0F);
}
//**************************************************************************************************
//**************************************************************************************************
void CEmptyFeederGravTask::CheckDrivecommand()
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
void CEmptyFeederGravTask::StoreRefillReleaseState()
{
	Dose_DSVGetRefillReleaseState(m_sID, &m_RefillReleaseState);
	if (m_RefillReleaseState)
	{
		Dose_DSVSetSWRefillRelease(m_sID, FALSE);
	}
}
//**************************************************************************************************
//**************************************************************************************************
void CEmptyFeederGravTask::RestoreRefillReleaseState() const
{
	Dose_DSVSetSWRefillRelease(m_sID, m_RefillReleaseState);
}
//**************************************************************************************************
//**************************************************************************************************
void CEmptyFeederGravTask::StoreLCPriority()
{
	assert(m_pWeightCtrl);
	m_LCOldPriority =  m_pWeightCtrl->SetPriority(base::LC_PRIORITY::LC_PRIORITY_HIGH);
	assert(m_LCOldPriority == base::LC_PRIORITY::LC_PRIORITY_NORMAL);
}
//**************************************************************************************************
//**************************************************************************************************
void CEmptyFeederGravTask::RestoreLCPriority() const
{
	assert(m_pWeightCtrl);
	m_pWeightCtrl->SetPriority(m_LCOldPriority);
}
//**************************************************************************************************
//**************************************************************************************************
void CEmptyFeederGravTask::CheckRefillReleaseState() const
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
void CEmptyFeederGravTask::InitWeight()
{
	assert(m_pWeightCtrl);
#if _DEBUG
	auto& Ltemp = m_pWeightCtrl->GetWeight();
	m_pWeightCtrl->Update(m_st);
#endif
	m_LastWeight = m_pWeightCtrl->GetWeight();
#if _DEBUG
	assert(Ltemp == m_LastWeight);
#endif
	m_RingBuffer.Init();
	m_RingBuffer.Add(m_LastWeight.m_fWeight);
}
//**************************************************************************************************
//**************************************************************************************************
void CEmptyFeederGravTask::StepError()
{
	Stop();
}
//**************************************************************************************************
//**************************************************************************************************
void CEmptyFeederGravTask::StepRunning()
{
	const float32_t cDelta = 0.100F;
	const uint32_t c_MaxCounter = 6U;

	assert(m_pWeightCtrl);
	assert(m_pDriveCtrl);
	if ( m_st != m_LastTime)
	{
		m_LastTime = m_st;
		auto error = CheckErrorState();
		if ( ! error)
		{
			CheckDrivecommand();
			CheckRefillReleaseState();
#if _DEBUG
			auto& Ltemp =  m_pWeightCtrl->GetWeight();
			m_pWeightCtrl->Update(m_st);
#endif
			auto& weight =  m_pWeightCtrl->GetWeight();
#if _DEBUG
			assert(Ltemp == weight);
#endif
			if (m_LastWeight.m_ulT != weight.m_ulT)
			{
				auto fMinnWeight = m_RingBuffer.GetMin();
				if (weight.m_fWeight < fMinnWeight)
				{
					m_Counter = 0;
				}
				else
				{
					if (weight.m_fWeight < fMinnWeight + cDelta)
					{
						m_Counter++;
						if (m_Counter >= c_MaxCounter)
						{
							Stop();
						}
					}
					else
					{
						m_Counter = 0;
					}
				}
				m_LastWeight = weight;
				m_RingBuffer.Add(weight.m_fWeight);
			}
		}
		else
		{
			m_sStep = eEmptyFeederSteps::eEmptyFeederError;
		}
	}
}
//*********************************************************************************************
//*********************************************************************************************
BOOL CEmptyFeederGravTask:: Update(const uint32_t t)
{
	auto result = CBaseClass::Update(t);
	if (result)
	{
		assert(m_pWeightCtrl);
		assert(m_pDriveCtrl);

		switch (m_sStep)
		{
		default:
			break;

		case eEmptyFeederSteps::eEmptyFeederRunning:
			StepRunning();
			break;

		case eEmptyFeederSteps::eEmptyFeederError:
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
BOOL CEmptyFeederGravTask::InitExecute()
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
BOOL CEmptyFeederGravTask::ExitExecute()
{
	auto result = CBaseClass::IsInit();
	if (result)
	{
		m_AdsClient.Exit();
		result = CBaseClass::ExitExecute();
	}
	return result;
}

