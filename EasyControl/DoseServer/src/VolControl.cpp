//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Implementation of module VolControl.cpp
///
/// @file   VolControl.cpp
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
#include "VolControl.h"
#include "DoseDataLib/include/DoseData.h"

using namespace base;

#define SET_MIN_DOSEPERFORMANCE(a) __max((a), 0.01f);


//*********************************************************************************************
//*********************************************************************************************
CVolControl::CVolControl(int32_t s) : CBaseControl{ s }
	, m_AdsClient{ s }
	, m_RefillCtrl(s)
	, m_MeterCtrl(s)
	, m_DriveCtrl(s)
	, m_CalibTask(s)
	, m_EmptyFeederTask(s)
	, m_fActualSetpoint(0.0f)
	, m_fMassflow(0.0f)
	, m_tRegelNext(0L)
	, m_ControlSteps{ 0 }
	, m_bSetpointChanged(FALSE)
	, m_bDosePerformanceChanged(FALSE)
{
	m_RefillCtrl.registerAlarmManager(this->m_aAlarm);
	m_DriveCtrl.registerAlarmManager(this->m_aAlarm);
	m_CalibTask.registerAlarmManager(this->m_aAlarm);
	m_CalibTask.registerDriveCtrl(m_DriveCtrl);
	m_EmptyFeederTask.registerAlarmManager(this->m_aAlarm);
	m_EmptyFeederTask.registerDriveCtrl(m_DriveCtrl);
}
//*********************************************************************************************
//*********************************************************************************************
inline void CVolControl::SetActualDosePerformance(const float32_t fQMaxfaktor)
{
	Dose_DSVSetActualDoseperformance(m_sID, fQMaxfaktor);
}
//*********************************************************************************************
//*********************************************************************************************
inline float32_t CVolControl::CalcDriveCommand(const float32_t fNomwert) const
{
	return m_MeterCtrl.CalcDriveCommand(fNomwert);
}
//*********************************************************************************************
//*********************************************************************************************
void CVolControl::TotalizerSet(const float32_t fMassflow)
{
	if (m_aTotalizer.UpdateMassflow(m_st, fMassflow))
	{
		Dose_DSVSetTotalizer(m_sID, _F32(m_aTotalizer.GetTotalizer()));
	}
}
//*********************************************************************************************
//*********************************************************************************************
void CVolControl::DosefactorSet(const float32_t fNomwert)
{
	if (m_bDosePerformanceChanged)
	{
		// Berechne neuen Startdosierfaktor
		base::calib::CalibType cfg;
		Dose_DSVPopCalibCfg(m_sID, &cfg); // [kg/h]
		m_MeterCtrl.Init(cfg);

		auto fActualDoseperformance = m_MeterCtrl.CalcDosePerformance(fNomwert);
		fActualDoseperformance = SET_MIN_DOSEPERFORMANCE(fActualDoseperformance);
		SetActualDosePerformance(fActualDoseperformance);
		m_bDosePerformanceChanged = FALSE;
		m_bSetpointChanged = TRUE;
	}
}
//*********************************************************************************************
//*********************************************************************************************
BOOL CVolControl::GetDosePerformance(void)
{
	BOOL bChanged = FALSE;

	Dose_DSVGetChangedCalibCfg(m_sID, &bChanged); // [kg/h]
	if (bChanged)
	{
		base::calib::CalibType cfg;
		Dose_DSVPopCalibCfg(m_sID, &cfg); // [kg/h]
		if (cfg.count > 0)
		{
			auto fActualDoseperformance = cfg.fDosePerformance[0];
			fActualDoseperformance = SET_MIN_DOSEPERFORMANCE(fActualDoseperformance);
			SetActualDosePerformance(fActualDoseperformance);
		}
	}
	return bChanged;
}
//*********************************************************************************************
//*********************************************************************************************
inline void CVolControl ::  GetMassflow (void)
{
	m_fMassflow = m_fActualSetpoint;
}
//*********************************************************************************************
//*********************************************************************************************
BOOL CVolControl ::GetDoseSetpoint( void)
{
    auto bChanged = CBaseControl ::GetDoseSetpoint();
    if ( bChanged )
    {
		Dose_DSVGetActualSetpoint(m_sID, &m_fActualSetpoint);
    }
    return bChanged;
}
//*********************************************************************************************
//*********************************************************************************************
inline void CVolControl ::  SetDriveCommand (const float32_t drive)
{
	float32_t drivevalue = drive;
	drivevalue = RANGE(drivevalue, 0.0F, 1.0F);
	m_DriveCtrl.UpdateDriveCommand(drivevalue);
}
//*********************************************************************************************
//*********************************************************************************************
BOOL CVolControl :: GetVolumetric ( void)
{
	BOOL	bVolumetric			 = FALSE;

	Dose_DSVGetVolumetricState	    (m_sID, &bVolumetric)	;
    if ( ! bVolumetric )
    {
		Dose_DSVSetVolumetricState		(m_sID, TRUE) ;
	}
	return TRUE;
}
//*********************************************************************************************
//*********************************************************************************************
BOOL CVolControl :: InitExecute ( void)
{
	auto result = CBaseControl :: InitExecute ();
	if (result)
	{
		m_bSetpointChanged = FALSE;
		m_bDosePerformanceChanged = FALSE;
		m_fActualSetpoint = 0.0f;
		m_fMassflow = 0.0f;
		m_tRegelNext = 0;

		m_AdsClient.Init();
		m_EmptyFeederTask.InitExecute();

		SetMassflow(0.0f);
		m_DriveCtrl.InitExecute();
		m_RefillCtrl.InitExecute();
		m_RefillCtrl.Start(m_st);
	}
	return result;
}
//*********************************************************************************************
//*********************************************************************************************
BOOL CVolControl :: ExitExecute ( void)
{
	BOOL result = CBaseControl::IsInit();
	if (result)
	{
		Stop();
		m_RefillCtrl.ExitExecute();
		m_DriveCtrl.ExitExecute();
		m_EmptyFeederTask.ExitExecute();
		m_AdsClient.Exit();
		result = CBaseControl::ExitExecute();;
	}
	return result;
}
//*********************************************************************************************
//*********************************************************************************************
BOOL CVolControl :: Stop ( void)
{
    auto result = CBaseControl ::IsStarted();
    if (result)
    {
		m_bSetpointChanged = TRUE;
		m_fActualSetpoint = 0.0F;

		m_fMassflow = 0.0F;
	    SetMassflow(m_fMassflow);
		//DoseDriveCommandSet(m_fActualSetpoint);
		m_DriveCtrl.Stop();
        m_aTotalizer.Close();
		return CBaseControl::Stop();
    }
	return result;
}
//*********************************************************************************************
//*********************************************************************************************
BOOL CVolControl :: Start(const uint32_t t)
{
    auto result = CBaseControl :: Start(t);
    if (result)
    {
		m_fMassflow = 0.0f;

		m_DriveCtrl.Start(m_st);

		Dose_DSVGetActualSetpoint(m_sID, &m_fActualSetpoint);

		// Verbrauch
		float32_t fVerbrauch = 0.0f;
		Dose_DSVGetTotalizer(m_sID, &fVerbrauch);
		m_aTotalizer.Open(m_st, fVerbrauch, m_fActualSetpoint);

		SetMassflow(m_fMassflow);
		m_bDosePerformanceChanged = TRUE;
		DosefactorSet(m_fActualSetpoint);
		DoseDriveCommandSet(m_fActualSetpoint);

		m_ControlSteps = 0;
		m_tRegelNext = 0L;
	}
	return result;
}
//*********************************************************************************************
//*********************************************************************************************
inline void CVolControl :: 	SetMassflow (const float32_t fMassflow)
{
	m_AdsClient.SetMassflow(__max(fMassflow, 0.0f));
}
//*********************************************************************************************
//*********************************************************************************************
void CVolControl ::DoseDriveCommandSet( const float32_t fNomwert )
{
	if (m_bSetpointChanged)
	{
		// Verbrauch initialisieren
		TotalizerSet(fNomwert);

		// DriveCommand initialisieren
		const float32_t fDriveCommand = CalcDriveCommand(fNomwert);
		SetDriveCommand(fDriveCommand);

		m_bDosePerformanceChanged = FALSE;
		m_bSetpointChanged = FALSE;
	}
}
//*********************************************************************************************
//*********************************************************************************************
void	CVolControl :: Phase0 (void)
{
	SetMassflow (m_fMassflow);
	assert(m_fActualSetpoint > 0.0f);
	DoseDriveCommandSet(m_fActualSetpoint);
	m_tRegelNext = m_st + 5;
	m_ControlSteps = 1;
}
//*********************************************************************************************
//*********************************************************************************************
void	CVolControl :: Phase1 ()
{
	SetMassflow (m_fMassflow);
	TotalizerSet(m_fMassflow);
	if ( m_st >= m_tRegelNext)
	{
		m_ControlSteps = 2;
	}
}
//*********************************************************************************************
//*********************************************************************************************
void	CVolControl :: Phase2 ()
{
	SetMassflow (m_fMassflow);
	TotalizerSet(m_fMassflow);

	if (m_bDosePerformanceChanged)
	{
		DosefactorSet(m_fActualSetpoint);
	}
	if ( m_bSetpointChanged )
	{
		DoseDriveCommandSet(m_fActualSetpoint);
	}
}
//*********************************************************************************************
//*********************************************************************************************
void	CVolControl :: Control()
{
	m_RefillCtrl.CheckRefill();

	switch (m_ControlSteps )
	{
		default:
		case 0:
				{
					Phase0 ();
				}
				break;
		case 1:
				{
					Phase1 ();
				}
				break;
		case 2:
				{
					Phase2 ();
				}
				break;
	}
	m_DriveCtrl.Update(m_st);
	CheckExtAlarm();
}
//*********************************************************************************************
//*********************************************************************************************
BOOL CVolControl :: Execute ( void)
{
	BOOL bChanged = FALSE;

	auto result = CBaseControl::Execute();
	if (result)
	{
		// aktuellen Sollwert holen
		bChanged = GetDoseSetpoint();
		if ( bChanged )
		{
			m_bSetpointChanged = TRUE;
		}
		// aktuellen Maxdosierfaktor holen
		bChanged = GetDosePerformance();
		if (bChanged)
		{
			m_bDosePerformanceChanged = TRUE;
		}

		/// Massflow holen
		GetMassflow ();

		// Refill immer updaten
		m_RefillCtrl.Update(m_st);

		// Freigabe
		BOOL bRelease = FALSE;
		Dose_DSVGetRelease(m_sID, &bRelease);

		//--------------------------------------
		switch (GetOperatingMode())
		{
			case eOperatingMode::RUNNING:
			{
				if (!bRelease)
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

			case  eOperatingMode::EMPTYFEEDER:
			{
				auto bEmptyFeeder = GetEmptyFeeder();
				if (bEmptyFeeder)
				{
					m_EmptyFeederTask.Update(m_st);
				}
				else
				{
					m_EmptyFeederTask.Stop();
					SetOperatingMode(eOperatingMode::IDLE);
				}
			}
			break;


			case eOperatingMode::CALIBRATING:
			{
				BOOL bCalibration = GetCalibration();
				if (bCalibration)
				{
					m_CalibTask.Update(m_st);
				}
				else
				{
					m_CalibTask.Stop();
					m_CalibTask.ExitExecute();
					SetOperatingMode(eOperatingMode::IDLE);
				}
			}
			break;

			case eOperatingMode::IDLE:
			default:
			{
				BOOL bCalibration = GetCalibration();
				if (bCalibration)
				{
					m_CalibTask.InitExecute();
					m_CalibTask.Start(m_st);
					SetOperatingMode(eOperatingMode::CALIBRATING);
				}
				else
				{
					BOOL bEmptyFeeder = GetEmptyFeeder();
					if (bEmptyFeeder)
					{
						m_EmptyFeederTask.Start(m_st);
						SetOperatingMode(eOperatingMode::EMPTYFEEDER);
					}
					else
					{
						//--------------------------------------
						// sonst Freigabe erfolgt ?
						if ( ! bRelease)
						{
							Stop();
						}
						else
						{
							Start();
						}
					}
				}
			}
			break;
		}
		SetAlarmOutput();
	}
	return TRUE;
}


