//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
///
/// @brief  Implementation of module WbfControl.cpp
///
/// @file   WbfControl.cpp
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
#include "WbfControl.h"
#include "DoseDataLib/include/DoseData.h"

using namespace base;

#define REGELFAKTOR   1
#define STARTFAKTOR   (10*REGELFAKTOR)
#define STARTDELAY    (5 )
#define DREHGEBERSTARTDELAY 5
#define MAXDRIVECOMMANDZEIT 60
#define ACTUALVALUE_MONITORING_DELAY 10


static const float32_t fMIN2H    = 60.0f;
static const float32_t fMM2M	 = 1E-03f;		// 0.001 m
  

CWbfControl::CWbfControl(int32_t s, loadcell::ILCModuleInterface& rlc) : CBaseControl{ s }
	, m_LoadCell(rlc)
	, m_AdsClient ( s )
	, m_DriveCtrl(s)
	, m_WeightCtrl(s)
	, m_MassflowAlarmCtrl(s)
	, m_TareCtrl(s)
	, m_fActualSetpoint(0.0f)
	, m_fLastLeistung(0.0f)
	, m_fPidPropGainGross(0.0f)
	, m_fInputFilter(0.0f)
	, m_fWeight(0.0f)
	, m_fMassflow(0.0f)
	, m_fBandV(0.0f)
	, m_fBeltLoadSetpoint(0.0f)
	, m_fBandlast(0.0f)
	, m_bStartupDelay(FALSE)
	, m_bBeltLoadSetpointChanged(FALSE)
	, m_bInputFilterChanged(FALSE)
	, m_bPropGrossGainChanged(FALSE)
	, m_bSampleIntervalChanged(FALSE)
	, m_bLeistungChanged(FALSE)
	, m_fMassflowFilter(0.0f)
	, m_fBeltLoadFilter(0.0f)
	, m_fAktLeistung(0.0f)
	, m_tMassflowNext(0)
	, m_lSampleInterval(0)
	, m_lMassflowIntervall(0)
	, m_ControlSteps{ eControlSteps::eStepStart }
{
	m_DriveCtrl.registerAlarmManager(this->m_aAlarm);

	m_WeightCtrl.registerAlarmManager(this->m_aAlarm);
	m_WeightCtrl.registerLoadcell(rlc);

	m_MassflowAlarmCtrl.registerAlarmManager(this->m_aAlarm);

	m_TareCtrl.registerAlarmManager(this->m_aAlarm);
	m_TareCtrl.registerDriveCtrl(m_DriveCtrl);
	m_TareCtrl.registerWeightCtrl(m_WeightCtrl);
}


//*********************************************************************************************
//*********************************************************************************************
float32_t CWbfControl ::  GetVMax(void) const
{
	float32_t fReduction = 0.0f;
	float32_t fMaxRotationalSpeed = 0.0f;
	uint32_t lWheelSize = 0L;

	Dose_DSVGetWbfReduction		(m_sID, &fReduction);
	Dose_DSVGetWbfWheelSize		(m_sID, &lWheelSize);
	Dose_DSVGetMaxRotationalSpeed		(m_sID, &fMaxRotationalSpeed);

	const float32_t fCircumference   = _F32(lWheelSize * (M_PI * fMM2M));	// in m
	const float32_t fRotationalSpeed = FDIV0(fMaxRotationalSpeed, fReduction);		// U/min
	const float32_t fVMax			  = (fCircumference * fRotationalSpeed) * fMIN2H;	        // m/h
	return fVMax;
}
//*********************************************************************************************
//*********************************************************************************************
inline void CWbfControl :: 	SetMassflow (const float32_t fAktLeistung)
{
    m_fAktLeistung		= __max(fAktLeistung,0.0f);
	m_AdsClient.SetMassflow(m_fAktLeistung);
}
//*********************************************************************************************
//*********************************************************************************************
void CWbfControl :: TotalizerSet (const float32_t _fAktLeistung)
{
    float32_t fAktLeistung    = __max(_fAktLeistung, 0.0f);
	if (m_aTotalizer.UpdateMassflow(m_st, fAktLeistung))
	{
		Dose_DSVSetTotalizer(m_sID, _F32(m_aTotalizer.GetTotalizer()));
	}
}
//*********************************************************************************************
//*********************************************************************************************
inline void CWbfControl ::  UpdateBeltLoadFilter( void )
{
	m_fBeltLoadFilter = (m_fMassflowFilter*m_fBandlast) + (1.0f-m_fMassflowFilter)*m_fBeltLoadFilter;
}
//*********************************************************************************************
//*********************************************************************************************
inline void CWbfControl ::  UpdateMassflowFilter( void )
{
	m_fMassflowFilter = (m_fMassflowFilter*m_fMassflow) + (1.0f-m_fMassflowFilter)*m_fMassflowFilter;
}
//*********************************************************************************************
//*********************************************************************************************
BOOL CWbfControl ::GetDoseSetpoint( void)
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
inline BOOL CWbfControl ::  GetBeltLoadSetpoint( void )
{
	float32_t fBeltLoadSetpoint = 0.0f;
	BOOL bChanged = FALSE;

	Dose_DSVGetChangedWbfBeltLoadSetpoint (m_sID, &bChanged) ;
	if ( bChanged )
	{
	    Dose_DSVPopWbfBeltLoadSetpoint	 (m_sID, &fBeltLoadSetpoint) ;
		m_fBeltLoadSetpoint = fBeltLoadSetpoint;
	}
	return bChanged;
}
//*********************************************************************************************
//*********************************************************************************************
inline BOOL CWbfControl ::  GetPidPropGainGross (void)
{
	BOOL bChanged			= FALSE;

	Dose_DSVGetChangedPidPropGainGross		(m_sID, &bChanged)	; // [0..1]
	if ( bChanged ) 
	{
		Dose_DSVPopPidPropGainGross			(m_sID, &m_fPidPropGainGross)	; // [0..1]
		m_fPidPropGainGross /= 100.0f;
	}
	return bChanged;
}
//*********************************************************************************************
//*********************************************************************************************
inline BOOL CWbfControl ::  GetInputFilter (void)
{
	BOOL bChanged			= FALSE;

	Dose_DSVGetChangedMassflowFilter	(m_sID, &bChanged)	; // [0..1]
	if ( bChanged ) 
	{
		Dose_DSVPopMassflowFilter(m_sID, &m_fMassflowFilter)	; // [0..1]
		m_fMassflowFilter /= 100.0f;
	}
	return bChanged;
}
//*********************************************************************************************
//*********************************************************************************************
BOOL CWbfControl :: GetVolumetric ( void)
{
	BOOL	bVolumetric	 = FALSE;
	BOOL	bLCAktiv = m_WeightCtrl.isLoadcellActive();

	Dose_DSVGetVolumetricState	    (m_sID, &bVolumetric);
    if ( (!bVolumetric) && (! bLCAktiv) )
    {
		Dose_DSVSetVolumetricState		(m_sID, TRUE) ;
        return TRUE;
    }
    return CBaseControl :: GetVolumetric ();
}
//*********************************************************************************************
//*********************************************************************************************
inline void CWbfControl ::  GetPidSampleInterval (void)
{
	Dose_DSVPopPidSampleInterval		(m_sID, &m_lSampleInterval)	; // [0..1]
    if ( m_lSampleInterval <= 0)
    {
        m_lSampleInterval = 100L;
		Dose_DSVSetPidSampleInterval	(m_sID, m_lSampleInterval)	; // [0..1]
    }
	m_lMassflowIntervall = m_lSampleInterval / 100;   // lIntervall;
}
//*******************************************************************************************************
//*******************************************************************************************************
inline BOOL CWbfControl ::  GetSampleInterval (void)
{
	BOOL bChanged			= FALSE;

	Dose_DSVGetChangedPidSampleInterval		(m_sID, &bChanged)	; // [0..1]
	if ( bChanged ) 
	{
		GetPidSampleInterval();
	}
	return bChanged;
}
//*******************************************************************************************************
//*******************************************************************************************************
void CWbfControl :: GetWeight ( void)
{
	m_WeightCtrl.Update(m_st);
	m_aLoadCell0 = m_WeightCtrl.GetWeight();
}
//*******************************************************************************************************
//*******************************************************************************************************
BOOL CWbfControl :: GetBeltLoad (void)
{
	GetWeight ();

	// Istgeschwindigkeit ausrechnen
	float32_t fEncoder = 0.0f;
	Dose_DSVGetActualLogoEncoder(m_sID, &fEncoder);
    if ( fEncoder < 0.0006 )
    {
        fEncoder = 0.0f;
    }

	float32_t fMaxRotationalSpeed = 0.0f;
	Dose_DSVGetMaxRotationalSpeed	 (m_sID, &fMaxRotationalSpeed);
	Dose_DSVSetActualRotationalSpeed (m_sID, fEncoder * fMaxRotationalSpeed) ;
    
	float32_t fV = GetVMax();
	m_fBandV = fV * fEncoder;
	Dose_DSVSetWbfActVelocity		(m_sID, m_fBandV);

	uint32_t lWeighingLine = 0L;
	Dose_DSVGetWbfWeighingLine		(m_sID, &lWeighingLine);

	BOOL bVerwiegungFull = FALSE;
	Dose_DSVGetWbfWeighingFull(m_sID, &bVerwiegungFull) ;
	const int32_t lFaktor = (bVerwiegungFull == 0) + 1;

	// Bandlast [kg/m]
	m_fBandlast = lFaktor * FDIV0(m_fWeight, lWeighingLine*fMM2M); 
	Dose_DSVSetWbfBeltLoadMassflow(m_sID, m_fBandlast) ;

	// Massflow in kg/h
	m_fMassflow = m_fBandlast * m_fBandV;
	return TRUE;
}
//*******************************************************************************************************
//*******************************************************************************************************
void CWbfControl :: BeltLoadMonitoring ( void)
{
	eEncoderState sEncoderState	= eEncoderState::ENCODERSTATE_OFF;
	Dose_DSVGetEncoderState (m_sID, &sEncoderState) ;
	if (sEncoderState != eEncoderState::ENCODERSTATE_OFF)
	{
		assert(m_aTimer.IsStarted());
		uint32_t	lS = 0L;
		Dose_DSVGetAlarmStartReactionDelay(m_sID, &lS);
		if (m_st > lS)
		{
			float32_t	fMinBeltLoad = 0.0f;
			Dose_DSVGetWbfAlarmMinBeltLoad(m_sID, &fMinBeltLoad);
			const BOOL bBandLastError = BOOL(m_fBeltLoadFilter < fMinBeltLoad);
			m_AdsClient.SetBeltLoadLimit(bBandLastError);
			m_aAlarm.SetAlarm(m_st, eAlarmError::ERROR_WBF_BELTLOAD_LOW, bBandLastError, eAlarmClass::eALARMTYP);
			m_aAlarm.CheckWarningLevel(m_st, eAlarmError::ERROR_WBF_BELTLOAD_LOW);
		}
	}
}
//*******************************************************************************************************
//*******************************************************************************************************
void CWbfControl :: ExtAlarmMonitoring ( void)
{
	eEncoderState sEncoderState = eEncoderState::ENCODERSTATE_OFF;
	Dose_DSVGetEncoderState(m_sID, &sEncoderState);
	if (sEncoderState != eEncoderState::ENCODERSTATE_OFF)
	{
		CBaseControl::CheckExtAlarm();
	}
}
//*******************************************************************************************************
//*******************************************************************************************************
void CWbfControl :: AlarmMonitoring ( void)
{
	m_DriveCtrl.Update(m_st);
	m_MassflowAlarmCtrl.Update(m_st);
	BeltLoadMonitoring ();
    ExtAlarmMonitoring ();
}
//*******************************************************************************************************
//*******************************************************************************************************
void CWbfControl ::  SetDriveCommand (float32_t f)
{
	m_DriveCtrl.UpdateDriveCommand(f);
}
//*******************************************************************************************************
//*******************************************************************************************************
void CWbfControl ::  SetDrehgeber (float32_t fAktLeistung, float32_t fAktBandlast)
{
	const float32_t fVMax = GetVMax ();
	const float32_t fV    = FDIV0(fAktLeistung, fAktBandlast);

	const float32_t fDriveCommand = FDIV0(fV, fVMax);

	// DriveCommand initialisieren
	m_DriveCtrl.UpdateDriveCommand(fDriveCommand);
    
}
//*******************************************************************************************************
//*******************************************************************************************************
void CWbfControl :: StartVol ( void )
{
	m_fLastLeistung = m_fActualSetpoint;
	SetMassflow (m_fActualSetpoint);
	TotalizerSet(m_fActualSetpoint)	;

	// DriveCommand initialisieren
	SetDrehgeber (m_fActualSetpoint, m_fBeltLoadSetpoint);

	m_bLeistungChanged				= FALSE;
	m_bBeltLoadSetpointChanged		= FALSE;
}
//*******************************************************************************************************
//*******************************************************************************************************
void CWbfControl :: StartGrav ( void)
{
	m_fLastLeistung  = m_fActualSetpoint;
	SetMassflow (m_fActualSetpoint);
	TotalizerSet(m_fActualSetpoint);

	// DriveCommand initialisieren
	SetDrehgeber (m_fActualSetpoint, m_fBandlast);

	m_bLeistungChanged				= FALSE;
	m_bBeltLoadSetpointChanged		= FALSE;
}
//*******************************************************************************************************
//*******************************************************************************************************
inline void CWbfControl ::  InitBeltLoadFilter (void)
{
	m_fBeltLoadFilter = m_fBandlast;
}
//*******************************************************************************************************
//*******************************************************************************************************
inline void CWbfControl ::  InitMassflowFilter (void)
{
	m_fMassflowFilter = m_fMassflow;
}
//*******************************************************************************************************
//*******************************************************************************************************
void CWbfControl :: InitStart ( void)
{
	LOGDEBUG(__FUNCTION__);

	float32_t fVerbrauch = 0.0f;

	m_bStartupDelay				= FALSE;
	m_tMassflowNext				= 0L;
	m_fLastLeistung				= 0.0f;
	m_bLeistungChanged			= FALSE;
	m_bPropGrossGainChanged		= FALSE;
	m_bInputFilterChanged	= FALSE;
	m_bSampleIntervalChanged	= FALSE;

	m_fMassflowFilter				= 0.0f;
	m_fBandlast						= 0.0f;

    Dose_DSVGetActualSetpoint (m_sID, &m_fActualSetpoint);
	Dose_DSVGetPidPropGainGross(m_sID, &m_fPidPropGainGross);
	m_fPidPropGainGross /= 100.0f;
	Dose_DSVGetMassflowFilter(m_sID, &m_fMassflowFilter);
	m_fMassflowFilter /= 100.0f;
    GetPidSampleInterval();

	Dose_DSVGetWbfBeltLoadSetpoint(m_sID, &m_fBeltLoadSetpoint);
	Dose_DSVSetWbfMinBeltLoadState( m_sID,base::eWbmState:: WBFMINBELTLOAD_NOT_REACHED)	;

	// Verbrauch
	Dose_DSVGetTotalizer   (m_sID, &fVerbrauch) ;
	m_aTotalizer.Open(m_st, fVerbrauch, m_fActualSetpoint);

	m_ControlSteps = eControlSteps::eStepStart;				// Warten
}
//*******************************************************************************************************
//*******************************************************************************************************
void CWbfControl :: StepStart ( void)
{
	LOGDEBUG(__FUNCTION__);

	uint32_t lDelay			= 0L;
	Dose_DSVGetStartupDelay(m_sID, &lDelay) ;
	if ( lDelay > 0L)
	{
		InitPrepareWaitDelay();
	}
	else
	{
		StartLeistung ();
	}
}
//*******************************************************************************************************
//*******************************************************************************************************
void CWbfControl :: InitPrepareWaitDelay ( void)
{
	LOGDEBUG(__FUNCTION__);

	uint32_t lDelay			= 0L;
	Dose_DSVGetStartupDelay(m_sID, &lDelay) ;

	m_tMassflowNext	= m_zt + lDelay*10;
	m_ControlSteps		    = eControlSteps::eStepPrepareWaitDelay;
}
//*******************************************************************************************************
//*******************************************************************************************************
void CWbfControl :: StepPrepareWaitDelay ( void)
{
	LOGDEBUG(__FUNCTION__);

	if ( m_zt >= m_tMassflowNext)
	{
		StartLeistung ();
	}
}
//*******************************************************************************************************
//*******************************************************************************************************
void CWbfControl :: StartLeistung ( void )
{
	LOGDEBUG(__FUNCTION__);

	BOOL bVolumetric = FALSE;
	Dose_DSVGetVolumetricState(m_sID, &bVolumetric);
	if ( bVolumetric )
	{
		StartLeistungVol();
	}
	else
	{
		StartLeistungGrav();
	}
}
//*******************************************************************************************************
//*******************************************************************************************************
void CWbfControl :: StartVolGrav ( void )
{
	LOGDEBUG(__FUNCTION__);

	BOOL bVolumetric = FALSE;
	Dose_DSVGetVolumetricState(m_sID, &bVolumetric) ;
	if ( bVolumetric )
	{
		StartVolumetric();
	}
	else
	{
		StartGravimetrisch();
	}
}
//*******************************************************************************************************
//*******************************************************************************************************
void CWbfControl :: StartLeistungVol ( void)
{
	LOGDEBUG(__FUNCTION__);

	StartVol();
    Dose_DSVSetGravVolState	        (m_sID, FALSE )          ;
	InitPrepareVol();
}
//*******************************************************************************************************
//*******************************************************************************************************
void CWbfControl :: InitPrepareVol ( void)
{
	LOGDEBUG(__FUNCTION__);

	m_tMassflowNext  = m_zt + 50;    // 5 Sekunden warten
	m_ControlSteps		    = eControlSteps::eStepPrepareVol;
}
//*******************************************************************************************************
//*******************************************************************************************************
void CWbfControl :: StepPrepareVol ( void)
{
	LOGDEBUG(__FUNCTION__);

	BOOL bVolumetric = FALSE;

	Dose_DSVGetVolumetricState(m_sID, &bVolumetric) ;
	if (  ! bVolumetric )
	{
		StartVolGrav();
	}
	else
	{
	    if ( m_bLeistungChanged || m_bBeltLoadSetpointChanged )
	    {
		    StartLeistungVol();		// zurueck zu 1
	    }
        else
        {
	        if ( m_zt >= m_tMassflowNext)
	        {
                StartVolumetric();
            }
        }
    }
}
//*******************************************************************************************************
//*******************************************************************************************************
void CWbfControl :: StartVolumetric ( void)
{
	LOGDEBUG(__FUNCTION__);

	InitMassflowFilter ();
	InitBeltLoadFilter ();
	m_bInputFilterChanged    = FALSE;
    m_bSampleIntervalChanged   = FALSE;
    InitVolumetric();
}
//*******************************************************************************************************
//*******************************************************************************************************
void CWbfControl :: InitVolumetric ( void)
{
	LOGDEBUG(__FUNCTION__);

	m_tMassflowNext = m_zt + m_lMassflowIntervall;
	m_ControlSteps	 = eControlSteps::eStepVolumetric;
}
//*******************************************************************************************************
//*******************************************************************************************************
void CWbfControl :: StepVolumetric ( void)
{
	LOGDEBUG(__FUNCTION__);

	BOOL bVolumetric = FALSE;
	Dose_DSVGetVolumetricState(m_sID, &bVolumetric) ;
	if (  ! bVolumetric )
	{
		StartVolGrav();
	}
	else
	{
		if ( m_bLeistungChanged || m_bBeltLoadSetpointChanged)
		{
			StartLeistungVol();		// zurueck zu 1
		}
        else
        {
			BOOL bChanged = 	m_bInputFilterChanged || 
						        m_bSampleIntervalChanged;
            if ( bChanged )
            {
                StartVolumetric();
            }
            else
            {
                if ( m_zt >= m_tMassflowNext)
			    {
			        // naechsten Massflow
			        m_tMassflowNext = m_zt + m_lMassflowIntervall;

				    // Filtern
				    UpdateMassflowFilter ();

				    // Aktuelle (Ist)Leistung setzen
				    SetMassflow (m_fMassflowFilter);

                    // Verbrauch aktualisieren
                    TotalizerSet(m_fMassflowFilter);
    	
				    // gefilterten Bandlast updaten
				    UpdateBeltLoadFilter();
                }
            }
        }
	}
}
//*******************************************************************************************************
//*******************************************************************************************************
void CWbfControl :: StartLeistungGrav ( void)
{
	LOGDEBUG(__FUNCTION__);

	StartGrav();
	InitPrepareGrav();
}
//*******************************************************************************************************
//*******************************************************************************************************
void CWbfControl :: InitPrepareGrav ( void)
{
	LOGDEBUG(__FUNCTION__);

	m_tMassflowNext  = m_zt + 50;    // 5 Sekunden warten
	m_ControlSteps		    = eControlSteps::eStepPrepareGrav;
}
//*******************************************************************************************************
//*******************************************************************************************************
void CWbfControl :: StepPrepareGrav ( void)
{
	LOGDEBUG(__FUNCTION__);

	BOOL bVolumetric = FALSE;

	Dose_DSVGetVolumetricState(m_sID, &bVolumetric) ;
	if (  bVolumetric )
	{
		StartVolGrav();
	}
	else
	{
		if ( m_bLeistungChanged || m_bBeltLoadSetpointChanged )
		{
			StartLeistungGrav();		// zurueck zu 1
		}
        else
        {
			float32_t fMinBeltLoad = 0.0f;
			Dose_DSVGetWbfBeltLoadVol	(m_sID, &fMinBeltLoad)		;
	        if ( m_fBandlast < fMinBeltLoad )
	        {
	            StartForceVolumetric();
	        }
            else
            {
		        if ( m_zt >= m_tMassflowNext)
		        {
                    StartGravimetrisch();
                }
            }
        }
    }
}
//*******************************************************************************************************
//*******************************************************************************************************
void CWbfControl :: StartGravimetrisch ( void)
{
	LOGDEBUG(__FUNCTION__);

	Dose_DSVSetGravVolState	        (m_sID, FALSE )          ;
	InitMassflowFilter ();
	InitBeltLoadFilter ();
	m_bPropGrossGainChanged  = FALSE;
	m_bInputFilterChanged        = FALSE;
    m_bSampleIntervalChanged       = FALSE;
    InitGravimetricStart();
}
//*******************************************************************************************************
//*******************************************************************************************************
void CWbfControl :: InitGravimetricStart ( void)
{
	m_tMassflowNext	= m_zt + m_lMassflowIntervall;    
	m_ControlSteps			= eControlSteps::eStepGravimetricStart;
}
//*******************************************************************************************************
//*******************************************************************************************************
void CWbfControl :: StepGravimetricStart ( void)
{
	LOGDEBUG(__FUNCTION__);

	BOOL bVolumetric = FALSE;
	Dose_DSVGetVolumetricState(m_sID, &bVolumetric) ;
	if (  bVolumetric )
	{
		StartVolGrav();
	}
	else
	{
		if ( m_bLeistungChanged || m_bBeltLoadSetpointChanged )
		{
			StartLeistungGrav();
		}
		else
		{
			if ( m_zt >= m_tMassflowNext)
			{
				// naechsten Massflow
				m_tMassflowNext = m_zt + m_lMassflowIntervall;

				// Filtern
				UpdateMassflowFilter ();

				// Aktuelle (Ist)Leistung setzen
				SetMassflow (m_fMassflowFilter);

                // Verbrauch aktualisieren
                TotalizerSet(m_fMassflowFilter);

				// gefilterten Bandlastwert aktualisieren
				UpdateBeltLoadFilter ();

				float32_t fMinBeltLoad = 0.0f;
				Dose_DSVGetWbfBeltLoadVol	(m_sID, &fMinBeltLoad)		;
				if ( m_fBeltLoadFilter < fMinBeltLoad )
				{
				    StartForceVolumetric();
				}
				else
				{
					InitGravimetric ();
				}
			}
		}
	}
}
//*******************************************************************************************************
//*******************************************************************************************************
void CWbfControl :: InitGravimetric ( void)
{
	LOGDEBUG(__FUNCTION__);


	m_PidControl.Open (m_fPidPropGainGross, 0.1f, 0.05f);

	const float32_t fFehler = m_fActualSetpoint - m_fMassflow;
	float32_t fD		    = m_PidControl.Start(m_zt, fFehler);

	// Neuer Sollwert zum Einstellen
	m_fLastLeistung			=__max(m_fLastLeistung + fD, 0.0f);

	SetDrehgeber (m_fLastLeistung, m_fBeltLoadFilter);

	m_tMassflowNext	= m_zt + m_lMassflowIntervall;
	m_ControlSteps			= eControlSteps::eStepGravimetric;
}
//*******************************************************************************************************
//*******************************************************************************************************
void CWbfControl :: StepGravimetric ( void)
{
	LOGDEBUG(__FUNCTION__);

    BOOL bVolumetric  = FALSE;
	Dose_DSVGetVolumetricState(m_sID, &bVolumetric) ;
	if (  bVolumetric )
	{
		StartVolGrav();
	}
	else
	{
		if ( m_bLeistungChanged || m_bBeltLoadSetpointChanged )
		{
			StartLeistungGrav();
		}
		else
		{
			BOOL bChanged = 	m_bPropGrossGainChanged		||
						        m_bInputFilterChanged			|| 
						        m_bSampleIntervalChanged;

			if ( bChanged )
			{
				StartGravimetrisch();
			}
			else
			{
				if ( m_zt >= m_tMassflowNext)
				{
					// naechsten Massflow
					m_tMassflowNext = m_zt + m_lMassflowIntervall;

					// Filtern
					UpdateMassflowFilter ();

					// Aktuelle (Ist)Leistung setzen
					SetMassflow (m_fMassflowFilter);

                    // Verbrauch aktualisieren
                    TotalizerSet(m_fMassflowFilter);
	
					// gefilterten Bandlast updaten
					UpdateBeltLoadFilter();

					float32_t fMinBeltLoad = 0.0f;
					Dose_DSVGetWbfBeltLoadVol	(m_sID, &fMinBeltLoad)		;
					if ( m_fBeltLoadFilter < fMinBeltLoad )
					{
						StartForceVolumetric();
					}
					else
					{
						// Bandgeschwindigkeit
						const float32_t fFehler = m_fActualSetpoint - m_fMassflow;
						float32_t fD = m_PidControl.Update(m_zt, fFehler);
						m_fLastLeistung		= __max(m_fLastLeistung + fD, 0.0f);
						SetDrehgeber (m_fLastLeistung, m_fBeltLoadFilter);
					}
				}
			}
		}
	}
}
//*******************************************************************************************************
//*******************************************************************************************************
void CWbfControl :: StartForceVolumetric ( void)
{
	LOGDEBUG(__FUNCTION__);

	StartVol();
    Dose_DSVSetGravVolState	        (m_sID, TRUE );
	Dose_DSVSetWbfMinBeltLoadState	( m_sID, base::eWbmState::WBFMINBELTLOAD_REACHED);
	InitForceVolumetric();
}
//*******************************************************************************************************
//*******************************************************************************************************
void CWbfControl :: InitForceVolumetric ( void)
{
	LOGDEBUG(__FUNCTION__);

	m_tMassflowNext	= m_zt + m_lMassflowIntervall;
	m_ControlSteps			= eControlSteps::eStepForceVolumetric;
}
//*******************************************************************************************************
//*******************************************************************************************************
void CWbfControl :: StepForceVolumetric ( void)
{
	LOGDEBUG(__FUNCTION__);

	BOOL bVolumetric = FALSE;
	Dose_DSVGetVolumetricState(m_sID, &bVolumetric) ;
	if (  bVolumetric )
	{
		StartVolGrav();
	}
	else
	{
	    if ( m_bLeistungChanged || m_bBeltLoadSetpointChanged )
	    {
	        StartLeistungGrav();
	    }
	    else
	    {
		    if ( m_zt >= m_tMassflowNext)
		    {
			    m_tMassflowNext	= m_zt + m_lMassflowIntervall;

			    // Filtern
			    UpdateMassflowFilter ();

			    // Aktuelle (Ist)Leistung setzen
			    SetMassflow (m_fMassflowFilter);

                // Verbrauch aktualisieren
                TotalizerSet(m_fMassflowFilter);

			    // Filtern
			    UpdateBeltLoadFilter ();

				float32_t fMinBeltLoad = 0.0f;
				Dose_DSVGetWbfBeltLoadVol	(m_sID, &fMinBeltLoad)		;
			    if ( m_fBeltLoadFilter >= fMinBeltLoad )
			    {
				    StartGravimetrisch();
			    }
		    }
        }
	}
}
//*******************************************************************************************************
//*******************************************************************************************************
BOOL CWbfControl :: Control ( void)
{
	switch (m_ControlSteps)
	{
		default:
		case eControlSteps::eStepStart:
			{
				StepStart ();
			}
			break;


		case eControlSteps::eStepPrepareWaitDelay:
			{
				StepPrepareWaitDelay ();

			}
			break;

		case eControlSteps::eStepPrepareVol:
			{
				StepPrepareVol ();
			}
			break;

		case eControlSteps::eStepVolumetric:
			{
				StepVolumetric ();
			}
			break;

		case eControlSteps::eStepPrepareGrav:
			{
				StepPrepareGrav ();
			}
			break;

		case eControlSteps::eStepGravimetricStart:
			{
				StepGravimetricStart ();
			}
			break;

		case eControlSteps::eStepGravimetric:
			{
				StepGravimetric ();
			}
			break;

		case eControlSteps::eStepForceVolumetric:
		{
				StepForceVolumetric ();
			}
			break;
	}
	AlarmMonitoring();
	return TRUE;
}
//*******************************************************************************************************
//*******************************************************************************************************
void CWbfControl :: ClearAlarm   (const base::eAlarmError lError)
{
    CBaseControl :: ClearAlarm (lError);
    const BOOL bBandLastError = ( lError == eAlarmError::ERROR_WBF_BELTLOAD_LOW) || (lError == eAlarmError::ERROR_MAX);
    if ( bBandLastError )
    {
		m_AdsClient.SetBeltLoadLimit(FALSE);
    }
}
//*******************************************************************************************************
//*******************************************************************************************************
BOOL CWbfControl :: InitExecute ( void)
{
	auto result = CBaseControl :: InitExecute ();
	if (result)
	{
		m_fActualSetpoint = 0.0f;
		m_fLastLeistung = 0.0f;
		m_fPidPropGainGross = 0.0f;
		m_fMassflowFilter = 0.0f;
		m_fWeight = 0.0f;
		m_fMassflow = 0.0f;
		m_fBandV = 0.0f;
		m_fBeltLoadSetpoint = 0.0f;
		m_fBandlast = 0.0f;
		m_bStartupDelay = FALSE;
		m_bBeltLoadSetpointChanged = FALSE;
		m_bInputFilterChanged = FALSE;
		m_bPropGrossGainChanged = FALSE;
		m_bSampleIntervalChanged = FALSE;
		m_bLeistungChanged = FALSE;
		m_fMassflowFilter = 0.0f;
		m_fBeltLoadFilter = 0.0f;
		m_fAktLeistung = 0.0f;
		m_tMassflowNext = 0;
		m_lSampleInterval = 0;
		m_lMassflowIntervall = 0;

		m_AdsClient.Init();

		m_DriveCtrl.InitExecute();
		m_WeightCtrl.InitExecute();
		m_WeightCtrl.Start(m_st);
		m_MassflowAlarmCtrl.InitExecute();
		SetMassflow(0.0f);
		Dose_DSVSetWbfMinBeltLoadState(m_sID, base::eWbmState::WBFMINBELTLOAD_NOT_REACHED);

		m_LoadCell.SetPriority(m_sID, LC_PRIORITY::LC_PRIORITY_NORMAL);
	}
	return result;
}
//*******************************************************************************************************
//*******************************************************************************************************
BOOL CWbfControl :: ExitExecute ( void)
{
	auto result = CBaseControl::IsInit();
	if (result)
	{
		Stop();
		m_LoadCell.SetPriority(m_sID, LC_PRIORITY::LC_PRIORITY_NORMAL);
		m_WeightCtrl.Stop();
		m_WeightCtrl.ExitExecute();
		m_DriveCtrl.Stop();
		m_DriveCtrl.ExitExecute();
		m_AdsClient.Exit();
		result = CBaseControl::ExitExecute();
	}
	return result;
}
//*******************************************************************************************************
//*******************************************************************************************************
BOOL CWbfControl :: Stop ( void)
{
    auto result = CBaseControl :: IsStarted();
    if ( result)
    {
		m_DriveCtrl.Stop();
		m_MassflowAlarmCtrl.Stop();
	    SetMassflow		     (0.0f);

	    Dose_DSVSetWbfMinBeltLoadState	(m_sID, base::eWbmState::WBFMINBELTLOAD_NOT_REACHED)	;
		m_TareCtrl.Stop();
        m_aTotalizer.Close();
		m_LoadCell.SetPriority(m_sID, LC_PRIORITY::LC_PRIORITY_NORMAL);
		m_ControlSteps = eControlSteps::eStepStart;
		result = CBaseControl::Stop();
	}
	return result;
}
//*******************************************************************************************************
//*******************************************************************************************************
BOOL CWbfControl :: Start (const uint32_t t )
{
    auto result = CBaseControl :: Start(t);
    if (result)
    {
	    InitStart();

		m_DriveCtrl.Start(m_st);
		m_MassflowAlarmCtrl.Start(m_st);
		SetMassflow (0.0f);

	    GetBeltLoad();
		m_LoadCell.SetPriority (m_sID, LC_PRIORITY::LC_PRIORITY_HIGH);
    }
	return result;
}
//*******************************************************************************************************
//*******************************************************************************************************
BOOL CWbfControl :: Execute ( void)
{
	// aktuelle Zeit holen
	auto result = CBaseControl :: Execute();
	if (result)
	{
		// aktuelles Gewicht holen
		GetBeltLoad();

		// aktuellen Sollwert holen
		BOOL bChanged = GetDoseSetpoint();
		if (bChanged)
		{
			m_bLeistungChanged = TRUE;
		}

		// aktuellen BeltLoadSetpoint holen
		bChanged = GetBeltLoadSetpoint();
		if (bChanged)
		{
			m_bBeltLoadSetpointChanged = TRUE;
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

		// Proportional Grobverstaerkung
		bChanged = GetPidPropGainGross();
		if (bChanged)
		{
			m_bPropGrossGainChanged = TRUE;
		}
		//--------------------------------------
		// Freigabe
		BOOL bRelease = FALSE;
		Dose_DSVGetRelease(m_sID, &bRelease);

		switch (GetOperatingMode())
		{
			default:
			case eOperatingMode::IDLE:
			{
				BOOL bTaring = GetTaring();
				if (bTaring)
				{
					m_TareCtrl.InitExecute();
					m_TareCtrl.Start(m_st);
					SetOperatingMode(eOperatingMode::TARING);
				}
				else
				{
					if (bRelease)
					{
						Start();
					}
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

			case eOperatingMode::TARING:
			{
				BOOL bTaring = GetTaring();
				if (bTaring)
				{
					m_TareCtrl.Update(m_st);
				}
				else
				{
					m_TareCtrl.Stop();
					m_TareCtrl.ExitExecute();
					SetOperatingMode(eOperatingMode::IDLE);
				}
				ExtAlarmMonitoring();
			}
			break;
		}
		SetAlarmOutput();
	}
	return result;
}


