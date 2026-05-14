//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Implementation of module LwfTareTask
///
/// @file   LwfTareTask.cpp
///
///
/// @coypright Ing.büro Hafer
///            Branderweg 8A
///            D-91058 Erlangen
///
/// @author    Detlef Hafer
///
//------------------------------------------------------------------------------------
#include "LwfTareTask.h"
#include "WeightControl.h"
#include "AlarmManager.h"
#include "DriveControl.h"
#include "DoseDataLib/include/DoseData.h"


using namespace base;

//**************************************************************************************************
//**************************************************************************************************
void CLwfTareTask::registerWeightCtrl(CWeightControl& weightCtrl)
{
	m_pWeightCtrl = &weightCtrl;
}
//**************************************************************************************************
//**************************************************************************************************
void CLwfTareTask::registerAlarmManager(CAlarmManager& rAlarm)
{
	m_pAlarm = &rAlarm;
}

//**************************************************************************************************
//**************************************************************************************************
BOOL CLwfTareTask::CheckErrorState(void)
{
	assert(m_pWeightCtrl);
	assert(m_pAlarm);

	eAlarmError errorid = eAlarmError::ERROR_MAX;
	BOOL bLoadCellError = m_pWeightCtrl->isLoadcellError();
	if (bLoadCellError)
	{
		errorid = eAlarmError::ERROR_TARE_LOADCELL;
	}
	BOOL error = (errorid != eAlarmError::ERROR_MAX);
	if (error)
	{
		m_pAlarm->SetAlarm(m_st, errorid, TRUE, base::eAlarmClass::eALARMTYP);
	}
	return error;
}
//*********************************************************************************************
//*********************************************************************************************
void CLwfTareTask::StepRunning()
{
	auto error = CheckErrorState();
	if ( ! error)
	{
		assert(m_pWeightCtrl);

#if _DEBUG
		auto& Ltemp = m_pWeightCtrl->GetWeight();
		m_pWeightCtrl->Update(m_st);
#endif
		auto& L1 = m_pWeightCtrl->GetWeight();
#if _DEBUG
		assert(Ltemp == L1);
#endif
#if _DEBUG
#ifndef __SIM_WEIGHT_GUI
		if ((L1.m_ulT > m_L0.m_ulT) && (L1.m_ulT - m_L0.m_ulT > 1000))
#endif
#else
		if (L1.m_ulT > m_L0.m_ulT)
#endif
		{
			m_pWeightCtrl->UpdateTaraWeight();
			Stop();
		}
	}
	else
	{
		m_sStep = eLwfTareSteps::eTareError;
	}
}
//*********************************************************************************************
//*********************************************************************************************
void CLwfTareTask::StepError()
{
	Stop();
}
//*********************************************************************************************
//*********************************************************************************************
BOOL CLwfTareTask::Update(const uint32_t t)
{
	auto result = CStatemachine::Update(t);
	if (result)
	{
		switch (m_sStep)
		{
		default:
			break;

		case eLwfTareSteps::eTareRunning:
			StepRunning();
			break;

		case eLwfTareSteps::eTareError:
			StepError();
			break;
		}
	}
	return result;
}
//*********************************************************************************************
//*********************************************************************************************
BOOL CLwfTareTask::Start(const uint32_t t)
{
	auto result = CStatemachine::Start(t);
	if ( result )
	{
		assert(m_pWeightCtrl);
		m_L0 = m_pWeightCtrl->GetWeight();
		m_LCOldPriority = m_pWeightCtrl->SetPriority(base::LC_PRIORITY::LC_PRIORITY_HIGH);
		assert(m_LCOldPriority == base::LC_PRIORITY::LC_PRIORITY_NORMAL);
		Dose_DSVSetTaraState(m_sID, TRUE);
		m_sStep = eLwfTareSteps::eTareRunning;
	}
	return result;
}
//*********************************************************************************************
//*********************************************************************************************
BOOL CLwfTareTask::Stop()
{
	auto result = CStatemachine::IsStarted();
	if ( result)
	{
		assert(m_pWeightCtrl);
		assert(m_pAlarm);

#if _DEBUG
		BOOL bState = FALSE;
		Dose_DSVGetTaraState(m_sID, &bState);
		assert(bState);
#endif
		m_pWeightCtrl->SetPriority(m_LCOldPriority);
		Dose_DSVSetTaraState(m_sID, FALSE);
		m_sStep = eLwfTareSteps::eTareInit;
		m_pAlarm->CheckWarningLevel(eAlarmError::ERROR_TARE_LOADCELL);
		result = CStatemachine::Stop();
	}
	return result;
}
