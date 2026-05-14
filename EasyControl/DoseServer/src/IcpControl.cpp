//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Implementation of module IcpControl
///
/// @file   IcpControl.cpp
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
#include "IcpControl.h"
#include "DoseDataLib/include/DoseData.h"
#include "BASE/Base.def"



using namespace base;


//*********************************************************************************************
//*********************************************************************************************
CIcpControl :: CIcpControl(int32_t s) : CBaseControl{ s }
	, m_AdsClient{ s }
	, m_DriveCtrl{ s }
	, m_MassflowAlarmCtrl{ s }
	, m_fActualSetpoint{ 0.0F }
	, m_fCurrentMassflow{ 0.0F }
	, m_fNominalMassflow{ 0.0F }
	, m_fMaxSetpoint{ 0.0F }
	, m_fSteepnessMassflow{ 0.0F }
	, m_bMaxSetpointChanged(FALSE)
	, m_bSteepnessMassflowChanged(FALSE)
	, m_bLeistungChanged(FALSE)
	, m_tRegelNext(0L)
	, m_lDrehgeberIntervall(0L)
	, m_ControlSteps{ 0 }
{
	m_DriveCtrl.registerAlarmManager(this->m_aAlarm);
	m_MassflowAlarmCtrl.registerAlarmManager(this->m_aAlarm);
}
//*********************************************************************************************
//*********************************************************************************************
void CIcpControl::TotalizerSet(const float32_t fMassflow)
{
	if (m_aTotalizer.UpdateMassflow(m_st, fMassflow))
	{
		Dose_DSVSetTotalizer(m_sID, _F32(m_aTotalizer.GetTotalizer()));
	}
}
//*********************************************************************************************
//*********************************************************************************************
BOOL CIcpControl :: GetMaxSetpoint ( void )
{
	BOOL bChanged = FALSE;

	Dose_DSVGetChangedMaxSetpoint(m_sID, &bChanged)			; // [kg/h]
	if ( bChanged )
	{
		Dose_DSVPopMaxSetpoint(m_sID, &m_fMaxSetpoint)	; // [kg/h]
	}
	return bChanged;
}
//*********************************************************************************************
//*********************************************************************************************
void CIcpControl ::  GetMassflow (void)
{
	float32_t fTemp = 0.0F;
	(void)m_AdsClient.GetMassflow(fTemp);
	m_MassflowBuffer.Add(fTemp);
	fTemp = m_MassflowBuffer.GetMean();
	m_fCurrentMassflow = fTemp * m_fMaxSetpoint*m_fSteepnessMassflow;
}
//*********************************************************************************************
//*********************************************************************************************
BOOL CIcpControl ::  GetSteepnessMassflow (void)
{
	BOOL bChanged			= FALSE;

	Dose_DSVGetChangedSteepnessMassflow	(m_sID, &bChanged); // [0..1]
	if ( bChanged ) 
	{
		Dose_DSVPopSteepnessMassflow (m_sID, &m_fSteepnessMassflow); // [0..1]
	}
	return bChanged;
}
//*********************************************************************************************
//*********************************************************************************************
BOOL CIcpControl :: GetDoseSetpoint ( void)
{
    auto bChanged = CBaseControl ::GetDoseSetpoint();
	if (bChanged)
	{
		Dose_DSVGetActualSetpoint(m_sID, &m_fActualSetpoint);
	}
    return bChanged;
}
//*********************************************************************************************
//*********************************************************************************************
inline void CIcpControl ::  SetDriveCommand (const float32_t f)
{
	m_DriveCtrl.UpdateDriveCommand(f);
}
//*********************************************************************************************
//*********************************************************************************************
void CIcpControl :: AlarmMonitoring ( void)
{
	m_AdsClient.RefillMonitoring();
	SetAlarmOutput();
}
//*********************************************************************************************
//*********************************************************************************************
BOOL CIcpControl :: InitExecute ( void)
{
	auto result = CBaseControl :: InitExecute ();
	if (result)
	{
		m_bLeistungChanged = FALSE;
		m_bMaxSetpointChanged = FALSE;
		m_bSteepnessMassflowChanged = FALSE;
		m_fNominalMassflow = 0.0f;
		m_fCurrentMassflow = 0.0f;
		m_fMaxSetpoint = 0.0f;
		m_fSteepnessMassflow = 0.0f;
		m_tRegelNext = 0;

		m_AdsClient.Init();
		m_MassflowBuffer.Init();
		m_DriveCtrl.InitExecute();
		m_MassflowAlarmCtrl.InitExecute();
		SetMassflow(0.0f);
	}
	return result;
}
//*********************************************************************************************
//*********************************************************************************************
BOOL CIcpControl :: ExitExecute ( void)
{
	auto result = CBaseControl::IsInit();
	if (result)
	{
		Stop();
		m_MassflowBuffer.Init();
		m_DriveCtrl.ExitExecute();
		m_MassflowAlarmCtrl.ExitExecute();
		m_AdsClient.Exit();
		result = CBaseControl::ExitExecute();
	}
	return result;
}
//*********************************************************************************************
//*********************************************************************************************
BOOL CIcpControl :: InitStart ( void)
{
    float32_t fVerbrauch = 0.0f;

	m_tRegelNext				 = 0L;
	m_bLeistungChanged           = FALSE;
	m_bMaxSetpointChanged        = FALSE;
    m_bSteepnessMassflowChanged  = FALSE;
	m_fCurrentMassflow           = 0.0f;
	m_fNominalMassflow			= 0.0f;
	m_ControlSteps				= 0;

	Dose_DSVGetSteepnessMassflow	(m_sID, &m_fSteepnessMassflow); // [0..1]
    Dose_DSVGetActualSetpoint		(m_sID, &m_fActualSetpoint);
	Dose_DSVGetMaxSetpoint			(m_sID, &m_fMaxSetpoint)			; // [kg/h]

    // Verbrauch
	Dose_DSVGetTotalizer   (m_sID, &fVerbrauch) ;
	m_aTotalizer.Open(m_st, fVerbrauch, m_fActualSetpoint);
	return TRUE;
}
//*********************************************************************************************
//*********************************************************************************************
BOOL CIcpControl :: Stop ( void)
{
    auto result = CBaseControl :: IsStarted();
    if (result)
    {
	    SetMassflow(0.0f);
		m_MassflowBuffer.Init();
		m_DriveCtrl.Stop();
		m_MassflowAlarmCtrl.Stop();
		m_aTotalizer.Close();
		m_ControlSteps = 0;
		result = CBaseControl::Stop();
    }
	return result;
}
//*********************************************************************************************
//*********************************************************************************************
BOOL CIcpControl :: Start(const uint32_t t)
{
    auto result = CBaseControl :: Start(t);
    if (result)
    {
	    InitStart();
		SetMassflow(0.0f);
		m_MassflowBuffer.Init();
		m_aAlarm.SetStart(eAlarmError::ERROR_DOSE_EXTALARM, m_st);
		m_DriveCtrl.Start(m_st);
		m_MassflowAlarmCtrl.Start(m_st);
    }
	return result;
}
//*********************************************************************************************
//*********************************************************************************************
inline void CIcpControl :: 	SetMassflow (const float32_t fMassflow)
{
	m_AdsClient.SetMassflow(fMassflow);
}
//*********************************************************************************************
//*********************************************************************************************
//--------------------------------------------------------------------------------
// voluemtrisch 
//--------------------------------------------------------------------------------
void CIcpControl :: DosiererSet ( const float32_t fNomwert )
{
	// Verbrauch initialisieren
	TotalizerSet(fNomwert);

	// DriveCommand initialisieren
	const float32_t fDriveCommand = FDIV0(fNomwert, m_fMaxSetpoint);
	SetDriveCommand(fDriveCommand);

	m_bMaxSetpointChanged		= FALSE;
	m_bLeistungChanged			= FALSE;
}
//*********************************************************************************************
//*********************************************************************************************
//--------------------------------------------------------------------------------
// Phase0
//--------------------------------------------------------------------------------
void	CIcpControl :: Phase0 (void)
{
	SetMassflow (m_fCurrentMassflow);
	assert(m_fActualSetpoint > 0.0f);
	DosiererSet (m_fActualSetpoint);
	m_tRegelNext = m_st + 5;
	m_ControlSteps = 1;
}
//*********************************************************************************************
//*********************************************************************************************
//--------------------------------------------------------------------------------
// Phase 1
//--------------------------------------------------------------------------------
void	CIcpControl :: Phase1 ()
{
	SetMassflow (m_fCurrentMassflow);
	TotalizerSet(m_fCurrentMassflow);

	if ( m_st >= m_tRegelNext)
	{
		m_ControlSteps = 2;
	}
}
//*********************************************************************************************
//*********************************************************************************************
//--------------------------------------------------------------------------------
// Phase 2
//--------------------------------------------------------------------------------
void	CIcpControl :: Phase2 ()
{
	SetMassflow (m_fCurrentMassflow);
	TotalizerSet(m_fCurrentMassflow);

	if ( m_bLeistungChanged )
	{
		DosiererSet (m_fActualSetpoint);
	}
}
//*********************************************************************************************
//*********************************************************************************************
//--------------------------------------------------------------------------------
// Control
//--------------------------------------------------------------------------------
void	CIcpControl :: Control()
{
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
	m_MassflowAlarmCtrl.Update(m_st);
	m_DriveCtrl.Update(m_st);
	CheckExtAlarm();
}
//*********************************************************************************************
//*********************************************************************************************
//--------------------------------------------------------------------------------
// Execute
//--------------------------------------------------------------------------------
BOOL CIcpControl :: Execute ( void)
{
	// aktuelle Zeit holen
	auto result = CBaseControl :: Execute();
	if (result)
	{
		BOOL bChanged = FALSE;

		// aktuellen Sollwert holen
		bChanged = GetDoseSetpoint();
		if (bChanged)
		{
			m_bLeistungChanged = TRUE;
		}

		// aktuellen Maxsollwert (m_fMaxSetpoint) holen
		bChanged = GetMaxSetpoint();
		if (bChanged)
		{
			m_bMaxSetpointChanged = TRUE;
		}

		bChanged = GetSteepnessMassflow();
		if (bChanged)
		{
			m_bSteepnessMassflowChanged = TRUE;
		}

		// Massflow holen
		GetMassflow();

		// Freigabe
		BOOL bRelease = FALSE;
		Dose_DSVGetRelease(m_sID, &bRelease);

		//--------------------------------------
		switch (GetOperatingMode())
		{
			default:
			case eOperatingMode::IDLE:
			{
				if ( bRelease )
				{
					Start(m_st);
				}
			}
			break;

			case eOperatingMode::RUNNING:
			{
				if ( ! bRelease)
				{
					Stop();
				}
				else
				{
					Control();
				}
			}
			break;
		}
		AlarmMonitoring();
	}
	return result;
}


