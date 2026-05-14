//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Implementation of module DriveControl
///
/// @file   DriveControl.cpp
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
#include "DriveControl.h"
#include "DoseDataLib/include/DoseData.h"
#include "Logger/include/Logger.h"


using namespace base;

//*****************************************************************************************************************
//*****************************************************************************************************************
inline void CDriveControl::SetDriveCommand(const float32_t value)
{
	if ( ( ! m_bValidDriveCommand)  && (value > 0.0F))
	{
		m_bValidDriveCommand = TRUE;
		m_tSampleTime = m_st;
		m_tStartTime  = m_st;
		Dose_DSVSetDriveControlReady(m_sID, TRUE);
	}
	m_AdsClient.SetDriveCommand(value);
}
//*****************************************************************************************************************
//*****************************************************************************************************************
inline void CDriveControl::SetOperate(const BOOL bRun)
{
	m_AdsClient.SetOperate(bRun);
}
//*****************************************************************************************************************
//*****************************************************************************************************************
inline void CDriveControl::SetDriveRelease(const BOOL bRelease)
{
	m_AdsClient.SetDriveRelease(bRelease);
	m_AdsClient.SetStart(bRelease);
}

//*****************************************************************************************************************
//*****************************************************************************************************************
void CDriveControl::InternalStop()
{
	SetDriveCommand(0.0F);
	SetDriveRelease(FALSE);
	SetOperate(FALSE);
	Dose_DSVSetDriveControlReady(m_sID, FALSE);
	m_bValidDriveCommand = FALSE;
	Dose_DSVSetActualRotationalSpeed(m_sID, 0.0F);
	Dose_DSVSetEncoderState(m_sID, eEncoderState::ENCODERSTATE_OFF);
}
//*****************************************************************************************************************
//*****************************************************************************************************************
uint32_t CDriveControl::GetActEncoderSpeed() const
{
	float32_t fMaxRotationalSpeed = 0.0F;
	Dose_DSVGetMaxRotationalSpeed(m_sID, &fMaxRotationalSpeed);
	assert(fMaxRotationalSpeed > 0.0F);
	const uint32_t lMaxRotationalSpeed = _U32(NEXTLONG(fMaxRotationalSpeed));
	float32_t fEncoder = 0.0f;
	Dose_DSVGetActualLogoEncoder(m_sID, &fEncoder); // [ 0..1 ]
	assert(fEncoder >= 0.0F && fEncoder <= 1.0F);
	uint32_t ulDrehgeber = NEXTLONG(fEncoder * fMaxRotationalSpeed);
	ulDrehgeber = __min(ulDrehgeber, lMaxRotationalSpeed);
	return ulDrehgeber;
}
//*****************************************************************************************************************
//*****************************************************************************************************************
void CDriveControl::CheckDriveErrorPaddle(const uint32_t t)
{
	BOOL bDriveErrorPaddle = FALSE;
	Dose_DSVGetIODriveErrorPaddleInput(m_sID, &bDriveErrorPaddle);
	m_pAlarm->SetAlarm(t, eAlarmError::ERROR_DOSE_DRIVEERRORPADDLE, bDriveErrorPaddle, eAlarmClass::eALARMTYP);
}
//*****************************************************************************************************************
//*****************************************************************************************************************
void CDriveControl::CheckDriveError(const uint32_t t)
{
	BOOL bDriveError = FALSE;
	Dose_DSVGetIODriveErrorInput(m_sID, &bDriveError);
	m_pAlarm->SetAlarm(t, eAlarmError::ERROR_DOSE_DRIVEERROR, bDriveError, eAlarmClass::eALARMTYP);
}
//*****************************************************************************************************************
//*****************************************************************************************************************
void CDriveControl::CheckMotorLoad(const uint32_t t)
{
	BOOL bMotorLoadError = FALSE;

	Dose_DSVGetIOMotorLoadInput(m_sID, &bMotorLoadError);
	m_pAlarm->SetAlarm(t, eAlarmError::ERROR_DOSE_MOTORLOAD, bMotorLoadError, eAlarmClass::eALARMTYP);
}
//*****************************************************************************************************************
//*****************************************************************************************************************
BOOL CDriveControl::GetRunState() const
{
	const auto ulDrehgeber = GetActEncoderSpeed();
	Dose_DSVSetActualRotationalSpeed(m_sID, _F32(ulDrehgeber));

	uint32_t lEncoderMonitor = 0L;
	Dose_DSVGetEncoderMonitor(m_sID, &lEncoderMonitor);

	BOOL bRun = FALSE;
	auto sEncoderState = eEncoderState::ENCODERSTATE_UNDEFINED;
	if (lEncoderMonitor == 0L)
	{
		sEncoderState = eEncoderState::ENCODERSTATE_OFF;
		bRun = ulDrehgeber > 0;
	}
	else
	{
		if (ulDrehgeber >= lEncoderMonitor)
		{
			sEncoderState = eEncoderState::ENCODERSTATE_RUN;
			bRun = TRUE;
		}
		else
		{
			sEncoderState = eEncoderState::ENCODERSTATE_STOP;
			bRun = FALSE;
		}
	}
	Dose_DSVSetEncoderState(m_sID, sEncoderState);
	return bRun;
}
//*****************************************************************************************************************
//*****************************************************************************************************************
void CDriveControl::Check(const uint32_t t)
{
	assert(IsStarted());

	auto bRun = GetRunState();
	SetOperate(bRun);

	if (t > m_tStartTime + 2U)
	{
		BOOL bRunControl = FALSE;
		Dose_DSVGetLogoRunControlEnable(m_sID, &bRunControl);

		BOOL bOperate = FALSE;
		Dose_DSVGetIOOperateOutput(m_sID, &bOperate);

		if (bOperate && (!bRun))
		{
			// Während der Betriebsphase -> Sofort Fehler melden
			if (bRunControl)
			{
				m_tSampleTime = t;
				m_pAlarm->SetAlarm(t, eAlarmError::ERROR_DOSE_ENCODER, (bRun == FALSE), eAlarmClass::eALARMTYP);
			}
			else
			{
				m_pAlarm->SetAlarm(t, eAlarmError::ERROR_DOSE_ENCODER_SPEED_LOW, !bRun, eAlarmClass::eWARNTYP);
			}
		}
		else
		{
			// Beim Anfahren
			// operate == FALSE || bRun == TRUE
			if (bRunControl)
			{
				uint32_t logosampletime = 0;
				Dose_DSVGetLogoEncoderSampleTime(m_sID, &logosampletime);
				logosampletime = __max(logosampletime, 2U);
				if (t >= m_tSampleTime + logosampletime)
				{
					m_tSampleTime = t;
					m_pAlarm->SetAlarm(t, eAlarmError::ERROR_DOSE_ENCODER, (bRun == FALSE), eAlarmClass::eALARMTYP);
				}
			}
			else
			{
				m_pAlarm->SetAlarm(t, eAlarmError::ERROR_DOSE_ENCODER_SPEED_LOW, (bRun == FALSE), eAlarmClass::eWARNTYP);
			}
		}
		float32_t value = 0.0F;
		Dose_DSVGetActualDriveCommand(m_sID, &value);
		uint32_t lValue = NEXTLONG(value * 100.0f);
		lValue = RANGE(lValue, 0, 100U);

		uint32_t ulMax = 0;
		BOOL bHigh = FALSE;
		BOOL bLow = FALSE;
		Dose_DSVGetAlarmDriveCommandHigh(m_sID, &ulMax);
		if (lValue >= ulMax)
		{
			bHigh = TRUE;
		}
		else
		{
			uint32_t	ulMin = 0;
			Dose_DSVGetAlarmDriveCommandLow(m_sID, &ulMin);
			if (lValue < ulMin)
			{
				bLow = TRUE;
			}
		}
		m_pAlarm->SetAlarm(t, eAlarmError::ERROR_DOSE_DRIVECOMMAND_HIGH, bHigh, eAlarmClass::eWARNTYP);
		m_pAlarm->SetAlarm(t, eAlarmError::ERROR_DOSE_DRIVECOMMAND_LOW, bLow, eAlarmClass::eWARNTYP);
	}
}
//*****************************************************************************************************************
//*****************************************************************************************************************
BOOL CDriveControl::Update(const uint32_t t)
{
	auto result = CAlarmControl::Update(t);
	if (result)
	{
		m_AdsClient.UpdateState();
		if (m_bValidDriveCommand)
		{
			CheckDriveError(t);
			CheckMotorLoad(t);
			CheckDriveErrorPaddle(t);
			Check(t);
			m_pAlarm->CheckWarningLevel(eAlarmError::ERROR_DOSE_ENCODER);
			m_pAlarm->CheckWarningLevel(t, eAlarmError::ERROR_DOSE_DRIVECOMMAND_HIGH);
			m_pAlarm->CheckWarningLevel(t, eAlarmError::ERROR_DOSE_DRIVECOMMAND_LOW);
			m_pAlarm->CheckWarningLevel(t, eAlarmError::ERROR_DOSE_ENCODER_SPEED_LOW);
			m_pAlarm->CheckWarningLevel(t, eAlarmError::ERROR_DOSE_DRIVEERROR);
			m_pAlarm->CheckWarningLevel(t, eAlarmError::ERROR_DOSE_MOTORLOAD);
			m_pAlarm->CheckWarningLevel(t, eAlarmError::ERROR_DOSE_DRIVEERRORPADDLE);
		}
	}
	return result;
}
//*****************************************************************************************************************
//*****************************************************************************************************************
BOOL CDriveControl::Start(uint32_t t)
{
	auto result = CAlarmControl::Start(t);
	if ( result )
	{
		m_tSampleTime = t;
		m_bValidDriveCommand = FALSE;

		SetDriveCommand(0.0F);
		SetDriveRelease(TRUE);
		Dose_DSVSetEncoderState(m_sID, eEncoderState::ENCODERSTATE_OFF);

		m_pAlarm->SetStart(eAlarmError::ERROR_DOSE_ENCODER, t);
		m_pAlarm->SetStart(eAlarmError::ERROR_DOSE_DRIVECOMMAND_HIGH, t);
		m_pAlarm->SetStart(eAlarmError::ERROR_DOSE_DRIVECOMMAND_LOW, t);
		m_pAlarm->SetStart(eAlarmError::ERROR_DOSE_ENCODER_SPEED_LOW, t);
		m_pAlarm->SetStart(eAlarmError::ERROR_DOSE_DRIVEERROR, t);
		m_pAlarm->SetStart(eAlarmError::ERROR_DOSE_MOTORLOAD, t);
		m_pAlarm->SetStart(eAlarmError::ERROR_DOSE_DRIVEERRORPADDLE, t);
	}
	return result;
}
//*****************************************************************************************************************
//*****************************************************************************************************************
BOOL CDriveControl::Stop()
{
	auto result = CAlarmControl::IsStarted();
	if (result)
	{
		InternalStop();
		result = CAlarmControl::Stop();
	}
	return result;
}
//*****************************************************************************************************************
//*****************************************************************************************************************
BOOL CDriveControl::InitExecute()
{
	auto result = CAlarmControl::InitExecute();
	if (result)
	{
		m_AdsClient.Init();
		InternalStop();
	}
	return result;
}
//*****************************************************************************************************************
//*****************************************************************************************************************
BOOL CDriveControl::ExitExecute()
{
	auto result = CAlarmControl::IsInit();
	if (result)
	{
		InternalStop();
		m_AdsClient.Exit();
		result = CAlarmControl::ExitExecute();
	}
	return result;

}

//*****************************************************************************************************************
//*****************************************************************************************************************
void CDriveControl::UpdateDriveCommand(const float32_t value)
{
	if (IsStarted())
	{
		SetDriveCommand(value);
	}
}




