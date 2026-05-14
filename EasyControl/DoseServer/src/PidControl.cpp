//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Implementation of module CPidControl
///
/// @file   PidControl.cpp
///
///
/// @coypright Ing.büro Hafer
///            Branderweg 8A
///            D-91058 Erlangen
///
/// @author    Detlef Hafer
///
//------------------------------------------------------------------------------------
#include < math.h>
#include "PidControl.h"
#include "DoseDataLib/include/DoseData.h"



#define QMAXFILTER          0.05
#define SET_MIN_DOSEPERFORMANCE(a) __max((a), 0.01);
#define ACTUALVALUE_MONITORING_DELAY 10


//*********************************************************************************************
//*********************************************************************************************
void CPidControl::TotalizerSet(const float32_t fMassflow)
{
	if (m_aTotalizer.UpdateMassflow(m_st, fMassflow))
	{
		Dose_DSVSetTotalizer(m_sID, _F32(m_aTotalizer.GetTotalizer()));
	}
}
//*********************************************************************************************
//*********************************************************************************************
inline BOOL CPidControl ::  GetPidPropGainFine (void)
{
	BOOL bChanged			= FALSE;

	Dose_DSVGetChangedPidPropGainFine		(m_sID, &bChanged)	; // [0..1]
	if ( bChanged ) 
	{
		Dose_DSVPopPidPropGainFine			(m_sID, &m_fPidPropGainFine)	; // [0..1]
	}
	return bChanged;
}
//*********************************************************************************************
//*********************************************************************************************
BOOL CPidControl ::  GetPidPropGainGross (void)
{
	BOOL bChanged			= FALSE;

	Dose_DSVGetChangedPidPropGainGross		(m_sID, &bChanged)	; // [0..1]
	if ( bChanged ) 
	{
		Dose_DSVPopPidPropGainGross			(m_sID, &m_fPidPropGainGross)	; // [0..1]
	}
	return bChanged;
}
//*********************************************************************************************
//*********************************************************************************************
BOOL CPidControl ::  GetPidPropGainSwitchGrossFine (void)
{
	BOOL bChanged			= FALSE;

	Dose_DSVGetChangedPidPropGainSwitchGrossFine		(m_sID, &bChanged)	; // [0..1]
	if ( bChanged ) 
	{
		Dose_DSVPopPidPropGainSwitchGrossFine		(m_sID, &m_fPidPropGainSwitchGrossFine)	; // [0..1]
	}
	return bChanged;
}
//*********************************************************************************************
//*********************************************************************************************
BOOL CPidControl ::  GetIntegralVerstaerkung (void)
{
	BOOL bChanged			= FALSE;

	Dose_DSVGetChangedPidIntegralGain		(m_sID, &bChanged)	; // [0..1]
	if ( bChanged ) 
	{
		Dose_DSVPopPidIntegralGain			(m_sID, &m_fPidIntegralGain)	; // [0..1]
	}
	return bChanged;
}
//*********************************************************************************************
//*********************************************************************************************
BOOL CPidControl ::  GetInputFilter (void)
{
	BOOL bChanged			= FALSE;

	Dose_DSVGetChangedMassflowFilter		(m_sID, &bChanged)			; // [0..1]
	if ( bChanged ) 
	{
		Dose_DSVPopMassflowFilter			(m_sID, &m_fInputFilter)	; // [0..1]
	}
	return bChanged;
}
//*******************************************************************************************************
//*******************************************************************************************************
BOOL CPidControl ::  GetSampleInterval (void)
{
	BOOL bChanged			= FALSE;

	Dose_DSVGetChangedPidSampleInterval		(m_sID, &bChanged)	; // [0..1]
	if ( bChanged ) 
	{
		Dose_DSVPopPidSampleInterval		(m_sID, &m_lSampleInterval)	; // [0..1]
	}
	return bChanged;
}
//*********************************************************************************************
//*********************************************************************************************
BOOL CPidControl ::  GetSteepnessMassflow (void)
{
	BOOL bChanged			= FALSE;

	Dose_DSVGetChangedSteepnessMassflow		(m_sID, &bChanged)	; // [0..1]
	if ( bChanged ) 
	{
		Dose_DSVPopSteepnessMassflow			(m_sID, &m_fSteepnessMassflow)	; // [0..1]
	}
	return bChanged;
}
//*********************************************************************************************
//*********************************************************************************************
BOOL CPidControl :: GetMaxSetpoint ( void )
{
	BOOL bChanged = FALSE;

	Dose_DSVGetChangedMaxSetpoint	(m_sID, &bChanged)			; // [kg/h]
	if ( bChanged )
	{
		Dose_DSVPopMaxSetpoint		(m_sID, &m_fMaxSetpoint)	; // [kg/h]
	}
	return bChanged;
}
//*********************************************************************************************
//*********************************************************************************************
BOOL CPidControl :: GetDosePerformance ( void )
{
	BOOL bChanged = FALSE;

	Dose_DSVGetChangedCalibCfg(m_sID, &bChanged); // [kg/h]
	if (bChanged)
	{
		base::calib::CalibType cfg;
		Dose_DSVPopCalibCfg(m_sID, &cfg); // [kg/h]
		m_MeterCtrl.Init(cfg);
	}
	return bChanged;
}
//*********************************************************************************************
//*********************************************************************************************
BOOL CPidControl ::GetDoseSetpoint( void)
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
void CPidControl ::  GetMassflow (void)
{
	m_AdsClient.GetMassflow(m_fMassflow);
	m_fMassflow *= (m_fMaxSetpoint*m_fSteepnessMassflow);
}
//*********************************************************************************************
//*********************************************************************************************
ePidPropGain CPidControl ::  CheckReglerKonstanten ( void )
{
	const float32_t fD		= m_fActualSetpoint - m_fMassflowFilter;
	const float32_t fFehler	= FDIV0(100.0f*fD, m_fActualSetpoint);
	return  ( fFehler > m_fPidPropGainSwitchGrossFine ) ? ePidPropGain::PidPropRough : ePidPropGain::PidPropFine;
}
//--------------------------------------------------------------------------------------------
inline void CPidControl :: 	SetActualDosePerformance (const float32_t fQMaxfaktor)
{
	Dose_DSVSetActualDoseperformance	(m_sID, fQMaxfaktor)	;
}
//*********************************************************************************************
//*********************************************************************************************
void CPidControl ::  SetDriveCommand (const float32_t value)
{
	BOOL bInvers = FALSE;

	Dose_DSVGetPidDriveCommandInv(m_sID, &bInvers);
	const float32_t vf = (bInvers) ? _F32(1.0f - value) : value;
	m_DriveCtrl.UpdateDriveCommand(value);
}
//*********************************************************************************************
//*********************************************************************************************
inline void CPidControl :: SetMassflow (const float32_t fMassflow)
{
	m_AdsClient.SetMassflow(__max(fMassflow, 0.0f));
}
//*********************************************************************************************
//*********************************************************************************************
void CPidControl ::  UpdateMassflowFilter (void)
{
	float32_t fInputFilter = 0.0f;

	Dose_DSVGetMassflowFilter(m_sID, &fInputFilter); // [kg/h]
	fInputFilter /= 100.0f;
	m_fMassflowFilter = (fInputFilter*m_fMassflow) + (1.0f-fInputFilter)*m_fMassflowFilter;
}
//*********************************************************************************************
//*********************************************************************************************
void CPidControl :: AlarmMonitoring ( void)
{
	SetAlarmOutput();
}
//--------------------------------------------------------------------------------
// voluemtrisch 
//--------------------------------------------------------------------------------
void CPidControl :: DosiererSet ( float32_t fNomwert )
{
	m_fLastLeistung		= m_fMassflowFilter = fNomwert;

	SetMassflow (m_fMassflowFilter);
	TotalizerSet(m_fMassflowFilter);

	// DriveCommand initialisieren
	m_fLastDriveCommand = FDIV0(m_fLastLeistung, m_fActualDoseperformance);
	SetDriveCommand (m_fLastDriveCommand);

	m_bDosePerformanceChanged		= FALSE;
	m_bLeistungChanged			= FALSE;
	m_bSteilheitIswertChanged	= FALSE;
}
//--------------------------------------------------------------------------------
// voluemtrisch 
//--------------------------------------------------------------------------------
void CPidControl :: DosiererVolSet ( void )
{
	DosiererSet(m_fActualSetpoint);
}
//--------------------------------------------------------------------------------
// voluemtrisch 
//--------------------------------------------------------------------------------
void CPidControl :: StartVol ( void)
{
	DosiererVolSet ();
    Init3();
}
//--------------------------------------------------------------------------------
// gravimetrisch
//--------------------------------------------------------------------------------
void CPidControl :: StartGrav ( void)
{
	DosiererVolSet ();
    Init4();
}
//--------------------------------------------------------------------------------
// Rampe starten
//--------------------------------------------------------------------------------
void CPidControl :: StartRampe ( void)
{
}
//--------------------------------------------------------------------------------
// PID starten
//--------------------------------------------------------------------------------
void CPidControl :: StartPID ( void)
{
	float32_t fGain			= 0.0f;
	float32_t fPidIntegralGain = 0.0f;

	if ( m_ePidPropSwitch == ePidPropGain::PidPropRough )
	{
		Dose_DSVGetPidPropGainGross		(m_sID, &fGain)				;
	}
	else
	{
		Dose_DSVGetPidPropGainFine		(m_sID, &fGain)				;
	}
	Dose_DSVGetPidIntegralGain		(m_sID, &fPidIntegralGain)				;
	m_PidControl.Open (fGain /100.0f, fPidIntegralGain/100.0f, 0.01f);

	m_bPidPropGainFineChanged	= FALSE;
	m_bPropGrossGainChanged  = FALSE;
	m_bPidPropGainSwitchGrossFineChanged	= FALSE;
	m_bPidIntegralGainChanged	= FALSE;
	m_bInputFilterChanged		= FALSE;
	m_bSampleIntervalChanged		= FALSE;
}
//----------------------------------------------------------------------------
// Ueberprueft, ob gravimetrisch oder volumetrisch
void CPidControl :: CheckVolGrav ( void )
{
	BOOL bVolumetric = FALSE;
	Dose_DSVGetVolumetricState(m_sID, &bVolumetric) ;
	if ( bVolumetric )
	{
		StartVol();
	}
	else
	{
		StartGrav();
	}
}



//----------------------------------------------------------------------------
// Ueberprueft, ob gerampt werden soll
void CPidControl :: CheckRampen ( void)
{
	uint32_t lStartupRamp  = 0L;

	Dose_DSVGetStartupRamp (m_sID, &lStartupRamp)  ;
	if ( lStartupRamp > 0 )
	{
        Init2();
	}
	else
	{
		CheckVolGrav ();
	}
}
//----------------------------------------------------------------------------
// Warten auf StartupDelay
void CPidControl :: Phase0 ( void)
{
	uint32_t lDelay			= 0L;

	Dose_DSVGetStartupDelay(m_sID, &lDelay) ;
	if ( lDelay > 0L)
	{
        Init1();
	}
	else
	{
		CheckRampen ();
	}
}
//----------------------------------------------------------------------------
// Init1 - StartupDelay
void CPidControl :: Init1 ( void)
{
	uint32_t lDelay			= 0L;

	Dose_DSVGetStartupDelay(m_sID, &lDelay) ;
    m_tRegelNext    = m_st+ lDelay;
    m_ControlSteps  = 1;
}
//----------------------------------------------------------------------------
// Warten auf StartupDelay
void CPidControl :: Phase1 ( void)
{
	if ( m_st >= m_tRegelNext)
	{
		CheckRampen ();
	}
}
//----------------------------------------------------------------------------
// Init2
void CPidControl :: Init2 ( void)
{
	uint32_t lStartupRamp = 0L;

	Dose_DSVGetStartupRamp(m_sID, &lStartupRamp) ;
	assert(lStartupRamp > 0L);
    if ( lStartupRamp > 0)
    {
	    float32_t fStep		= m_fActualSetpoint / lStartupRamp;
	    m_tRampenStart  = m_st;
	    DosiererSet(fStep);
        m_tRegelNext = m_st  + lStartupRamp;
        m_ControlSteps = 2;
    }
    else
    {
		CheckVolGrav ();
    }
}




//----------------------------------------------------------------------------
// Hochrampen
void CPidControl :: Phase2 ( void)
{
	TotalizerSet(m_fMassflowFilter);

    BOOL bRampenEnde  = TRUE;

	uint32_t lStartupRamp = 0L;
	Dose_DSVGetStartupRamp (m_sID, &lStartupRamp);
    if ( lStartupRamp > 0)
    {
	    if ( m_st <= m_tRegelNext)
	    {
		    const int32_t tDiff	= __min(int32_t(m_st - m_tRampenStart), lStartupRamp);
		    const float32_t fStep   = m_fActualSetpoint / lStartupRamp * tDiff;
		    DosiererSet(fStep);
            bRampenEnde = FALSE;
	    }
    }
    if ( bRampenEnde )
    {
        CheckVolGrav ();
    }
}




//----------------------------------------------------------------------------
// Init3 volumetrisch
void CPidControl :: Init3 ( void)
{
    uint32_t lT = 0L;

	Dose_DSVGetRefillSwitchDelay	(m_sID, &lT)            ;
	m_tRegelNext = m_st  + lT;
    m_ControlSteps      = 3;
}



//--------------------------------------------------------------------------------
// volumetrisch - Phase 3
//--------------------------------------------------------------------------------
void CPidControl :: Phase3 ( void)
{
	TotalizerSet(m_fMassflowFilter);

	BOOL bVolumetric = FALSE;
	Dose_DSVGetVolumetricState(m_sID, &bVolumetric) ;
	if (  ! bVolumetric )
	{
	    StartGrav();
	}
	else
	{
		if ( m_bLeistungChanged || m_bDosePerformanceChanged )
		{
			StartVol ();		// in 3 bleiben
		}
        else
        {
			if ( m_st >= m_tRegelNext)
			{
				// Filtern
				UpdateMassflowFilter ();

				// Aktuelle (Ist)Leistung setzen
				SetMassflow (m_fMassflowFilter);

			    m_tRegelNext			= m_st + m_lSampleInterval; // lIntervall;
            }
        }
	}
}


//--------------------------------------------------------------------------------
// gravimetrisch - Init 4
//--------------------------------------------------------------------------------
void CPidControl :: Init4 ( void)
{
    uint32_t lT = 0L;

	Dose_DSVGetRefillSwitchDelay	(m_sID, &lT)            ;
    m_tRegelNext          = m_st  + lT;
	m_ControlSteps				  = 4;
}



//--------------------------------------------------------------------------------
// gravimetrisch - Phase 4 - Warten auf 5 Sekunden Ueberbrueckung
//--------------------------------------------------------------------------------
void CPidControl :: Phase4 ( void)
{
	TotalizerSet(m_fMassflowFilter);

	BOOL bVolumetric = FALSE;
	Dose_DSVGetVolumetricState(m_sID, &bVolumetric) ;
	if (  bVolumetric )
	{
		StartVol();
	}
	else
	{
		if ( m_st >= m_tRegelNext)
 		{
            Init5();
		}
	}
}



//--------------------------------------------------------------------------------
// gravimetrisch - Init 5
//--------------------------------------------------------------------------------
void CPidControl :: Init5 ( void)
{
    StartPID();
    m_tRegelNext  = m_st + m_lSampleInterval; // lIntervall;
	m_ControlSteps		  = 5;
}


//--------------------------------------------------------------------------------
// gravimetrisch - Phase 5
//--------------------------------------------------------------------------------
void CPidControl :: Phase5 ( void)
{
	TotalizerSet(m_fMassflowFilter);

	BOOL bVolumetric = FALSE;
	Dose_DSVGetVolumetricState(m_sID, &bVolumetric)	;
	if (  bVolumetric )
	{
		StartVol();
	}
	else
	{
		BOOL bChanged = m_bLeistungChanged || 
							m_bMaxSetpointChanged || 
								m_bDosePerformanceChanged || 
									m_bSteilheitIswertChanged;
		if ( bChanged )
		{
			StartGrav();
		}
		else
		{
			bChanged = 	m_bPidPropGainFineChanged ||
							m_bPropGrossGainChanged  ||
								m_bPidPropGainSwitchGrossFineChanged	||
									m_bPidIntegralGainChanged	||
										m_bInputFilterChanged		|| 
											m_bSampleIntervalChanged;
			if ( bChanged )
			{
				Init5();
			}
			else
			{
				if ( m_st >= m_tRegelNext)
				{
					// Filtern
					UpdateMassflowFilter ();

					// Aktuelle (Ist)Leistung setzen
					SetMassflow (m_fMassflowFilter);

					// Ueberpruefen, ob grob oder Feinjustierung, bei Wechsel umschalten
					const ePidPropGain sU = CheckReglerKonstanten ();
					if ( sU != m_ePidPropSwitch)
					{
						m_ePidPropSwitch = sU;
						Init5();
					}
					else
					{
						// Fehler berechnen Sollwert zum gefilterten Massflow
						const float32_t fFehler = m_fActualSetpoint - m_fMassflowFilter;

                        if ( (fabs(fFehler) < 0.5f*m_fActualSetpoint) && (m_fLastDriveCommand > 0.0f) )
                        {
                            const float64_t dAlt = m_fActualDoseperformance;
                            const float64_t dNeu = SET_MIN_DOSEPERFORMANCE(FDIV0(m_fMassflowFilter, m_fLastDriveCommand));
                            m_fActualDoseperformance  = (float32_t) (QMAXFILTER*dNeu +  (1.0-QMAXFILTER)*dAlt);
                            SetActualDosePerformance(m_fActualDoseperformance);
                        }

						float32_t fD = m_PidControl.Start(m_st, fFehler);

						// Neuer Sollwert zum Einstellen
						m_fLastLeistung	= __max(m_fLastLeistung + fD, 0.0f);

						// DriveCommand initialisieren
						m_fLastDriveCommand = FDIV0(m_fLastLeistung, m_fActualDoseperformance);
						SetDriveCommand (m_fLastDriveCommand);

						Init6();
					}
				}
			}
		}
	}
}



//--------------------------------------------------------------------------------
// gravimetrisch - Init 6
//--------------------------------------------------------------------------------
void CPidControl :: Init6 ( void)
{
    m_tRegelNext		  = m_st + m_lSampleInterval; // lIntervall;
	m_ControlSteps				  = 6;
}


//--------------------------------------------------------------------------------
// gravimetrisch
//--------------------------------------------------------------------------------
void CPidControl :: Phase6 ( void)
{
	TotalizerSet(m_fMassflowFilter);

	BOOL bVolumetric = FALSE;
	Dose_DSVGetVolumetricState(m_sID, &bVolumetric)	;
	if (  bVolumetric )
	{
		StartVol();
	}
	else
	{
		BOOL bChanged = m_bLeistungChanged || 
							m_bMaxSetpointChanged || 
								m_bDosePerformanceChanged || 
									m_bSteilheitIswertChanged;
		if ( bChanged )
		{
			StartGrav();
		}
		else
		{
			bChanged = 	m_bPidPropGainFineChanged ||
							m_bPropGrossGainChanged  ||
								m_bPidPropGainSwitchGrossFineChanged	||
									m_bPidIntegralGainChanged	||
										m_bInputFilterChanged		|| 
											m_bSampleIntervalChanged;
			if ( bChanged )
			{
                Init5();
			}
			else
			{
				if ( m_st >= m_tRegelNext)
				{
					// Filtern
					UpdateMassflowFilter ();

					// Aktuelle (Ist)Leistung setzen
					SetMassflow (m_fMassflowFilter);

					// Ueberpruefen, ob grob oder Feinjustierung, bei Wechsel umschalten
					const ePidPropGain sU = CheckReglerKonstanten ();
					if ( sU != m_ePidPropSwitch)
					{
						m_ePidPropSwitch = sU;
                        Init5();
					}
					else
					{
						// Fehler berechnen Sollwert zum gefilterten Massflow
						const float32_t fFehler = m_fActualSetpoint - m_fMassflowFilter;
                        if ( (fabs(fFehler) < 0.3f*m_fActualSetpoint) && (m_fLastDriveCommand > 0.0f) )
                        {
                            const float64_t dAlt = m_fActualDoseperformance;
                            const float64_t dNeu = SET_MIN_DOSEPERFORMANCE(FDIV0(m_fMassflowFilter, m_fLastDriveCommand));
                            m_fActualDoseperformance  = (float32_t) (QMAXFILTER*dNeu +  (1.0-QMAXFILTER)*dAlt);
                            SetActualDosePerformance(m_fActualDoseperformance);
                        }

						float32_t fD = m_PidControl.Update(m_st, fFehler);
						// Neuer Sollwert zum Einstellen
						m_fLastLeistung	 = __max(m_fLastLeistung + fD, 0.0f);

						// DriveCommand initialisieren
						m_fLastDriveCommand = FDIV0(m_fLastLeistung, m_fActualDoseperformance);
						SetDriveCommand (m_fLastDriveCommand);
						m_tRegelNext = m_st + m_lSampleInterval; // lIntervall;
					}
				}
			}
		}
	}
}






//--------------------------------------------------------------------------------
// Phasen
//--------------------------------------------------------------------------------
void	CPidControl :: Control()
{
	switch (m_ControlSteps)
	{
		default:
		case 0:
			{
				Phase0 ();
			}
			break;

		case 1:		// volumetrisch
			{
				Phase1 ();

			}
			break;

		case 2:   // gravimetrisch
			{
				Phase2 ();
			}
			break;

		case 3:   // gravimetrisch
			{
				Phase3 ();
			}
			break;

		case 4:
			{
				Phase4 ();
			}
			break;

		case 5:
			{
				Phase5 ();
			}
			break;

		case 6:
			{
				Phase6 ();
			}
			break;
	}
	m_MassflowAlarmCtrl.Update(m_st);
	m_DriveCtrl.Update(m_st);
}



//-----------------------------------------------------------------------------------
BOOL CPidControl :: InitExecute ( void)
{
	auto result = CBaseControl :: InitExecute ();
	if (result)
	{
		m_tRegelNext = 0;
		m_bStartupDelay = FALSE;
		m_tRegelNext = 0L;
		m_bLeistungChanged = FALSE;
		m_bMaxSetpointChanged = FALSE;
		m_bSteilheitIswertChanged = FALSE;
		m_bPidPropGainFineChanged = FALSE;
		m_bPropGrossGainChanged = FALSE;
		m_bPidPropGainSwitchGrossFineChanged = FALSE;
		m_bPidIntegralGainChanged = FALSE;
		m_bInputFilterChanged = FALSE;

		m_fMassflowFilter = 0.0f;
		m_fSteepnessMassflow = 0.0f;
		m_fMassflow = 0.0f;
		m_fActualSetpoint = 0.0f;
		m_fPidPropGainFine = 0.0f;
		m_fPidPropGainGross = 0.0f;
		m_fPidPropGainSwitchGrossFine = 0.0f;
		m_fPidIntegralGain = 0.0f;
		m_fInputFilter = 0.0f;

		m_AdsClient.Init();
		m_ePidPropSwitch = ePidPropGain::PidPropRough;
		m_DriveCtrl.InitExecute();
		m_MassflowAlarmCtrl.InitExecute();
		base::calib::CalibType cfg;
		Dose_DSVPopCalibCfg(m_sID, &cfg); // [kg/h]
		m_MeterCtrl.Init(cfg);
		SetMassflow(0.0f);
	}
    return result;
}


//-----------------------------------------------------------------------------------
BOOL CPidControl :: ExitExecute ( void)
{
	auto result = CBaseControl::IsInit();
	if (result)
	{
		Stop();
		m_PidControl.Close();
		m_DriveCtrl.ExitExecute();
		m_MassflowAlarmCtrl.ExitExecute();
		m_MeterCtrl.Close();
		result = CBaseControl::ExitExecute();
	}
	return result;
}




//-----------------------------------------------------------------------------------
void CPidControl :: InitStart ( void)
{
    float32_t fVerbrauch = 0.0f;

	m_bStartupDelay			= FALSE;
	m_tRegelNext					= 0L;
	m_fLastLeistung					= 0.0f;
	m_bLeistungChanged				= FALSE;
	m_bMaxSetpointChanged			= FALSE;
	m_bSteilheitIswertChanged		= FALSE;
	m_bPidPropGainFineChanged				= FALSE;
	m_bPropGrossGainChanged					= FALSE;
	m_bPidPropGainSwitchGrossFineChanged	= FALSE;
	m_bPidIntegralGainChanged				= FALSE;
	m_bInputFilterChanged					= FALSE;
	m_ControlSteps							= 0;

	m_fMassflowFilter				= 0.0f;
	m_fMassflow						= 0.0f;

	m_ePidPropSwitch					= ePidPropGain::PidPropRough;
	m_tRampenStart					= 0L;

	Dose_DSVGetPidSampleInterval		(m_sID, &m_lSampleInterval)		; // [0..1]
	Dose_DSVGetPidPropGainGross			(m_sID, &m_fPidPropGainGross)	; // [0..1]
    Dose_DSVGetPidPropGainFine			(m_sID, &m_fPidPropGainFine)	; // [0..1]
	Dose_DSVGetSteepnessMassflow		(m_sID, &m_fSteepnessMassflow)		; // [0..1]
	Dose_DSVGetMaxSetpoint				(m_sID, &m_fMaxSetpoint)			; // [kg/h]
	Dose_DSVGetMassflowFilter			(m_sID, &m_fInputFilter)			; // [0..1]
	Dose_DSVGetPidIntegralGain	(m_sID, &m_fPidIntegralGain)	; // [0..1]
	Dose_DSVGetPidPropGainSwitchGrossFine(m_sID, &m_fPidPropGainSwitchGrossFine)	; // [0..1]
    Dose_DSVGetActualSetpoint		     (m_sID, &m_fActualSetpoint)               ;
	Dose_DSVGetActualDoseperformance	(m_sID, &m_fActualDoseperformance)			; // [kg/h]

	// Verbrauch
	Dose_DSVGetTotalizer   (m_sID, &fVerbrauch) ;
	m_aTotalizer.Open(m_st, fVerbrauch, m_fActualSetpoint);
}


//-----------------------------------------------------------------------------------
BOOL CPidControl :: Stop ( void)
{
	auto result = CBaseControl :: IsStarted ();
    if (result)
    {
		m_PidControl.Close();
		m_DriveCtrl.Stop();
		m_MassflowAlarmCtrl.Stop();
		m_MeterCtrl.Close();
		SetMassflow(0.0f);
        m_aTotalizer.Close();
		m_ControlSteps = 0;
		result = CBaseControl::Stop();
    }
	return result;
}


//-----------------------------------------------------------------------------------
BOOL CPidControl :: Start(const uint32_t t)
{
	auto result = CBaseControl :: Start (t);
    if ( result )
    {
        InitStart();
		m_DriveCtrl.Start(m_st);
		m_MassflowAlarmCtrl.Start(m_st);
        SetMassflow(0.0f);
    }
	return result;
}


//------------------------------------------------------------------------------------------------
BOOL CPidControl :: Execute ( void)
{
	// aktuelle Zeit holen
	auto result = CBaseControl :: Execute();
	if (result)
	{
		// aktuellen Sollwert holen
		BOOL bChanged = GetDoseSetpoint();
		if (bChanged)
		{
			m_bLeistungChanged = TRUE;
		}

		// aktuellen Maxdosierfaktor holen
		bChanged = GetDosePerformance();
		if (bChanged)
		{
			m_bDosePerformanceChanged = TRUE;
		}

		// aktuellen Maxsollwert (m_fMaxSetpoint) holen
		bChanged = GetMaxSetpoint();
		if (bChanged)
		{
			m_bMaxSetpointChanged = TRUE;
		}

		// Massflowsteilheit holen
		bChanged = GetSteepnessMassflow();
		if (bChanged)
		{
			m_bSteilheitIswertChanged = TRUE;
		}
		// Proportional Feinverstaerkung
		bChanged = GetPidPropGainFine();
		if (bChanged)
		{
			if (m_ePidPropSwitch == ePidPropGain::PidPropFine)
			{
				m_bPidPropGainFineChanged = TRUE;
			}
		}

		// Proportional Grobverstaerkung
		bChanged = GetPidPropGainGross();
		if (bChanged)
		{
			if (m_ePidPropSwitch == ePidPropGain::PidPropRough)
			{
				m_bPropGrossGainChanged = TRUE;
			}
		}

		// Proportional Grob/Feinumschaltung
		bChanged = GetPidPropGainSwitchGrossFine();
		if (bChanged)
		{
			m_bPidPropGainSwitchGrossFineChanged = TRUE;
		}

		// IntegralVerstaerkung
		bChanged = GetIntegralVerstaerkung();
		if (bChanged)
		{
			m_bPidIntegralGainChanged = TRUE;
		}

		// InputFilter
		bChanged = GetInputFilter();
		if (bChanged)
		{
			m_bInputFilterChanged = TRUE;
		}

		// SampleInterval
		bChanged = GetSampleInterval();
		if (bChanged)
		{
			m_bSampleIntervalChanged = TRUE;
		}

		// aktuelles Gewicht (m_fMassflow) holen
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
				if ( bRelease)
				{
					Start();
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


