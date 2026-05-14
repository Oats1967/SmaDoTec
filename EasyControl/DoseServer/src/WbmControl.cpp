//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Implementation of module WbmControl.cpp
///
/// @file   WbmControl.cpp
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
#include "WbmControl.h"
#include "DoseDataLib/include/DoseData.h"



using namespace base;



#define MASSFLOWFAKTOR 1
#define REGELFAKTOR   1
#define STARTFAKTOR   (10*REGELFAKTOR)
#define STARTDELAY    (5 )
#define DREHGEBERSTARTDELAY 5
//#define M_PI 3.141592654

static const float32_t fMIN2H    = 60.0f;
static const float32_t fMM2M	 = 1E-03f;		// 0.001 m
  

//*****************************************************************************************************
//*****************************************************************************************************
CWbmControl::CWbmControl(int32_t s, loadcell::ILCModuleInterface& rlc) : CBaseControl(s)
, m_LoadCell(rlc)
, m_AdsClient(s)
, m_DriveCtrl(s)
, m_WeightCtrl(s)
, m_fWeight(0)
, m_fMassflow(0.0f)
, m_bStartupDelay(FALSE)
, m_bInputFilterChanged(FALSE)
, m_fBandV(0.0f)
, m_bSampleIntervalChanged(FALSE)
, m_fInputFilter(0.0f)
, m_fMassflowFilter(0.0f)
, m_fBeltLoadFilter(0.0f)
, m_fBeltLoadSetpoint(0.0f)
, m_fBandlast(0.0f)
, m_tMassflowNext(0)
, m_lMassflowIntervall(0)
, m_tTaraStop(0)
, m_lSampleInterval(0)
, m_ControlSteps{ 0 }
{
	m_DriveCtrl.registerAlarmManager(this->m_aAlarm);
	m_WeightCtrl.registerAlarmManager(this->m_aAlarm);
	m_WeightCtrl.registerLoadcell(rlc);
}
//*****************************************************************************************************
//*****************************************************************************************************
float32_t CWbmControl ::  GetVMax(void)
{
	//----------------------------------------------------------------------------
	// Bandgeschwindigkeit in m/s
	float32_t fReduction = 0.0f;
	float32_t fMaxRotationalSpeed = 0.0f;
	uint32_t lWheelSize = 0L;

	Dose_DSVGetWbfReduction				(m_sID, &fReduction);
	Dose_DSVGetWbfWheelSize			(m_sID, &lWheelSize);
	Dose_DSVGetMaxRotationalSpeed					(m_sID, &fMaxRotationalSpeed);

	const float32_t fCircumference   = _F32(lWheelSize * (M_PI * fMM2M));	// in m
	const float32_t fRotationalSpeed = FDIV0(fMaxRotationalSpeed, fReduction);		// U/min
	const float32_t fVMax	  = (fCircumference * fRotationalSpeed) * fMIN2H;	        // m/h
	return fVMax;
}
//*****************************************************************************************************
//*****************************************************************************************************
inline void CWbmControl :: SetMassflow (float32_t fAktLeistung)
{
    fAktLeistung = __max(fAktLeistung,0.0f);
	m_AdsClient.SetMassflow(fAktLeistung);
}
//*****************************************************************************************************
//*****************************************************************************************************
void CWbmControl :: TotalizerSet(const float32_t _fAktLeistung)
{
	float32_t fAktLeistung = __max(_fAktLeistung, 0.0f);
	if (m_aTotalizer.UpdateMassflow(m_st, fAktLeistung))
	{
		Dose_DSVSetTotalizer(m_sID, _F32(m_aTotalizer.GetTotalizer()));
	}
}
//*****************************************************************************************************
//*****************************************************************************************************
inline void CWbmControl ::  UpdateBeltLoadFilter( void )
{
	m_fBeltLoadFilter = (m_fInputFilter*m_fBandlast) + (1.0f-m_fInputFilter)*m_fBeltLoadFilter;
}
//*****************************************************************************************************
//*****************************************************************************************************
inline void CWbmControl ::  UpdateMassflowFilter( void )
{
	m_fMassflowFilter = (m_fInputFilter*m_fMassflow) + (1.0f-m_fInputFilter)*m_fMassflowFilter;
}
//*****************************************************************************************************
//*****************************************************************************************************
BOOL CWbmControl :: GetAnlagenModus ( void) const
{
    BOOL bLocal = FALSE;
    BOOL bChanged = FALSE;

	// Anlagenmodus  immer lokal
	Dose_DSVGetLocalModeState		(m_sID, &bLocal);
    if ( ! bLocal )
    {
	    Dose_DSVSetLocalModeState	(m_sID, TRUE);
        bChanged = TRUE;
    }
    return  bChanged;
}
//*****************************************************************************************************
//*****************************************************************************************************
BOOL CWbmControl ::  GetInputFilter (void)
{
	BOOL bChanged			= FALSE;

	Dose_DSVGetChangedMassflowFilter		(m_sID, &bChanged); // [0..1]
	if ( bChanged ) 
	{
		Dose_DSVPopMassflowFilter			(m_sID, &m_fInputFilter); // [0..1]
		m_fInputFilter /= 100.0f;
	}
	return bChanged;
}
//*****************************************************************************************************
//*****************************************************************************************************
inline void CWbmControl ::  GetPidSampleInterval (void)
{
	Dose_DSVPopPidSampleInterval		(m_sID, &m_lSampleInterval); // [0..1]
    if ( m_lSampleInterval <= 0)
    {
        m_lSampleInterval = 100L;
		Dose_DSVSetPidSampleInterval		(m_sID, m_lSampleInterval); // [0..1]
    }
	m_lMassflowIntervall = m_lSampleInterval / 100;   // lIntervall;
}
//*******************************************************************************************************
//*******************************************************************************************************
inline BOOL CWbmControl ::  GetSampleInterval (void)
{
	BOOL bChanged			= FALSE;

	Dose_DSVGetChangedPidSampleInterval		(m_sID, &bChanged); // [0..1]
	if ( bChanged ) 
	{
		GetPidSampleInterval();
	}
	return bChanged;
}
//*****************************************************************************************************
//*****************************************************************************************************
void CWbmControl::GetWeight(void)
{
	m_WeightCtrl.Update(m_st);
	auto& pair = m_WeightCtrl.GetWeight();
	m_fWeight = pair.m_fWeight;
}
//*****************************************************************************************************
//*****************************************************************************************************
BOOL CWbmControl :: GetBeltLoad (void)
{
	uint32_t lWeighingLine = 0L;
	float32_t fEncoder   = 0.0f;
    float32_t fMaxRotationalSpeed  = 0.0f;
	BOOL bVerwiegungFull = FALSE;

	GetWeight ();

	// Istgeschwindigkeit ausrechnen
	float32_t fV = GetVMax ();
	Dose_DSVGetActualLogoEncoder(m_sID, &fEncoder);
	if ( fEncoder < 0.0006 )
    {
        fEncoder = 0.0f;
    }
    
	Dose_DSVGetMaxRotationalSpeed	 (m_sID, &fMaxRotationalSpeed);
	Dose_DSVSetActualRotationalSpeed(m_sID, fEncoder * fMaxRotationalSpeed);
    
	m_fBandV = fV * fEncoder;
	Dose_DSVSetWbfActVelocity				(m_sID, m_fBandV);

	Dose_DSVGetWbfWeighingLine				(m_sID, &lWeighingLine);
	Dose_DSVGetWbfWeighingFull(m_sID, &bVerwiegungFull);
	const int32_t lFaktor = (bVerwiegungFull == 0) + 1;

	// Bandlast [kg/m]
	m_fBandlast = lFaktor * FDIV0(m_fWeight, lWeighingLine*fMM2M); 
	Dose_DSVSetWbfBeltLoadMassflow(m_sID, m_fBandlast);

	// Massflow in kg/h
	m_fMassflow =  m_fBandlast * m_fBandV;
	return TRUE;
}
//*****************************************************************************************************
//*****************************************************************************************************
void CWbmControl::SetDriveCommand(const float32_t )
{
	// m_DriveCtrl.UpdateDriveCommand(m_st, 0.0F);
}
//*****************************************************************************************************
//*****************************************************************************************************
void CWbmControl :: BeltLoadMonitoring ( void)
{
	float32_t fMinBeltLoad      = 0.0f;
    float32_t fMaxBeltLoad      = 0.0f;
    uint32_t ulS                = 0L;

    assert(m_aTimer.IsStarted());
    Dose_DSVGetAlarmStartReactionDelay(m_sID, &ulS) ;
    if ( m_st > ulS )
    {
	    Dose_DSVGetWbfAlarmMaxBeltLoad	(m_sID, &fMaxBeltLoad);
        const BOOL bBandLastErrorMax = ( m_fBeltLoadFilter > fMaxBeltLoad );
	    m_aAlarm.SetAlarm(m_st, eAlarmError::ERROR_WBF_BELTLOAD_HIGH, bBandLastErrorMax, eAlarmClass::eALARMTYP);

	    Dose_DSVGetWbfAlarmMinBeltLoad	(m_sID, &fMinBeltLoad)		;
        const BOOL bBandLastErrorMin = ( m_fBeltLoadFilter < fMinBeltLoad );
	    m_aAlarm.SetAlarm(m_st, eAlarmError::ERROR_WBF_BELTLOAD_LOW, bBandLastErrorMin, eAlarmClass::eALARMTYP);

        const BOOL bBandLastError = bBandLastErrorMin || bBandLastErrorMax;
		m_AdsClient.SetBeltLoadLimit(bBandLastError);

	    m_aAlarm.CheckWarningLevel(m_st, eAlarmError::ERROR_WBF_BELTLOAD_LOW);
	    m_aAlarm.CheckWarningLevel(m_st, eAlarmError::ERROR_WBF_BELTLOAD_HIGH);
    }
}
//*****************************************************************************************************
//*****************************************************************************************************
void CWbmControl :: AlarmMonitoring ( void)
{
	m_DriveCtrl.Update(m_st);
	BeltLoadMonitoring  ();
	CheckExtAlarm();
	SetAlarmOutput();
}
//*****************************************************************************************************
//*****************************************************************************************************
void CWbmControl :: StartVol ( void )
{
	// Verbrauch initialisieren
	TotalizerSet(m_fMassflowFilter);
}
//*****************************************************************************************************
//*****************************************************************************************************
inline void CWbmControl ::  InitBeltLoadFilter (void)
{
	m_fBeltLoadFilter = m_fBandlast;
}
//*****************************************************************************************************
//*****************************************************************************************************
inline void CWbmControl ::  InitMassflowFilter (void)
{
	m_fMassflowFilter = m_fMassflow;
}
//*****************************************************************************************************
//*****************************************************************************************************
void CWbmControl :: InitStart ( void)
{
	float32_t fVerbrauch = 0.0f;

	m_bStartupDelay			= FALSE;
	m_tMassflowNext					= 0L;
	m_lMassflowIntervall				= 0;
	m_bInputFilterChanged		= FALSE;

	m_fMassflowFilter				= 0.0f;
	m_fBandlast						= 0.0f;

	Dose_DSVGetMassflowFilter			(m_sID, &m_fInputFilter)			; // [0..1]
	m_fInputFilter /= 100.0f;
	GetPidSampleInterval();

	Dose_DSVGetWbfBeltLoadSetpoint		(m_sID, &m_fBeltLoadSetpoint)			;
	Dose_DSVSetWbfMinBeltLoadState		( m_sID, base::eWbmState::WBFMINBELTLOAD_NOT_REACHED)	;

	// Totalizer
	Dose_DSVGetTotalizer   (m_sID, &fVerbrauch) ;
	m_aTotalizer.Open(m_st, fVerbrauch, 0.0f);
}
//*****************************************************************************************************
//*****************************************************************************************************
void CWbmControl :: Phase0 ( void)
{
	uint32_t lDelay			= 0L;
	Dose_DSVGetStartupDelay(m_sID, &lDelay) ;
	if ( lDelay > 0L)
	{
		Init1();
	}
	else
	{
		Init2();
	}
}
//*****************************************************************************************************
//*****************************************************************************************************
void CWbmControl :: Init1 ( void)
{
	uint32_t lDelay			= 0L;
	Dose_DSVGetStartupDelay(m_sID, &lDelay) ;

	m_tMassflowNext	= m_zt+ lDelay*10;
	m_ControlSteps			= 1;				// Warten
}
//*****************************************************************************************************
//*****************************************************************************************************
void CWbmControl :: Phase1 ( void)
{
	if ( m_zt >= m_tMassflowNext)
	{
		Init2 ();
	}
}
//*****************************************************************************************************
//*****************************************************************************************************
void CWbmControl :: Init2 ( void)
{
    StartVol();
	InitMassflowFilter ();
	InitBeltLoadFilter ();

	m_tMassflowNext		= m_zt + m_lMassflowIntervall;
	m_ControlSteps				  = 2;
}
//*****************************************************************************************************
//*****************************************************************************************************
void CWbmControl :: Phase2 ( void)
{
    TotalizerSet(m_fMassflowFilter);

	if ( m_zt >= m_tMassflowNext)
	{
		// naechsten Massflow
		m_tMassflowNext = m_zt + m_lMassflowIntervall;

		// Filtern
		UpdateMassflowFilter ();

		// Aktuelle (Ist)Leistung setzen
		SetMassflow (m_fMassflowFilter);

		// gefilterten Bandlastwert aktualisieren
		UpdateBeltLoadFilter ();
	}
}
//*****************************************************************************************************
//*****************************************************************************************************
BOOL CWbmControl :: Control ( void)
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

	}
	return TRUE;
}
//*****************************************************************************************************
//*****************************************************************************************************
BOOL CWbmControl :: TareStart ( void)
{
	Dose_DSVSetTaraState (m_sID, TRUE) ;
    m_tTaraStop         = m_st + 2;
    m_ControlSteps             = 0;
	return TRUE;
}
//*****************************************************************************************************
//*****************************************************************************************************
BOOL CWbmControl :: TareStop ( void)
{
	Dose_DSVSetTaraState			    (m_sID, FALSE);
    return TRUE;
}
//*****************************************************************************************************
//*****************************************************************************************************
BOOL CWbmControl :: TareRun ( void)
{
	uint32_t    lTime           = 0L;
    BOOL    bIOReleaseInput = FALSE;


    switch (m_ControlSteps)
    {
        case 0:
                {
	                Dose_DSVGetWbfTareMeasurementTime	 (m_sID, &lTime) ;
                    if ( lTime > 0 )
                    {
						Dose_DSVGetIOReleaseInput(m_sID, &bIOReleaseInput)		;
                        if ( ! bIOReleaseInput )
	                    {
		                    Stop();
	                    }
                        else
                        {
							m_fTaraWeightFifo.Init();
                            m_ControlSteps = 1;
                        }
                    }
                    else
                    {
						m_WeightCtrl.UpdateTaraWeight();
                        m_ControlSteps = 2;
                    }
                }
                break;

          case 1:
                {
					Dose_DSVGetIOReleaseInput(m_sID, &bIOReleaseInput);
                    if ( ! bIOReleaseInput )
	                {
		                Stop();
	                }
                    else
                    {
						const auto& rRawWeight = m_WeightCtrl.GetRawWeight();
						m_fTaraWeightFifo.Add(rRawWeight.m_fWeight);
						float32_t fTaraWeight = m_fTaraWeightFifo.GetMean();
						m_WeightCtrl.SetTaraWeight(fTaraWeight);
	                    Dose_DSVGetWbfTareMeasurementTime	 (m_sID, &lTime) ;
	                    assert(lTime > 0);
	                    if ( m_st >= lTime)
	                    {
                            m_ControlSteps = 2;
	                    }
                    }
                }
                break;

            case 2:
                {
				Dose_DSVGetIOReleaseInput(m_sID, &bIOReleaseInput);
				if ( ! bIOReleaseInput )
	                {
		                Stop();
	                }
                    else
                    {
		                if ( m_st >= m_tTaraStop)
	                    {
		                    Stop();
	                    }
                    }
                }
                break;
    }
	return TRUE;
}
//*****************************************************************************************************
//*****************************************************************************************************
void CWbmControl :: ClearAlarm (const base::eAlarmError lError)
{
    CBaseControl :: ClearAlarm (lError);
    const BOOL bBandLastError = ( lError == eAlarmError::ERROR_WBF_BELTLOAD_LOW) || ( lError == eAlarmError::ERROR_WBF_BELTLOAD_HIGH) || (lError == eAlarmError::ERROR_MAX);
    if ( bBandLastError )
    {
		m_AdsClient.SetBeltLoadLimit(FALSE);
    }
}
//*****************************************************************************************************
//*****************************************************************************************************
BOOL CWbmControl :: InitExecute ( void)
{
	auto result = CBaseControl :: InitExecute ();
	if (result)
	{
		m_fWeight = 0.0f;
		m_fMassflow = 0.0f;
		m_bStartupDelay = FALSE;
		m_bInputFilterChanged = FALSE;
		m_fBandV = 0.0f;
		m_bSampleIntervalChanged = FALSE;
		m_fInputFilter = 0.0f;
		m_fMassflowFilter = 0.0f;
		m_fBeltLoadFilter = 0.0f;
		m_fBeltLoadSetpoint = 0.0f;
		m_fBandlast = 0.0f;
		m_tMassflowNext = 0;
		m_lMassflowIntervall = 0;
		m_tTaraStop = 0;
		m_lSampleInterval = 0;
		m_fTaraWeightFifo.Init();

		TareStop();

		m_AdsClient.Init();

		m_DriveCtrl.Stop();
		m_WeightCtrl.InitExecute();
		m_WeightCtrl.Start(m_st);

		SetMassflow(0.0f);

		Dose_DSVSetWbfMinBeltLoadState(m_sID, base::eWbmState::WBFMINBELTLOAD_NOT_REACHED);
		Dose_DSVSetLocalModeState(m_sID, TRUE);

		m_LoadCell.SetPriority(m_sID, LC_PRIORITY::LC_PRIORITY_NORMAL);
	}
	return result;
}
//*****************************************************************************************************
//*****************************************************************************************************
BOOL CWbmControl :: ExitExecute ( void)
{
	auto result = CBaseControl::IsInit();
	if (result)
	{
		m_LoadCell.SetPriority(m_sID, LC_PRIORITY::LC_PRIORITY_NORMAL);
		m_WeightCtrl.Stop();
		m_WeightCtrl.ExitExecute();
		result = CBaseControl::ExitExecute();
	}
	return result;
}
//*****************************************************************************************************
//*****************************************************************************************************
BOOL CWbmControl :: Stop ( void)
{
	auto result = CBaseControl::IsStarted();
	if (result)
	{
        TareStop();

		m_DriveCtrl.Stop();
        SetMassflow (0.0f);

	    Dose_DSVSetWbfMinBeltLoadState	 (m_sID, base::eWbmState::WBFMINBELTLOAD_NOT_REACHED)	;

        m_aTotalizer.Close();

		m_LoadCell.SetPriority	  (m_sID, LC_PRIORITY::LC_PRIORITY_NORMAL);
    }
	return result;
}
//*****************************************************************************************************
//*****************************************************************************************************
BOOL CWbmControl :: Start ( void)
{
	auto result = CBaseControl :: Start();
    if (result)
    {
	    InitStart();

		m_DriveCtrl.Start(m_st);
		SetMassflow(0.0f);

	    GetBeltLoad();
		m_LoadCell.SetPriority (m_sID, LC_PRIORITY::LC_PRIORITY_HIGH);
    }
	return result;
}
//*****************************************************************************************************
//*****************************************************************************************************
BOOL CWbmControl ::GetRelease( void)
{
	// Freigabe
	BOOL bFreigabeOld		= FALSE;
	BOOL bFreigabe			= FALSE;

	Dose_DSVGetRelease				(m_sID, &bFreigabeOld)			;
	bFreigabe			= GetStart ();
	if ( bFreigabe != bFreigabeOld )
	{
		Dose_DSVSetRelease				(m_sID, bFreigabe) ;
	}
	return bFreigabe;
}
//*****************************************************************************************************
//*****************************************************************************************************
BOOL CWbmControl :: Execute ( void)
{
	//! aktuelle Zeit holen
	auto result = CBaseControl :: Execute();
	if (result)
	{
		//! aktuelles Gewicht holen
		GetBeltLoad();

		//! aktuellen InputFilter
		BOOL bChanged = GetInputFilter();
		if (bChanged)
		{
			m_bInputFilterChanged = TRUE;
		}
		//! SampleInterval
		bChanged = GetSampleInterval();
		if (bChanged)
		{
			m_bSampleIntervalChanged = TRUE;
		}

		// Freigabe
		BOOL bRelease = FALSE;
		Dose_DSVGetRelease(m_sID, &bRelease);

		//!--------------------------------------
		//! Tarierung testen
		BOOL bTarierung = GetTaring();
		if (bTarierung)
		{
			if (GetOperatingMode() != eOperatingMode::CALIBRATING)
			{
				Stop();
				Start();
				TareStart();
				SetOperatingMode(eOperatingMode::CALIBRATING);
			}
			TareRun();
			AlarmMonitoring();
			return TRUE;
		}

		//!--------------------------------------
		//! sonst Freigabe erfolgt ?
		if ( ! bRelease)
		{
			Stop();
			return FALSE;
		}

		//!--------------------------------------
		//! Dosierung kann laufen ...
		if (GetOperatingMode() != eOperatingMode::RUNNING)
		{
			Stop();
			Start();
			SetOperatingMode(eOperatingMode::RUNNING);
		}
		Control();
		AlarmMonitoring();
	}
	return result;
}


