//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Implementation of module CWbfTareTask
///
/// @file   CWbfTareTask.cpp
///
///
/// @coypright Ing.büro Hafer
///            Branderweg 8A
///            D-91058 Erlangen
///
/// @author    Detlef Hafer
///
//------------------------------------------------------------------------------------
#include "WbfTareTask.h"
#include "WeightControl.h"
#include "AlarmManager.h"
#include "DriveControl.h"
#include "DoseDataLib/include/DoseData.h"


#define RANGEDRIVECOMMAND(f) RANGE(f, 0.0, 1.0)

using namespace base;


//**************************************************************************************************
//**************************************************************************************************
void CWbfTareTask::registerWeightCtrl(CWeightControl& weightCtrl)
{
	m_pWeightCtrl = &weightCtrl;
}
//**************************************************************************************************
//**************************************************************************************************
void CWbfTareTask::registerAlarmManager(CAlarmManager& rAlarm)
{
	m_pAlarm = &rAlarm;
}
//**************************************************************************************************
//**************************************************************************************************
void CWbfTareTask::registerDriveCtrl(CDriveControl& rDriveControl)
{
	m_pDriveCtrl = &rDriveControl;
}
//**************************************************************************************************
//**************************************************************************************************
BOOL CWbfTareTask::CheckErrorState(void)
{
	assert(m_pWeightCtrl);
	assert(m_pAlarm);

	eAlarmError errorid = eAlarmError::ERROR_MAX;
	BOOL bLoadCellError = ! m_pWeightCtrl->isLoadcellActive();
	if (bLoadCellError)
	{
		errorid = eAlarmError::ERROR_TARE_LOADCELL;
	}
	else
	{
		BOOL bRefill = FALSE;
		Dose_DSVGetRefillState(m_sID, &bRefill);
		if (bRefill)
		{
			errorid = eAlarmError::ERROR_TARE_REFILLACTIVE;
		}
		else
		{
			BOOL bIOReleaseInput = FALSE;
			Dose_DSVGetIOReleaseInput(m_sID, &bIOReleaseInput);
			if (!bIOReleaseInput)
			{
				errorid = eAlarmError::ERROR_TARE_RELEASEFAILED;
			}
		}
	}
	BOOL error = (errorid != eAlarmError::ERROR_MAX);
	if (error)
	{
		m_pAlarm->SetAlarm(m_st, errorid, TRUE, base::eAlarmClass::eALARMTYP);
	}
	return error;
}
//*******************************************************************************************************
//*******************************************************************************************************
BOOL CWbfTareTask::Start(const uint32_t t)
{
	assert(m_pDriveCtrl);

	auto result = CStatemachine::Start(t);
	if (result)
	{
		float32_t fDriveCommand = 0.0F;
		Dose_DSVGetWbfTareDriveCommand(m_sID, &fDriveCommand);
		fDriveCommand /= 100.0f;
		uint32_t lTime = 0L;
		Dose_DSVGetWbfTareMeasurementTime(m_sID, &lTime);
		if ((fDriveCommand > 0) && (lTime > 0))
		{
			auto error = CheckErrorState();
			if (!error)
			{
				Dose_DSVSetTaraState(m_sID, TRUE);
				m_pDriveCtrl->Start(m_st);
				m_sStep = eWbfTareSteps::eTareStart;
				result = TRUE;
			}
			else
			{
				m_sStep = eWbfTareSteps::eTareError;
			}
		}
		else
		{
			m_sStep = eWbfTareSteps::eTareError;
		}
	}
	return result;
}
//**************************************************************************************************
//**************************************************************************************************
void CWbfTareTask::InternalStop()
{
	assert(m_pDriveCtrl);

	BOOL bState = FALSE;
	Dose_DSVGetTaraState(m_sID, &bState);
	if (bState)
	{
		m_pDriveCtrl->Stop();
		Dose_DSVSetTaraState(m_sID, FALSE);
		m_sStep = eWbfTareSteps::eTareInit;
		m_st = 0;
	}
}

//**************************************************************************************************
//**************************************************************************************************
BOOL CWbfTareTask::Stop()
{
	auto result = CStatemachine::IsStarted();
	if (result)
	{
		InternalStop();
		result = CStatemachine::Stop();
	}
	return result;
}
//**************************************************************************************************
//**************************************************************************************************
BOOL CWbfTareTask::InitExecute()
{
	auto result = CStatemachine::InitExecute();
	if (result)
	{
		InternalStop();
	}
	return result;
}
//**************************************************************************************************
//**************************************************************************************************
BOOL CWbfTareTask::ExitExecute()
{
	auto result = CStatemachine::IsInit();
	if (result)
	{
		InternalStop();
		result = CStatemachine::ExitExecute();
	}
	return result;
}
//**************************************************************************************************
//**************************************************************************************************
void CWbfTareTask::SetDrivecommand()
{
	assert(m_pDriveCtrl);

	float32_t fDriveCommand = 0.0F;
	Dose_DSVGetWbfTareDriveCommand(m_sID, &fDriveCommand);
	fDriveCommand /= 100.0f;
	fDriveCommand = _F32(RANGEDRIVECOMMAND(fDriveCommand));
	m_pDriveCtrl->UpdateDriveCommand( fDriveCommand);
}
//*******************************************************************************************************
//*******************************************************************************************************
void CWbfTareTask::StepStart()
{
	assert(m_pWeightCtrl);

	SetDrivecommand();
	m_StartTime = m_st;
	m_fTaraWeightFifo.Init();
	m_pWeightCtrl->Update(m_st);
	m_LastWeight = m_pWeightCtrl->GetWeight();
	float32_t fTaraWeight = m_pWeightCtrl->GetRawWeight().m_fWeight;
	m_fTaraWeightFifo.Add(fTaraWeight);
	m_sStep = eWbfTareSteps::eTareRunning;
}
//*********************************************************************************************
//*********************************************************************************************
void CWbfTareTask::StepStop()
{
#if _DEBUG
	BOOL bState = FALSE;
	Dose_DSVGetTaraState(m_sID, &bState);
	assert(bState);
#endif
	Stop();
}
//**************************************************************************************************
//**************************************************************************************************
void CWbfTareTask::StepError()
{
#if _DEBUG
	BOOL bState = FALSE;
	Dose_DSVGetTaraState(m_sID, &bState);
	assert(bState);
#endif
	Stop();
}
//*********************************************************************************************
//*********************************************************************************************
void CWbfTareTask::StepRunning()
{
	assert(m_pWeightCtrl);

	auto error = CheckErrorState();
	if (! error)
	{
		m_pWeightCtrl->Update(m_st);
		auto& fWeight = m_pWeightCtrl->GetWeight();
		if (fWeight.m_ulT > m_LastWeight.m_ulT)
		{
			m_LastWeight = fWeight;
			float32_t fTaraWeight = m_pWeightCtrl->GetRawWeight().m_fWeight;
			m_fTaraWeightFifo.Add(fTaraWeight);
			fTaraWeight = m_fTaraWeightFifo.GetMean();
			m_pWeightCtrl->SetTaraWeight(fTaraWeight);
		}
		uint32_t lTime = 0L;
		Dose_DSVGetWbfTareMeasurementTime(m_sID, &lTime);
		if (m_st - m_StartTime > lTime)
		{
			m_sStep = eWbfTareSteps::eTareStop;
		}
	}
	else
	{
		m_sStep = eWbfTareSteps::eTareError;
	}
}
//*********************************************************************************************
//*********************************************************************************************
BOOL CWbfTareTask::Update(const uint32_t t)
{
	assert(m_pDriveCtrl);
	assert(m_pAlarm);

	auto result = CStatemachine::Update(t);
	if (result)
	{
		switch (m_sStep)
		{
		default:
			break;

		case eWbfTareSteps::eTareStart:
			StepStart();
			break;

		case eWbfTareSteps::eTareRunning:
			StepRunning();
			break;

		case eWbfTareSteps::eTareStop:
			StepStop();
			break;

		case eWbfTareSteps::eTareError:
			StepError();
			break;
		}
		if (m_pDriveCtrl->IsStarted())
		{
			m_pDriveCtrl->Update(m_st);
		}
		m_pAlarm->CheckWarningLevel(eAlarmError::ERROR_TARE_REFILLACTIVE);
		m_pAlarm->CheckWarningLevel(eAlarmError::ERROR_TARE_RELEASEFAILED);
		m_pAlarm->CheckWarningLevel(eAlarmError::ERROR_TARE_LOADCELL);
	}
	return result;
}
