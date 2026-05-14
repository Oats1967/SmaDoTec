//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Implementation of module BatchControl
///
/// @file   BatchControl.cpp
///
///
/// @coypright Ing.büro Hafer
///            Branderweg 8A
///            D-91058 Erlangen
///
/// @author    Detlef Hafer
///
//------------------------------------------------------------------------------------
#include <cmath>
#include "DoseDataLib/include/DoseData.h"
#include "AdsClient/include/AdsClient.h"
#include "BatchControl.h"
#include "RefillControl.h"
#include "AgitatorControl.h"
#include "Logger/include/Logger.h"



using namespace base;

#define RANGEDRIVECOMMAND(f) RANGE(f, 0.0F, 1.0F)
#define GAIN 1
#define LOSS (-1)


#if _DEBUG

static const char *szStep[12] = { "eIdle",
	"ePrepareStartup",
	"eWaitBatchRefill",
	"eWaitBatchRefillDelay",
	"eWBatchGrossStream",
	"eBatchFineStream" };

#define LOGFUNCTION \
{\
	{ \
		LOGDEBUG(__FUNCTION__ << "(t = " << m_st << ") Step = " << szStep[_S32(m_ControlSteps)]); \
	}\
}

#define LOGFUNCTIONSTEP(__a) \
{\
	if (m_LastControlSteps != __a) \
	{ \
		m_LastControlSteps = __a; \
		LOGDEBUG(__FUNCTION__ << "(t = " << m_st << ") Step = " << szStep[m_LastControlSteps]);  \
	}\
}

#else
#define LOGFUNCTION
#define LOGFUNCTIONSTEP(__a)
#endif



//*********************************************************************************************
//*********************************************************************************************
CBatchControl::CBatchControl(int32_t s) : CBaseControl(s)
, m_DriveCtrl(s)
, m_MeterCtrl(s)
, m_pRefillCtrl{ nullptr }
, m_pAgitatorCtrl{ nullptr }
, m_BatchTimeAlarmCtrl(s)
, m_fLastDriveCommand{ 0.0F }
, m_ControlSteps{ eControlSteps::eIdle }
#if _DEBUG
, m_LastControlSteps{ eControlSteps::eIdle }
#endif
, m_tNext(0L)
, m_tWeightNext(0L)
{
	m_DriveCtrl.registerAlarmManager(this->m_aAlarm);
	m_BatchTimeAlarmCtrl.registerAlarmManager(this->m_aAlarm);
	Dose_DSVSetRefillControlEnable(m_sID, FALSE);
	Dose_DSVSetAgitatorEnable(m_sID, FALSE);
}
//*********************************************************************************************
//*********************************************************************************************
void CBatchControl::registerRefillControl(CRefillControl* pRefillCtrl)
{
	if (pRefillCtrl)
	{
		pRefillCtrl->registerAlarmManager(this->m_aAlarm);
	}
	m_pRefillCtrl = pRefillCtrl;
}
//*********************************************************************************************
//*********************************************************************************************
void CBatchControl::registerAgitatorControl(CAgitatorControl* pAgitatorCtrl)
{
	m_pAgitatorCtrl = pAgitatorCtrl;
}
//*********************************************************************************************
//*********************************************************************************************
void CBatchControl::UpdateSwitchPoint()
{
	BOOL bChanged = FALSE;
	Dose_DSVGetChangedBatchSwitchpoint(m_sID, &bChanged);
	if ( bChanged )
	{
		CalcSwitchPoint();
	}
}
//*********************************************************************************************
//*********************************************************************************************
void CBatchControl::RefillUpdate(void)
{
	if (m_pRefillCtrl)
	{
		m_pRefillCtrl->Update(m_st);
	}
}
//*********************************************************************************************
//*********************************************************************************************
void CBatchControl :: SetDriveCommand ( const float32_t drive )
{	
	assert(drive >= 0.0F && drive <= 1.0F);
	m_DriveCtrl.UpdateDriveCommand(drive);
}
//*********************************************************************************************
//*********************************************************************************************
void CBatchControl :: InitPrepareStartup ( void)
{
	LOGFUNCTION

	uint32_t lWaitTime = 0L;
	Dose_DSVGetStartupDelay(m_sID, &lWaitTime);
	BOOL bEnable = FALSE;
	Dose_DSVGetRefillControlEnable(m_sID, &bEnable);
	if ( bEnable )
	{
		float32_t fMin = 0.0F;
		Dose_DSVGetRefillLimitMin(m_sID, &fMin);
		float32_t fSetpoint = 0.0F;
		Dose_DSVGetActualSetpoint(m_sID, &fSetpoint);
		if (fSetpoint < fMin)
		{
			Dose_DSVSetRefillLimitMin(m_sID, fSetpoint);
		}
		Dose_DSVPBSetPBRefillStart(m_sID);
		lWaitTime = __max(lWaitTime, 2U);
	}
	m_tNext = m_st + lWaitTime;
	m_ControlSteps = eControlSteps::ePrepareStartup;
}
//*********************************************************************************************
//*********************************************************************************************
void CBatchControl::StepPrepareStartup(void)
{
	LOGFUNCTION

	if (m_st >= m_tNext)
	{
		FinishPrepareStartup();
	}
}
//*********************************************************************************************
//*********************************************************************************************
void CBatchControl::FinishPrepareStartup(void)
{
	BOOL bRefill;
	Dose_DSVGetRefillState(m_sID, &bRefill);
	if ( ! bRefill)
	{
		InitStartBatch();
	}
	else
	{
		InitWaitBatchRefill();
	}
}
//*********************************************************************************************
//*********************************************************************************************
void CBatchControl::InitWaitBatchRefill(void)
{
	LOGFUNCTION
		// Nachfuellen starten
	m_ControlSteps = eControlSteps::eWaitBatchRefill;

}
//*********************************************************************************************
//*********************************************************************************************
void CBatchControl::StepWaitBatchRefill(void)
{
	LOGFUNCTION
		// Nachfuellen starten
	BOOL bRefill = FALSE;
	Dose_DSVGetRefillState(m_sID, &bRefill);
	if (! bRefill)
	{
		InitBatchRefillDelay();
	}
}
//*********************************************************************************************
//*********************************************************************************************
void CBatchControl::InitBatchRefillDelay(void)
{
	LOGFUNCTION

	uint32_t lDelay = 0;
	Dose_DSVGetRefillSwitchDelay(m_sID, &lDelay);
	m_tNext = m_st + lDelay;
	m_ControlSteps = eControlSteps::eWaitBatchRefillDelay;
}
//*********************************************************************************************
//*********************************************************************************************
void CBatchControl::StepBatchRefillDelay(void)
{
	LOGFUNCTION

	if (m_st >= m_tNext)
	{
		FinishBatchRefillDelay();
	}
}
//*********************************************************************************************
//*********************************************************************************************
void CBatchControl::FinishBatchRefillDelay(void)
{
	InitStartBatch();
}

//*********************************************************************************************
//*********************************************************************************************
void CBatchControl::InitStartBatch(void)
{
#if _DEBUG
	BOOL bUpdate = UpdateWeight();
	assert(bUpdate);
#else
	(void)UpdateWeight();
#endif

	BOOL bStarted = FALSE;
	Dose_DSVGetBatchStarted(m_sID, &bStarted);
	if (! bStarted)
	{
		StartBatch();
	}
	else
	{
		ContinueBatch();
	}
	Dose_DSVSetBatchStarted(m_sID, TRUE);
	Dose_DSVSetBatchFinished(m_sID, FALSE);
	AdsClient_DoseSetBatchFinished(m_sID, FALSE);

	if (IsBatchFinished())
	{
		InitFinishBatch();
	}
	else if ( IsSwitchPointReached() )
	{
		InitBatchFineStreamControl();
	}
	else
	{
		InitBatchGrossStreamControl();
	}
}
//*********************************************************************************************
//*********************************************************************************************
void CBatchControl::InitBatchGrossStreamDriveCommand(void)
{
	float32_t fGrossStream = 0.0;
	Dose_DSVGetBatchGrossStream(m_sID, &fGrossStream);
	m_fLastDriveCommand = __max(fGrossStream / 100.0F, 0.01F);
	m_fLastDriveCommand = RANGEDRIVECOMMAND(m_fLastDriveCommand);
	SetDriveCommand(m_fLastDriveCommand);
}
//*********************************************************************************************
//*********************************************************************************************
void CBatchControl::UpdateBatchGrossStreamDriveCommand(void)
{
	BOOL bChanged = FALSE;

	Dose_DSVGetChangedBatchGrossStream(m_sID, &bChanged);
	if (bChanged)
	{
		float32_t fGrossStream = 0.0;
		Dose_DSVGetBatchGrossStream(m_sID, &fGrossStream);
		m_fLastDriveCommand = __max(fGrossStream / 100.0F, 0.01F);
		m_fLastDriveCommand = RANGEDRIVECOMMAND(m_fLastDriveCommand);
		SetDriveCommand(m_fLastDriveCommand);
	}
}
//*********************************************************************************************
//*********************************************************************************************
void CBatchControl::InitBatchGrossStreamControl(void)
{
	InitBatchGrossStreamDriveCommand();
	m_ControlSteps = eControlSteps::eWBatchGrossStream;
}


//*********************************************************************************************
//*********************************************************************************************
void CBatchControl::StepBatchGrossStreamControl(void)
{
	auto bWeightUpdate = UpdateWeight();
	if (bWeightUpdate)
	{
		UpdateTotalizer();
		if (IsBatchFinished())
		{
			InitFinishBatch();
		}
		else
		{
			UpdateSwitchPoint();
			if (IsSwitchPointReached())
			{
				InitBatchFineStreamControl();
			}
			else
			{
				UpdateBatchGrossStreamDriveCommand();
			}
		}
	}
}
//*********************************************************************************************
//*********************************************************************************************
void CBatchControl::InitBatchFineStreamDriveCommand(void)
{
	float32_t fFineStream = 0.0F;
	Dose_DSVGetBatchFineStream(m_sID, &fFineStream);
	fFineStream = __max(fFineStream / 100.0F, 0.01F);
	m_fLastDriveCommand = __max(m_fLastDriveCommand / 2.0F, fFineStream);
	m_fLastDriveCommand = RANGEDRIVECOMMAND(m_fLastDriveCommand);
	SetDriveCommand(m_fLastDriveCommand);
}
//*********************************************************************************************
//*********************************************************************************************
void CBatchControl::UpdateBatchFineStreamDriveCommand(void)
{
	float32_t fFineStream = 0.0F;
	Dose_DSVGetBatchFineStream(m_sID, &fFineStream);
	fFineStream = __max(fFineStream / 100.0F, 0.01F);
	m_fLastDriveCommand = __max(m_fLastDriveCommand / 2.0F, fFineStream);
	m_fLastDriveCommand = RANGEDRIVECOMMAND(m_fLastDriveCommand);
	SetDriveCommand(m_fLastDriveCommand);
}

//*********************************************************************************************
//*********************************************************************************************
void CBatchControl::InitBatchFineStreamControl(void)
{
	InitBatchFineStreamDriveCommand();
	m_ControlSteps = eControlSteps::eBatchFineStream;
}
//*********************************************************************************************
//*********************************************************************************************
void CBatchControl::StepBatchFineStreamControl(void)
{
	auto bWeightUpdate = UpdateWeight();
	if (bWeightUpdate)
	{
		UpdateTotalizer();
		if (IsBatchFinished())
		{
			InitFinishBatch();
		}
		else
		{
			UpdateSwitchPoint();
			if (IsReturnGrossStream())
			{
				InitBatchGrossStreamControl();
			}
			else
			{
				UpdateBatchFineStreamDriveCommand();
			}
		}
	}
}
//*********************************************************************************************
//*********************************************************************************************
void	CBatchControl :: Control()
{
	switch (m_ControlSteps)
	{
		default:
		case eControlSteps::eIdle:
			break;

		case eControlSteps::ePrepareStartup:
		{
			StepPrepareStartup();
		}
		break;

		case eControlSteps::eWaitBatchRefill:
			{
				StepWaitBatchRefill();
			}
			break;

		case eControlSteps::eWaitBatchRefillDelay:
			{
				StepBatchRefillDelay();
			}
			break;

		case eControlSteps::eWBatchGrossStream:
			{
				StepBatchGrossStreamControl();
			}
			break;

		case eControlSteps::eBatchFineStream:
			{
				StepBatchFineStreamControl();
			}
			break;
	}
	if (m_DriveCtrl.IsStarted())
	{
		m_DriveCtrl.Update(m_st);
	}
	if (m_BatchTimeAlarmCtrl.IsStarted())
	{
		m_BatchTimeAlarmCtrl.Update(m_st);
	}
	CheckExtAlarm();
}
//*********************************************************************************************
//*********************************************************************************************
void CBatchControl :: ClearAlarm   (const base::eAlarmError lError)
{
    CBaseControl :: ClearAlarm (lError);
	if (m_pRefillCtrl)
	{
		m_pRefillCtrl->ClearAlarm(lError);
	}
}
//*********************************************************************************************
//*********************************************************************************************
void CBatchControl::InitFinishBatch(void)
{
	Stop();
	Dose_DSVSetBatchStarted(m_sID, FALSE);
	Dose_DSVSetBatchFinished(m_sID, TRUE);
	AdsClient_DoseSetBatchFinished(m_sID, TRUE);
}

//*********************************************************************************************
//*********************************************************************************************
BOOL CBatchControl :: Stop ( void)
{
	auto result = CBaseControl :: Stop();
    if (result)
    {
		LOGFUNCTION
		m_DriveCtrl.Stop();
		m_BatchTimeAlarmCtrl.Stop();
		m_ControlSteps = eControlSteps::eIdle;
		UpdateTotalizer();
    }
	return result;
}
//*********************************************************************************************
//*********************************************************************************************
BOOL CBatchControl :: Start(const uint32_t t)
{
 	auto result = CBaseControl :: Start(t);
    if ( result )
    {
		LOGFUNCTION

		m_DriveCtrl.Start(m_st);
		m_BatchTimeAlarmCtrl.Start(m_st);

		assert((!m_pRefillCtrl) || m_pRefillCtrl->IsStarted());
		InitPrepareStartup();
	}
	return result;
}
//*********************************************************************************************
//*********************************************************************************************
void CBatchControl::AgitatorUpdate()
{
	if (m_pAgitatorCtrl)
	{
		m_pAgitatorCtrl->Update(m_st);
	}
}
//*********************************************************************************************
//*********************************************************************************************
BOOL CBatchControl::ExitExecute(void)
{
	LOGFUNCTION

	auto result = CBaseControl::IsInit();
	if ( result )
	{
		if (m_pRefillCtrl)
		{
			m_pRefillCtrl->ExitExecute();
		}
		if (m_pAgitatorCtrl)
		{
			m_pAgitatorCtrl->ExitExecute();
		}
		m_DriveCtrl.ExitExecute();
		m_BatchTimeAlarmCtrl.ExitExecute();
		m_fLastDriveCommand = 0.0F;

		Dose_DSVSetBatchStarted(m_sID, FALSE);
		Dose_DSVSetBatchFinished(m_sID, FALSE);
		AdsClient_DoseSetBatchFinished(m_sID, FALSE);
		result = CBaseControl::ExitExecute();
	}
	return result;
}
//*********************************************************************************************
//*********************************************************************************************
BOOL CBatchControl::InitExecute(void)
{
	LOGFUNCTION

	auto result = CBaseControl::InitExecute();
	if ( result)
	{
		if (m_pRefillCtrl)
		{
			m_pRefillCtrl->InitExecute();
			m_pRefillCtrl->Start(m_st);
		}
		if (m_pAgitatorCtrl)
		{
			m_pAgitatorCtrl->InitExecute();
		}
		m_tNext = 0L;
		m_tWeightNext = 0;
		m_fLastDriveCommand = 0.0F;

		Dose_DSVSetBatchStarted(m_sID, FALSE);
		Dose_DSVSetBatchFinished(m_sID, FALSE);
		AdsClient_DoseSetBatchFinished(m_sID, FALSE);

		m_DriveCtrl.InitExecute();
		m_BatchTimeAlarmCtrl.InitExecute();

		m_ControlSteps = eControlSteps::eIdle;
	}
	return result;
}
//******************************************************************************************************
//******************************************************************************************************
BOOL CBatchControl::GetValidSetpoint(void)
{
	float32_t fSetPoint = 0.0f;
	Dose_DSVGetActualSetpoint(m_sID, &fSetPoint);
	return BOOL(fSetPoint > 1e-06);
}
//*********************************************************************************************
//*********************************************************************************************
BOOL CBatchControl::GetDoseSetpoint(void)
{
	CBaseControl::GetLineSetpoint();

	BOOL bChanged = FALSE;
	Dose_DSVGetChangedNominalSetpoint(m_sID, &bChanged);
	if (bChanged)
	{
		float32_t fSetpoint = 0.0F;
		Dose_DSVPopNominalSetpoint(m_sID, &fSetpoint);

		float32_t fSetpointLimit = 0.0f;
		Dose_DSVGetMinSetpointChange(m_sID, &fSetpointLimit);
		fSetpointLimit = RANGE(fSetpointLimit, 0, MAX_SETPOINT_LIMIT_GRAV);

		float32_t fActualSetpoint = 0.0F;
		Dose_DSVGetActualSetpoint(m_sID, &fActualSetpoint);
		const float32_t epsilon = (fActualSetpoint * (fSetpointLimit / 100.0f));

		bChanged = _F32(fabs(fSetpoint - fActualSetpoint)) > epsilon;
		if (bChanged)
		{
			Dose_DSVSetActualSetpoint(m_sID, fSetpoint);
		}
	}
	return bChanged;
}
//*********************************************************************************************
//*********************************************************************************************
BOOL CBatchControl::Execute(void)
{
	assert(IsInit());

	// aktuelle Zeit holen
	auto result = CBaseControl::Execute();
	if (result)
	{
		GetDoseSetpoint();

		RefillUpdate();

		AgitatorUpdate();
	}
	return result;
}


