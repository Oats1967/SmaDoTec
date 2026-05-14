//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Implementation of module CalibGravTask
///
/// @file   CalibGravTask.cpp
///
///
/// @coypright Ing.büro Hafer
///            Branderweg 8A
///            D-91058 Erlangen
///
/// @author    Detlef Hafer
///
//------------------------------------------------------------------------------------
#include "BASE/Base.h"
#include "DoseDataLib/include/DoseData.h"
#include "CalibGravTask.h"
#include "DriveControl.h"
#include "WeightControl.h"
#include "AlarmManager.h"



#define RANGEDRIVECOMMAND(f) RANGE(f, 0.0, 1.0)


using namespace base;
using namespace base::calib;



//**************************************************************************************************
//**************************************************************************************************
void CCalibGravTask::registerDriveCtrl(CDriveControl& encoderCtrl)
{
	m_pDriveCtrl = &encoderCtrl;
}
//**************************************************************************************************
//**************************************************************************************************
void CCalibGravTask::registerWeightCtrl(CWeightControl& weightCtrl)
{
	m_pWeightCtrl = &weightCtrl;
}
//**************************************************************************************************
//**************************************************************************************************
void CCalibGravTask::registerAlarmManager(CAlarmManager& rAlarm)
{
	m_pAlarm = &rAlarm;
}
//**************************************************************************************************
//**************************************************************************************************
BOOL CCalibGravTask::CheckErrorState(void)
{
	assert(m_pWeightCtrl);
	assert(m_pDriveCtrl);
	assert(m_pAlarm);

	eAlarmError errorid = eAlarmError::ERROR_MAX;

	if (m_CalibCfg.count == 0)
	{
		errorid = eAlarmError::ERROR_CALIB_INVALIDSETUP;
	}
	else
	{
		BOOL bLoadCellError = !m_pWeightCtrl->isLoadcellActive();
		if (bLoadCellError)
		{
			errorid = eAlarmError::ERROR_CALIB_LOADCELL;
		}
		else
		{
			BOOL bRefill = FALSE;
			Dose_DSVGetRefillState(m_sID, &bRefill);
			if (bRefill)
			{
				errorid = eAlarmError::ERROR_CALIB_REFILLACTIVE;
			}
			else
			{
				BOOL bIOReleaseInput = FALSE;
				Dose_DSVGetIOReleaseInput(m_sID, &bIOReleaseInput);
				BOOL bServiceMode = FALSE;
				Dose_EXGetServiceMode(&bServiceMode);
				if ((!bIOReleaseInput) && (!bServiceMode))
				{
					errorid = eAlarmError::ERROR_CALIB_RELEASEFAILED;
				}
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
//**************************************************************************************************
//**************************************************************************************************
BOOL CCalibGravTask::Start(const uint32_t t)
{
	assert(m_pWeightCtrl);
	assert(m_pDriveCtrl);
	assert(m_pAlarm);

	auto result = CStatemachine::Start(t);
	if (result)
	{
		Dose_DSVGetCalibCfg(m_sID, &m_CalibCfg);

		int32_t AktIndex = 0;
		Dose_DSVGetCalibAktIndex(m_sID, &AktIndex);
		m_bRunAll = (AktIndex < 0);
		m_CalibIndex = (m_bRunAll) ? 0 : AktIndex;
		Dose_DSVSetCalibAktIndex(m_sID, m_CalibIndex);
		m_pDriveCtrl->Start(m_st);
		m_LCOldPriority = m_pWeightCtrl->SetPriority(base::LC_PRIORITY::LC_PRIORITY_HIGH);
		assert(m_LCOldPriority == base::LC_PRIORITY::LC_PRIORITY_NORMAL);
		Dose_DSVSetCalibrationState(m_sID, TRUE);
		SetDrivecommand();
		m_LastTime = _U32(-1);
		m_sStep = eCalibGravSteps::eCalibrationRunning;
	}
	return result;
}
//**************************************************************************************************
//**************************************************************************************************
BOOL CCalibGravTask::Stop()
{
	assert(m_pWeightCtrl);
	assert(m_pDriveCtrl);
	assert(m_pAlarm);

	auto result = CStatemachine::IsStarted();
	if (result)
	{
#if _DEBUG
		BOOL bState = FALSE;
		Dose_DSVGetCalibrationState(m_sID, &bState);
		assert(bState);
#endif
		m_pDriveCtrl->Stop();
		m_pWeightCtrl->SetPriority(m_LCOldPriority);
		Dose_DSVSetCalibrationState(m_sID, FALSE);
		m_sStep = eCalibGravSteps::eCalibrationInit;

		m_pAlarm->CheckWarningLevel(eAlarmError::ERROR_CALIB_REFILLACTIVE);
		m_pAlarm->CheckWarningLevel(eAlarmError::ERROR_CALIB_RELEASEFAILED);
		m_pAlarm->CheckWarningLevel(eAlarmError::ERROR_CALIB_LOADCELL);
		m_pAlarm->CheckWarningLevel(eAlarmError::ERROR_CALIB_INVALIDSETUP);
		result = CStatemachine::Stop();
	}
	return result;
}
//**************************************************************************************************
//**************************************************************************************************
void CCalibGravTask::SetDrivecommand()
{
	assert(m_pWeightCtrl);
	assert(m_pDriveCtrl);
	assert(m_pAlarm);
	assert(m_CalibCfg.count > 0);
	assert(m_CalibIndex >= 0 && m_CalibIndex < _S32(m_CalibCfg.count));

	float32_t fDriveCommand = m_CalibCfg.fDriveCommand[m_CalibIndex];
	fDriveCommand /= 100.0f;
	fDriveCommand = _F32(RANGEDRIVECOMMAND(fDriveCommand));
	m_pDriveCtrl->UpdateDriveCommand(fDriveCommand);
#if _DEBUG
	auto& Ltemp = m_pWeightCtrl->GetWeight();
	m_pWeightCtrl->Update(m_st);
#endif
	m_aStartWeight = m_pWeightCtrl->GetWeight();
#if _DEBUG
	assert(m_aStartWeight == Ltemp);
#endif
	m_StartTime = m_st;
}
//**************************************************************************************************
//**************************************************************************************************
void CCalibGravTask::StepError()
{
	Stop();
}
//**************************************************************************************************
//**************************************************************************************************
void CCalibGravTask::StepRunning()
{
	assert(m_pWeightCtrl);
	assert(m_pDriveCtrl);
	assert(m_pAlarm);

	assert(m_CalibCfg.count > 0);
	const float32_t c_CorrectionFactor = 1.2F;

	if (m_st != m_LastTime)
	{
		m_LastTime = m_st;
		auto error = CheckErrorState();
		if ( ! error)
		{
#if _DEBUG
			auto& Ltemp = m_pWeightCtrl->GetWeight();
			m_pWeightCtrl->Update(m_st);
#endif
			auto& weight = m_pWeightCtrl->GetWeight();
#if _DEBUG
			assert(weight == Ltemp);
#endif
			auto aW = weight - m_aStartWeight;
			const float32_t fMassflow = _F32(aW.GetMassflowKgh());
			float32_t fDriveCommand = m_CalibCfg.fDriveCommand[m_CalibIndex];
			fDriveCommand /= 100.0f;
			fDriveCommand = _F32(RANGEDRIVECOMMAND(fDriveCommand));
			const float32_t fPerformance = FDIV0(fMassflow, fDriveCommand) * c_CorrectionFactor;
			m_CalibCfg.fDosePerformance[m_CalibIndex] = _F32(std::max(fPerformance, 1.0F));
			Dose_DSVSetCalibAktIndex(m_sID, m_CalibIndex);
			Dose_DSVSetCalibCfg(m_sID, m_CalibCfg);

			assert(m_st >= m_StartTime);
			if (m_st - m_StartTime > m_CalibCfg.lMeasureTime[m_CalibIndex])
			{
				BOOL bFinished = TRUE;
				if (m_bRunAll)
				{
					if (m_CalibIndex < _S32(m_CalibCfg.count) - 1)
					{
						m_CalibIndex++;
						SetDrivecommand();
						bFinished = FALSE;
					}
				}
				if (bFinished)
				{
					Stop();
				}
			}
		}
		else
		{
			m_sStep = eCalibGravSteps::eCalibrationError;
		}
	}
}
//*********************************************************************************************
//*********************************************************************************************
BOOL CCalibGravTask:: Update(const uint32_t t)
{
	assert(m_pWeightCtrl);
	assert(m_pDriveCtrl);
	assert(m_pAlarm);

	auto result = CStatemachine::Update(t);
	if (result)
	{
		switch (m_sStep)
		{
		default:
			break;

		case eCalibGravSteps::eCalibrationRunning:
			StepRunning();
			break;

		case eCalibGravSteps::eCalibrationError:
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


