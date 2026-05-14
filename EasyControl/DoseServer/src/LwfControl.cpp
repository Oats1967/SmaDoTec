//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Implementation of module LwfControl
///
/// @file   LwfControl.cpp
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
#include "LwfControl.h"
#include "Logger/include/Logger.h"



using namespace base;

#define RANGEDRIVECOMMAND(f) RANGE(f, 0.0, 1.0)



#define MAXPLAUSIBILITYCOUNTER		(2)
#define STARTLOOPCOUNTER            3
#define SET_MIN_DOSEPERFORMANCE(a) __max((a), 0.01);
#define RANGEDRIVECOMMAND(f) RANGE(f, 0.0, 1.0)
#define MAXREGELCOUNT           4
#define START_PLAUSIBILITY_RETRY_COUNTER      1
#define QMAXFILTER                      0.05

#define EDITITEM(_a, _func) 	 { _a, &CLwfControl::_func }

#if _DEBUG

static const char *szStep[12] = { "eIdle",
	"ePrepareStartupDelay",
	"eWaitStartupDelay",
	"eVolControl",
	"ePrepareDosing",
	"eCalcDosefactor",
	"eWaitStableMassflow",
	"eGravControl",
	"eWaitRefillFinished",
	"eWaitRefillStableMassflow",
	"eWaitFaultStableMassflow",
	"eForceVolumetric" };

#define LOGFUNCTION \
{\
	{ \
		LOGDEBUG(__FUNCTION__ << "(t = " << m_st << ") Step = " << szStep[_S32(m_ControlSteps)] <<  " Nom = " << m_fActualSetpoint << " Act = " << m_fMassflow << " Sample = " << _S32(m_tSampleInterval)); \
	}\
}

#define LOGFUNCTIONSTEP(__a) \
{\
	if (m_LastControlSteps != __a) \
	{ \
		m_LastControlSteps = __a; \
		LOGDEBUG(__FUNCTION__ << "(t = " << m_st << ") Step = " << szStep[_S32(m_LastControlSteps)] << " Nom = " << m_fActualSetpoint << " Act = " << m_fMassflow << " Sample = " << _S32(m_tSampleInterval));  \
	}\
}

#else
#define LOGFUNCTION
#define LOGFUNCTIONSTEP(__a)
#endif


//*********************************************************************************************
//*********************************************************************************************
CLwfControl::CLwfControl(int32_t s, loadcell::ILCModuleInterface& rlc) : CBaseClass{ s }
, c_EditMap({
		EDITITEM(eControlSteps::eIdle,						StepIdle),
		EDITITEM(eControlSteps::ePrepareStartupDelay,	    StepPrepareStartupDelay),
		EDITITEM(eControlSteps::eWaitStartupDelay,	        StepWaitStartupDelay),
		EDITITEM(eControlSteps::eVolControl,	            StepVolControl),
		EDITITEM(eControlSteps::ePrepareDosing,	            StepPrepareDosing),
		EDITITEM(eControlSteps::eCalcDosefactor,	        StepCalcDosefactor),
		EDITITEM(eControlSteps::eWaitStableMassflow,	    StepWaitStableMassflow),
		EDITITEM(eControlSteps::eGravControl,				StepGravControl),
		EDITITEM(eControlSteps::eWaitRefillFinished,		StepWaitRefillFinished),
		EDITITEM(eControlSteps::eWaitRefillStableMassflow,	StepWaitRefillStableMassflow),
		EDITITEM(eControlSteps::eWaitFaultStableMassflow,	StepWaitFaultStableMassflow),
		EDITITEM(eControlSteps::eForceVolumetric,			StepForceVolumetric),
	})
	, m_DriveCtrl(s)
	, m_AgitatorControl(s)
	, m_WeightCtrl(s)
	, m_MassflowAlarmCtrl(s)
	, m_DosePerformanceAlarmCtrl(s)
	, m_NoiseAlarmCtrl(s)
	, m_CalibTask(s)
	, m_TareTask(s)
	, m_EmptyFeederTask(s)
	, m_MeterCtrl(s)
	, m_RefillCtrl(s)
	, m_AdsClient(s)
	, m_MassflowCtrl()
	, m_fActualSetpoint(0.0f)
	, m_fMassflow(0.0f)
	, m_fMassflowGefiltert(0.0f)
	, m_fMassflowGleitend(0.0f)
	, m_fCurrentMassflow(0.0f)
	, m_fPidPropGainGross(0.0f)
	, m_fDosePerformance(0.0f)
	, m_dActualDoseperformance(0.0)
	, m_dLastDriveCommand(0.0)
	, m_fPidGatefilter(0.0f)
	, m_fMassflowFilter(0.0f)
	, m_fLastError(0.0f)
	, m_bLoadCellInitialized(FALSE)
	, m_bDosePerformanceChanged(FALSE)
	, m_bSetpointChanged(FALSE)
	, m_bPropGrossGainChanged(FALSE)
	, m_ControlSteps{ eControlSteps::eIdle }
	#if _DEBUG
	, m_bDosefaktorInitialized(FALSE)
	, m_LastControlSteps{ eControlSteps::eIdle }
	#endif
	, m_bDoseFaktorOK(FALSE)
	, m_zt0(0L)
	, m_tNext(0L)
	, m_tWeightNext(0L)
	, m_tSampleInterval(0L)
	, m_lControlCount(0)
	, m_bGatefilterModified(FALSE)
	, m_lPlausibilityRetryCounter(0L)
	, m_fMinDiff(0.0f)
	, m_lMaxCount(0)
	, m_bGateFilterHit{ FALSE }
{
	assert(c_EditMap.size() == _S32(eControlSteps::eMax));
	m_NoiseAlarmCtrl.registerAlarmManager(this->m_aAlarm);

	m_WeightCtrl.registerAlarmManager(this->m_aAlarm);
	m_WeightCtrl.registerLoadcell(rlc);

	m_DosePerformanceAlarmCtrl.registerAlarmManager(this->m_aAlarm);

	m_MassflowAlarmCtrl.registerAlarmManager(this->m_aAlarm);

	m_DriveCtrl.registerAlarmManager(this->m_aAlarm);

	m_RefillCtrl.registerAlarmManager(this->m_aAlarm);
	m_RefillCtrl.registerWeightCtrl(m_WeightCtrl);

	m_EmptyFeederTask.registerAlarmManager(this->m_aAlarm);
	m_EmptyFeederTask.registerDriveCtrl(m_DriveCtrl);
	m_EmptyFeederTask.registerWeightCtrl(m_WeightCtrl);

	m_TareTask.registerAlarmManager(this->m_aAlarm);
	m_TareTask.registerWeightCtrl(m_WeightCtrl);

	m_CalibTask.registerAlarmManager(this->m_aAlarm);
	m_CalibTask.registerDriveCtrl(m_DriveCtrl);
	m_CalibTask.registerWeightCtrl(m_WeightCtrl);
}
//*********************************************************************************************
//*********************************************************************************************
void CLwfControl::TotalizerSet(const float32_t fMassflow)
{
	if (m_aTotalizer.UpdateMassflow(m_st, fMassflow))
	{
		auto v = _F32(m_aTotalizer.GetTotalizer());
		Dose_DSVSetTotalizer(m_sID, _F32(m_aTotalizer.GetTotalizer()));
	}
}
//*********************************************************************************************
//*********************************************************************************************
BOOL CLwfControl ::  VerifySampleInterval (uint32_t lSampleInterval)
{
	if ( m_fActualSetpoint < 5.0f )
	{
		lSampleInterval = __max(4U, lSampleInterval) ;
	}
	else
	{
		lSampleInterval = RANGE(lSampleInterval, MIN_LWF_SAMPLETIME, MAX_LWF_SAMPLETIME);
	}
	BOOL bChanged = (lSampleInterval != m_tSampleInterval);
	m_tSampleInterval = lSampleInterval;
	return bChanged;
}
//*******************************************************************************************************
//*******************************************************************************************************
void CLwfControl::InitSampleInterval(void)
{
	uint32_t tSampleInterval = 0;
	Dose_DSVPopPidSampleInterval(m_sID, &tSampleInterval); // [0..1]
	(void)VerifySampleInterval(tSampleInterval);
}
//*******************************************************************************************************
//*******************************************************************************************************
BOOL CLwfControl::GetChangedSampleInterval(void)
{
	BOOL bChanged = FALSE;

	Dose_DSVGetChangedPidSampleInterval(m_sID, &bChanged); // [0..1]
	if (bChanged)
	{
		uint32_t tSampleInterval = 0;
		Dose_DSVPopPidSampleInterval(m_sID, &tSampleInterval); // [0..1]
		bChanged = VerifySampleInterval(tSampleInterval);
	}
	return bChanged;
}
//*********************************************************************************************
//*********************************************************************************************
BOOL CLwfControl :: GetGatefilter ( void)
{
	BOOL bChanged		= FALSE;

	Dose_DSVGetChangedPidGatefilter(m_sID, &bChanged);
	if ( bChanged )
	{
		Dose_DSVPopPidGatefilter		  (m_sID, &m_fPidGatefilter);
        m_fPidGatefilter /= 100.0f;
		m_aPlausibility.SetPidGatefilter(m_fPidGatefilter);
		m_bGatefilterModified = FALSE;
	}
	return bChanged;
}
//*********************************************************************************************
//*********************************************************************************************
BOOL CLwfControl ::GetDoseSetpoint( void)
{
	CBaseClass::GetLineSetpoint();

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
		const float32_t epsilon = (fActualSetpoint * (fSetpointLimit / 100.0f ) );

		bChanged = _F32(fabs(fSetpoint - fActualSetpoint)) > epsilon;
		if (bChanged)
		{
			m_fActualSetpoint = fSetpoint;
			Dose_DSVSetActualSetpoint(m_sID, fSetpoint);
		}
	}
	return bChanged;
}
//*********************************************************************************************
//*********************************************************************************************
BOOL CLwfControl :: GetMassflowfilter ( void)
{
	BOOL bChanged		= FALSE;

	Dose_DSVGetChangedMassflowFilter(m_sID, &bChanged);
	if ( bChanged )
	{
		Dose_DSVPopMassflowFilter(m_sID, &m_fMassflowFilter);
        m_fMassflowFilter     /= 100.0f;
        m_fMassflowFilter      = RANGE(m_fMassflowFilter, 0.0f, 1.0f);
		m_aPlausibility.SetMassflowFilter(m_fMassflowFilter);
	}
	return bChanged;
}
//*********************************************************************************************
//*********************************************************************************************
inline void CLwfControl :: 	SetMassflow (const float32_t fMassflow)
{
    m_fMassflow	= __max(fMassflow, 0.0f);
	m_AdsClient.SetMassflow(m_fMassflow);
}
//*********************************************************************************************
//*********************************************************************************************
inline void CLwfControl :: 	SetActualDosePerformance (const float32_t fQMaxfaktor)
{
	Dose_DSVSetActualDoseperformance	(m_sID, fQMaxfaktor);
}
//*********************************************************************************************
//*********************************************************************************************
inline void CLwfControl :: MassflowInit ( void)
{
	m_MassflowCtrl.Init();
	m_MassflowCtrl.Add(m_aLoadCell0);
}
//*********************************************************************************************
//*********************************************************************************************
inline void CLwfControl :: InitWeight ( void)
{
	const uint32_t t = __min(2U * m_tSampleInterval, 20U);
	m_tWeightNext   = m_st + t;
}
//*********************************************************************************************
//*********************************************************************************************
inline BOOL CLwfControl :: UpdateWeight ( void)
{
    BOOL bWeightUpdate = (m_aLoadCell0.m_ulT != m_aLoadCell1.m_ulT) && (m_st >= m_tWeightNext);
    if (bWeightUpdate)
    {
		assert(m_tSampleInterval >= 2);
		m_MassflowCtrl.Add(m_aLoadCell0);
        m_tWeightNext   = m_st + m_tSampleInterval;
        m_aLoadCell1    = m_aLoadCell0;
    }
    return bWeightUpdate;
}
//*********************************************************************************************
//*********************************************************************************************
inline BOOL CLwfControl :: UpdateMassflow ( void)
{
	float32_t temp = 0.0F;
	auto result = m_MassflowCtrl.GetMassflow(&temp);
	if (result)
	{
		m_fCurrentMassflow = temp;
	}
    return result;
}
//*********************************************************************************************
//*********************************************************************************************
inline BOOL CLwfControl::UpdateWeightAndMassflow(void)
{
	auto bMassflowUpdate = UpdateWeight();
	if (bMassflowUpdate)
	{
		bMassflowUpdate = UpdateMassflow();
	}
	return bMassflowUpdate;
}
//*********************************************************************************************
//*********************************************************************************************
inline BOOL CLwfControl::UpdateWeightAndMassflowAndPlausibility(void)
{
	auto bPlausibilityUpdate = UpdateWeightAndMassflow();
	if (bPlausibilityUpdate)
	{
		UpdatePlausibilityCheck();
	}
	return bPlausibilityUpdate;
}
//*********************************************************************************************
//*********************************************************************************************
void CLwfControl :: InitPlausibilityCheck ( void )
{
	m_aPlausibility.Init (m_fPidGatefilter, m_fMassflowFilter, m_fActualSetpoint, m_fCurrentMassflow);
	m_aPlausibility.SetMaxPlausibelCounter(MAXPLAUSIBILITYCOUNTER);
}
//*********************************************************************************************
//*********************************************************************************************
void CLwfControl :: UpdatePlausibilityCheck ( void)
{
    utils::NOISEINDEX aR;

	m_aPlausibility.Push  (m_fCurrentMassflow , aR);
    m_fMassflowGefiltert = aR.fMassflowGewichtet;
    m_fCurrentMassflow   = aR.fMassflow;
    m_fMassflowGleitend  = aR.fMassflowGleitend;
    
	Dose_DSVSetActualNoise(m_sID, aR.fNoiseIndex);
}
//*********************************************************************************************
//*********************************************************************************************
BOOL CLwfControl ::  GetPidPropGainGross (void)
{
	BOOL bChanged			= FALSE;

	Dose_DSVGetChangedPidPropGainGross		(m_sID, &bChanged); // [0..1]
	if ( bChanged ) 
	{
		Dose_DSVPopPidPropGainGross			(m_sID, &m_fPidPropGainGross); // [0..1]
		m_fPidPropGainGross /= 100.0f;
	}
	return bChanged;
}
//*********************************************************************************************
//*********************************************************************************************
BOOL CLwfControl :: GetDosePerformance ( void )
{
	BOOL bChanged = FALSE;

	Dose_DSVGetChangedCalibCfg	 (m_sID, &bChanged); // [kg/h]
	if (bChanged)
	{
		base::calib::CalibType cfg;
		Dose_DSVPopCalibCfg(m_sID, &cfg); // [kg/h]
		if (cfg.count > 0)
		{
			m_dActualDoseperformance = cfg.fDosePerformance[0];
			m_dActualDoseperformance = SET_MIN_DOSEPERFORMANCE(m_dActualDoseperformance);
			SetActualDosePerformance(_F32(m_dActualDoseperformance));
		}
	}
	return bChanged;
}
//*********************************************************************************************
//*********************************************************************************************
void CLwfControl :: GetWeight ( void)
{
	m_WeightCtrl.Update(m_st);
	m_aLoadCell0 = m_WeightCtrl.GetWeight();
}
//*********************************************************************************************
//*********************************************************************************************
void CLwfControl :: SetDriveCommand ( const float32_t drive )
{	
	assert(drive >= 0.0F && drive <= 1.0F);
	m_DriveCtrl.UpdateDriveCommand(drive);
}
//*********************************************************************************************
//*********************************************************************************************
void CLwfControl::SetDeltaDriveCommand(const float32_t delta)
{
	assert(m_dActualDoseperformance > 0.0);
	m_dLastDriveCommand += delta / m_dActualDoseperformance;
	m_dLastDriveCommand = RANGEDRIVECOMMAND(m_dLastDriveCommand);
	SetDriveCommand(_F32(m_dLastDriveCommand));
}
//*********************************************************************************************
//*********************************************************************************************
void CLwfControl :: DoseDriveCommandSet ( const float32_t fNomwert )
{
	if ( m_bSetpointChanged )
	{
		// Wenn der Dosierfaktor noch nicht berechnet wurde, dann Dosierperformanz vom Ursprungswert
		if ( ! m_bDoseFaktorOK)
		{
			float32_t fActualDoseperformance = 0.0F;
			Dose_DSVGetActualDoseperformance(m_sID, &fActualDoseperformance);
			m_dActualDoseperformance = SET_MIN_DOSEPERFORMANCE(fActualDoseperformance);
		}
		// Berechne neuen Stellbefehl in LwsControl Nomwert / m_dActualDoseperformance
		m_dLastDriveCommand = CalcDriveCommand(fNomwert);
		m_dLastDriveCommand = RANGEDRIVECOMMAND(m_dLastDriveCommand);

		SetDriveCommand(_F32(m_dLastDriveCommand));
		m_fCurrentMassflow = fNomwert;
		// Massflowpuffer loschen
		m_MassflowCtrl.Init();
		m_bSetpointChanged = FALSE;
	}
}
//*********************************************************************************************
//*********************************************************************************************
void CLwfControl::DosefactorSet(const float32_t fNomwert)
{
	if ( m_bDosePerformanceChanged )
	{
		// Berechne neuen Startdosierfaktor
		base::calib::CalibType cfg;
		Dose_DSVPopCalibCfg(m_sID, &cfg); // [kg/h]
		m_MeterCtrl.Init(cfg);

		m_dActualDoseperformance = m_MeterCtrl.CalcDosePerformance(fNomwert);
		m_dActualDoseperformance = SET_MIN_DOSEPERFORMANCE(m_dActualDoseperformance);
		SetActualDosePerformance(_F32(m_dActualDoseperformance));
		m_bDosePerformanceChanged = FALSE;
		m_bSetpointChanged = TRUE;
	}
}
//*********************************************************************************************
//*********************************************************************************************
BOOL CLwfControl::CheckTransition(void)
{
	BOOL bTransition = FALSE;

	if (m_bSetpointChanged || m_bDosePerformanceChanged)
	{
		StartWithSetpoint();
		bTransition = TRUE;
	}
	else
	{
		BOOL bForceVolGravState = m_RefillCtrl.isMinMinActive() || m_WeightCtrl.isLoadcellError();
		if (bForceVolGravState)
		{
			StartForceVolumetric();
			bTransition = TRUE;
		}
		else
		{
			BOOL bRefill = FALSE;
			Dose_DSVGetRefillState(m_sID, &bRefill);
			if (bRefill)
			{
				StartRefill();
				bTransition = TRUE;
			}
		}
	}
	return bTransition;
}
//*********************************************************************************************
//*********************************************************************************************
BOOL CLwfControl::CheckGravTransition(void)
{
	BOOL bTransition = CheckTransition();
	if (!bTransition)
	{
		BOOL bVolumetric = FALSE;
		Dose_DSVGetVolumetricState(m_sID, &bVolumetric);
		if (bVolumetric)
		{
			StartDosing();
			bTransition = TRUE;
		}
	}
	return bTransition;
}
//*********************************************************************************************
//*********************************************************************************************
BOOL CLwfControl::CheckVolTransition(void)
{
	BOOL bTransition = CheckTransition();
	if (!bTransition)
	{
		BOOL bVolumetric = FALSE;
		Dose_DSVGetVolumetricState(m_sID, &bVolumetric);
		if (!bVolumetric)
		{
			StartDosing();
			bTransition = TRUE;
		}
	}
	return bTransition;
}
//*********************************************************************************************
//*********************************************************************************************
void CLwfControl :: InitPrepareStartupDelay ( void)
{
	LOGFUNCTION

    float32_t fActualDoseperformance    = 0.0f;
    float32_t fActualDriveCommand   = 0.0f;

	m_bPropGrossGainChanged				= FALSE;
#if _DEBUG
    m_bDosefaktorInitialized            = FALSE;
	m_LastControlSteps = eControlSteps::eIdle;
#endif
	m_bDoseFaktorOK						= FALSE;
	m_bGatefilterModified               = FALSE;
	m_fMassflowGefiltert				= 0.0f;
	m_fCurrentMassflow					= 0.0f;
    m_lControlCount                     = 0L;
	m_tSampleInterval					= 0L;
	m_tWeightNext						= 0;

	Dose_DSVGetActualSetpoint(m_sID, &m_fActualSetpoint);
	Dose_DSVGetActualDoseperformance(m_sID, &fActualDoseperformance);
    m_dActualDoseperformance = SET_MIN_DOSEPERFORMANCE(fActualDoseperformance);
	Dose_DSVGetActualDriveCommand (m_sID, &fActualDriveCommand);
    m_dLastDriveCommand = _F64(fActualDriveCommand);

    // Control unit
	Dose_DSVGetPidPropGainGross(m_sID, &m_fPidPropGainGross);
	m_fPidPropGainGross /= 100.0f;

	Dose_DSVGetPidGatefilter(m_sID, &m_fPidGatefilter) ;
    m_fPidGatefilter /= 100.0f;
	Dose_DSVGetMassflowFilter(m_sID, &m_fMassflowFilter) ;
    m_fMassflowFilter     /= 100.0f;
    m_fMassflowFilter      = RANGE(m_fMassflowFilter, 0.0f, 1.0f);
    Dose_DSVSetGravVolState	(m_sID, FALSE )          ;

	Dose_DSVSetActualNoise(m_sID, 0.0F);
	Dose_DSVSetPidGatefilterState(m_sID, eGatefilter::GATEFILTER_NONE);

	InitSampleInterval();
	//InitWeight();		// Ist nicht notwendig
	m_MassflowCtrl.Init(); // Ist nicht notwendig, wird überschrieben in
    //MassflowInit();		// Ist nicht notwendig, wird überschrieben in

	base::calib::CalibType cfg;
	Dose_DSVPopCalibCfg(m_sID, &cfg); // [kg/h]
	m_MeterCtrl.Init(cfg);

	// Verbrauch
	float32_t fTotalizer = 0.0f;
	Dose_DSVGetTotalizer   (m_sID, &fTotalizer) ;
	m_aTotalizer.Open(m_st, fTotalizer, m_fActualSetpoint);

	m_ControlSteps = eControlSteps::ePrepareStartupDelay;

}
//*********************************************************************************************
//*********************************************************************************************
void CLwfControl :: StepPrepareStartupDelay ( void)
{
	LOGFUNCTIONSTEP(eControlSteps::ePrepareStartupDelay)

	uint32_t lDelay			= 0L;
	Dose_DSVGetStartupDelay(m_sID, &lDelay) ;
	if ( lDelay > 0L)
	{
		InitWaitStartupDelay();
	}
	else
	{
		InitLeistung ();
	}
}
//*********************************************************************************************
//*********************************************************************************************
void CLwfControl :: InitWaitStartupDelay ( void)
{
	LOGFUNCTION

	uint32_t lDelay		= 0L;
	Dose_DSVGetStartupDelay(m_sID, &lDelay) ;
	m_tNext	        = m_st+ lDelay;
	m_ControlSteps		    = eControlSteps::eWaitStartupDelay;
}
//*********************************************************************************************
//*********************************************************************************************
void CLwfControl :: StepWaitStartupDelay ( void)
{
	LOGFUNCTIONSTEP(eControlSteps::eWaitStartupDelay)
	if ( m_st >= m_tNext)
	{
		InitLeistung ();
	}
}
//*********************************************************************************************
//*********************************************************************************************
void CLwfControl :: InitLeistung( void )
{
	LOGFUNCTION
#if _DEBUG
	m_bDosefaktorInitialized = FALSE;
#endif
    m_bGatefilterModified     = FALSE;
	m_bSetpointChanged		  = TRUE;
	m_DriveCtrl.Start(m_st);
	StartWithSetpoint();
}
//*********************************************************************************************
//*********************************************************************************************
void CLwfControl :: StartWithSetpoint( void )
{
	LOGDEBUG(__FUNCTION__ << "Setpoint = " << m_fActualSetpoint);

    m_lPlausibilityRetryCounter    = START_PLAUSIBILITY_RETRY_COUNTER;

	DosefactorSet			(m_fActualSetpoint);
    DoseDriveCommandSet     (m_fActualSetpoint);
    SetMassflow				(m_fCurrentMassflow);
	TotalizerSet			(m_fCurrentMassflow);
	InitPrepareDosing();
}
//*********************************************************************************************
//*********************************************************************************************
void CLwfControl::InitPrepareDosing(void)
{
	m_tNext = m_st + 3U;
	m_ControlSteps = eControlSteps::ePrepareDosing;
	LOGFUNCTION
}
//*********************************************************************************************
//*********************************************************************************************
void CLwfControl::StepPrepareDosing(void)
{
	LOGFUNCTIONSTEP(eControlSteps::ePrepareDosing)

	TotalizerSet(m_fMassflow);

	if (m_st >= m_tNext)
	{
		StartDosing();
	}
}
//*********************************************************************************************
//*********************************************************************************************
void CLwfControl::StartDosing(void)
{
	LOGFUNCTION

	BOOL bVolumetric = FALSE;
	m_bPropGrossGainChanged = FALSE;

	Dose_DSVGetVolumetricState(m_sID, &bVolumetric);
	if (bVolumetric)
	{
		StartVolDosierung();
	}
	else
	{
		StartGravDosierung();
	}
}
//*********************************************************************************************
//*********************************************************************************************
void CLwfControl::StartGravDosierung(void)
{
	LOGFUNCTION

	if ( ! m_bDoseFaktorOK)
	{
		InitCalcDosefactor();   // Doseperformance ist noch nicht berechnet
	}
	else
	{
		InitWaitStableMassflow();   // Doseperformance ist berechnet => Warte auf stabile Messwerte
	}
}
//*********************************************************************************************
//*********************************************************************************************
void CLwfControl::StartVolumetric(void)
{
	LOGFUNCTION

	BOOL bVolumetric = FALSE;
	Dose_DSVGetVolumetricState(m_sID, &bVolumetric);
	if (!bVolumetric)
	{
		Dose_DSVSetGravVolState(m_sID, TRUE);
	}
}

//*********************************************************************************************
//*********************************************************************************************
void CLwfControl :: StartVolDosierung ( void)
{
	LOGFUNCTION

	InitVolControl();
}
//*********************************************************************************************
//*********************************************************************************************
void CLwfControl :: InitVolControl ( void)
{
	InitWeight();
	MassflowInit();
	InitPlausibilityCheck();
    m_aPlausibility.SetMassflow(m_fMassflow);
	m_ControlSteps	= eControlSteps::eVolControl;

	LOGFUNCTION
}
//*********************************************************************************************
//*********************************************************************************************
void CLwfControl :: StepVolControl ( void)
{
	LOGFUNCTIONSTEP(eControlSteps::eVolControl)

	TotalizerSet(m_fMassflow);

	auto bTransition = CheckVolTransition();
	if ( ! bTransition)
	{
		auto bPlausibilityUpdate = UpdateWeightAndMassflowAndPlausibility();
		if (bPlausibilityUpdate)
		{
			LOGFUNCTION

			if (m_aPlausibility.isNoError())
			{
				SetMassflow(m_fMassflowGleitend);
			}
		}
	}
}
//*********************************************************************************************
//*********************************************************************************************
void CLwfControl::InitCalcDosefactor(void)
{
	LOGFUNCTION

	//assert(m_bDosefaktorInitialized == FALSE);
	assert(!m_bDoseFaktorOK);

	MassflowInit();
	InitWeight();
	InitPlausibilityCheck();
	m_ControlSteps = eControlSteps::eCalcDosefactor;
}
//*********************************************************************************************
//*********************************************************************************************
void CLwfControl::StepCalcDosefactor(void)
{
	LOGFUNCTIONSTEP(eControlSteps::eCalcDosefactor)

	TotalizerSet(m_fMassflow);

	auto bTransition = CheckGravTransition();
	if (!bTransition)
	{
		auto bPlausibilityUpdate = UpdateWeightAndMassflowAndPlausibility();
		if (bPlausibilityUpdate)
		{
			LOGFUNCTION

			SetMassflow(m_fMassflowGleitend);
			if (m_aPlausibility.isNoError())
			{
				CalcDosefactorHandleSmallError();
			}
			else if (m_aPlausibility.isErrorLimitExceeded())
			{
				CalcDosefactorHandleLargeError();
			}
			else
			{}
		}
	}
}
//*********************************************************************************************
//*********************************************************************************************
void CLwfControl::CalcDosefactorHandleLargeError(void)
{
	const float32_t epsilon = 1e-06F;

	LOGFUNCTION

	m_dActualDoseperformance = SET_MIN_DOSEPERFORMANCE(m_dActualDoseperformance * FDIV0(m_fCurrentMassflow, m_fActualSetpoint));
	float64_t dDriveCommand = FDIV0(m_fActualSetpoint, m_dActualDoseperformance);
	dDriveCommand = RANGE(dDriveCommand, 0.0, 1.0);

	const float64_t dDifferenz = dDriveCommand - m_dLastDriveCommand;
	if (dDifferenz >= 0.0)
	{
		float32_t fMaxDriveCommand = 0.0F;

		Dose_DSVGetMaxDriveCommand(m_sID, &fMaxDriveCommand);
		fMaxDriveCommand = RANGE(fMaxDriveCommand, 1.0f, 50.0f);
		fMaxDriveCommand /= 100.0f;

		if (dDifferenz > fMaxDriveCommand)
		{
			m_dLastDriveCommand = RANGE(m_dLastDriveCommand + fMaxDriveCommand, 0.0, 1.0);
			m_dActualDoseperformance = SET_MIN_DOSEPERFORMANCE(FDIV0(m_fActualSetpoint, m_dLastDriveCommand));
		}
		else if (dDifferenz < 0.01)
		{
			m_dLastDriveCommand = RANGE(m_dLastDriveCommand + 0.01, 0.0, 1.0);
			m_dActualDoseperformance = SET_MIN_DOSEPERFORMANCE(FDIV0(m_fActualSetpoint, m_dLastDriveCommand));
		}
		else
		{
			m_dLastDriveCommand = dDriveCommand;
		}
	}
	else
	{
		m_dLastDriveCommand = dDriveCommand;
	}

	assert(m_dLastDriveCommand >= 0.0 && m_dLastDriveCommand <= 1.0);
	SetDriveCommand(_F32(m_dLastDriveCommand));
	// Achtung !!! SetActualDosePerformance(..) darf nicht aufgerufen werde, weil der Dosierfaktor erst übernommen wird, wenn gültig
	//SetActualDosePerformance(_F32(m_dActualDoseperformance));
	float32_t gatefilter = m_fPidGatefilter * 2.0f;
	gatefilter = RANGE(m_fPidGatefilter, 0.2F, 1.0F);
	if (_F32(fabs(m_fPidGatefilter - gatefilter)) > epsilon)
	{
		m_fPidGatefilter = gatefilter;
		m_bGatefilterModified = TRUE;
	}
	assert(!m_bDosefaktorInitialized);
	InitCalcDosefactor();   // Doseperformance ist noch nicht berechnet
}
//*********************************************************************************************
//*********************************************************************************************
void CLwfControl::CalcDosefactorHandleSmallError(void)
{
	LOGFUNCTION

	m_dActualDoseperformance	= SET_MIN_DOSEPERFORMANCE(m_dActualDoseperformance * FDIV0(m_fMassflowGefiltert, m_fActualSetpoint));
	SetActualDosePerformance(_F32(m_dActualDoseperformance));
	m_lPlausibilityRetryCounter = START_PLAUSIBILITY_RETRY_COUNTER;
	m_bDoseFaktorOK				= TRUE;
#if _DEBUG
	m_bDosefaktorInitialized	= TRUE;
#endif
	LOGDEBUG(__FUNCTION__ << "(t = " << m_st << ") Dosedactor successfully calculated =" << m_dActualDoseperformance);
	InitGravControl();
}
//*********************************************************************************************
//*********************************************************************************************
void CLwfControl :: InitWaitStableMassflow ( void)
{
	assert(m_bDoseFaktorOK);
	// Falls Sollwert geändert wurde, wurde Massflow Puffer geleert
	if (m_MassflowCtrl.isEmpty()) 
	{
		MassflowInit();
		InitWeight();
		InitPlausibilityCheck();
	}
	m_ControlSteps	= eControlSteps::eWaitStableMassflow;

	LOGFUNCTION
}
//*********************************************************************************************
//*********************************************************************************************
void CLwfControl :: StepWaitStableMassflow ( void)
{
	LOGFUNCTIONSTEP(eControlSteps::eWaitStableMassflow)

	TotalizerSet(m_fMassflow);

	auto bTransition = CheckGravTransition();
	if ( ! bTransition)
	{
		auto bPlausibilityUpdate = UpdateWeightAndMassflowAndPlausibility();
		if (bPlausibilityUpdate)
		{
			LOGFUNCTION

			SetMassflow(m_fMassflowGleitend);

			if ( m_aPlausibility.isNoError() )
			{
				InitGravControl();
			}
			else if ( m_aPlausibility.isErrorLimitExceeded() )
			{
				if (m_lPlausibilityRetryCounter)
				{
					m_lPlausibilityRetryCounter--;
					StartFault();
				}
				else
				{
					m_bDoseFaktorOK = FALSE;
					InitCalcDosefactor();
				}
			}
		}
	}
}
//*********************************************************************************************
//*********************************************************************************************
void CLwfControl :: InitGravControl ( void)
{
	static const float32_t fPidIntegralGain     = 0.00f;
	static const float32_t fDifferentialverstaerkung = 0.01f;
    

	// Ueberpruefen, ob grob oder Feinjustierung, bei Wechsel umschalten
	// Fehler berechnen Sollwert zum gefilterten Massflow
	const float32_t fError = m_fActualSetpoint - m_fMassflowGefiltert;
    m_fLastError = _F32(fabs(fError));

	// PD-Regler starten
	m_PidControl.Open (m_fPidPropGainGross, fPidIntegralGain, fDifferentialverstaerkung);

	// Akt. Fehler berechnen
	float32_t fD = m_PidControl.Start(m_st, fError);

	// Neuer Differenzwert
	SetDeltaDriveCommand(fD);

	m_bPropGrossGainChanged = FALSE;

    m_lControlCount = 0L;
	m_ControlSteps = eControlSteps::eGravControl;

	LOGFUNCTION
}
//*********************************************************************************************
//*********************************************************************************************
void CLwfControl :: StepGravControl ( void)
{
	LOGFUNCTIONSTEP(eControlSteps::eGravControl)

	TotalizerSet(m_fMassflow);

	auto bTransition = CheckGravTransition();
	if ( ! bTransition)
	{
		if ( m_bPropGrossGainChanged )
		{
			InitGravControl();
		}
		else
		{
			auto bPlausibilityUpdate = UpdateWeightAndMassflowAndPlausibility();
			if (bPlausibilityUpdate)
			{
				LOGFUNCTION
				// Gatefilter wurde vergroessert
				SetMassflow(m_fMassflowGleitend);

				if (m_aPlausibility.isNoError())
				{
					// Gatefilter wurde vergroessert
					if (m_bGatefilterModified)
					{
						float32_t fGatefilter = 0.0f;
						Dose_DSVGetPidGatefilter(m_sID, &fGatefilter);
						fGatefilter /= 100.0f;

						const float32_t fDiv = _F32(fabs(m_fCurrentMassflow / m_fActualSetpoint - 1.0f));
						if (fDiv < fGatefilter)
						{
							// Differenz kleiner => Gatefilter verkleinern
							m_fPidGatefilter = RANGE(fGatefilter, 0.0f, 1.0f);
							m_aPlausibility.SetPidGatefilter(m_fPidGatefilter);
							m_bGatefilterModified = FALSE;
						}
					}
					// Neuen Doseperformance berechnen
					const float64_t dActualDoseperformance = m_dActualDoseperformance;
					const float64_t dNewDosefactor = SET_MIN_DOSEPERFORMANCE(FDIV0(m_fMassflowGefiltert, m_dLastDriveCommand));
					// Neuberechneter Doseperformance geht mit QMAXFILTER ( 0.05) % ein.
					m_dActualDoseperformance = QMAXFILTER * dNewDosefactor + (1.0 - QMAXFILTER) * dActualDoseperformance;
					SetActualDosePerformance(_F32(m_dActualDoseperformance));

					const float32_t fError = m_fActualSetpoint - m_fMassflowGefiltert;
					BOOL bControlschritt = ((fError > 0.0f) && (fError > 0.8f * m_fLastError)) ||
						((fError < 0.0f) && (fError < m_fLastError)) ||
						(++m_lControlCount >= MAXREGELCOUNT);

					if (bControlschritt)
					{
						float32_t fD = m_PidControl.Update(m_st, fError);
						// New drive command
						SetDeltaDriveCommand(fD);
						m_fLastError = fError;
						m_lControlCount = 0L;
					}
				}
				else if (m_aPlausibility.isErrorLimitExceeded())
				{
					// Goto error mode
					StartFault();
				}
				else
				{}
			}
		}
	}
}
//*********************************************************************************************
//*********************************************************************************************
void CLwfControl :: StartRefill ( void)
{
	StartVolumetric();
    InitWaitRefillFinished();
}
//*********************************************************************************************
//*********************************************************************************************
void CLwfControl :: InitWaitRefillFinished ( void)
{
	m_ControlSteps	= eControlSteps::eWaitRefillFinished;

	LOGFUNCTION
}
//*********************************************************************************************
//*********************************************************************************************
void CLwfControl :: StepWaitRefillFinished ( void)
{
	LOGFUNCTIONSTEP(eControlSteps::eWaitRefillFinished)

	TotalizerSet(m_fMassflow);

	BOOL bRefill = FALSE;
	Dose_DSVGetRefillState(m_sID, &bRefill) ;
    const BOOL bForceVolGravState =   m_RefillCtrl.isMinMinActive() ||  m_WeightCtrl.isLoadcellError();
	BOOL bFinishStep			  =     bForceVolGravState      ||
										m_bSetpointChanged      || 
										( ! bRefill )			||
										m_bDosePerformanceChanged;
    if ( ! bFinishStep)
    {
        auto bWeightUpdate = UpdateWeight();
        if ( bWeightUpdate )
        {
			LOGFUNCTION
        }
	}
	else // bFinishStep = TRUE
	{
		FinishRefill();
	}
}
//*********************************************************************************************
//*********************************************************************************************
void CLwfControl::FinishRefill(void)
{
	LOGFUNCTION

	BOOL bRefill = FALSE;
	Dose_DSVGetRefillState(m_sID, &bRefill);
	if ( ! bRefill)
	{
		StartRefillStableMassflow();
	}
	else
	{
		FinishVolumetric();
	}
}
//*********************************************************************************************
//*********************************************************************************************
void CLwfControl::StartRefillStableMassflow(void)
{
	InitWaitRefillStableMassflow();
}

//*********************************************************************************************
//*********************************************************************************************
void CLwfControl :: InitWaitRefillStableMassflow ( void)
{
	uint32_t lT = 0L;
	Dose_DSVGetRefillSwitchDelay	(m_sID, &lT)            ;
    m_zt0				= m_st + lT;
	m_fMinDiff			= FLT_MAX;
	m_lMaxCount			= 0L;
	m_bGateFilterHit	= FALSE;
	m_MassflowCtrl.SetBufferSize(4);
	m_MassflowCtrl.Init(); // Falls noch etwas nachfällt
	m_ControlSteps	= eControlSteps::eWaitRefillStableMassflow;
	LOGFUNCTION
}
//*********************************************************************************************
//*********************************************************************************************
void CLwfControl::StepWaitRefillStableMassflow(void)
{
	LOGFUNCTIONSTEP(eControlSteps::eWaitRefillStableMassflow)

	TotalizerSet(m_fMassflow);

	BOOL bRefill = FALSE;
	Dose_DSVGetRefillState(m_sID, &bRefill);
	const BOOL bForceVolGravState = m_RefillCtrl.isMinMinActive() || m_WeightCtrl.isLoadcellError();
	BOOL bFinishStep = bForceVolGravState || m_bSetpointChanged || bRefill || m_bDosePerformanceChanged;
	if ( ! bFinishStep)
	{
		auto bMassflowUpdate = UpdateWeightAndMassflow();
		if (bMassflowUpdate)
		{
			LOGFUNCTION

			const float32_t dF = _F32(fabs(m_fCurrentMassflow - m_fActualSetpoint));
			if (dF < m_fMinDiff)
			{
				m_fMinDiff = dF;
				m_lMaxCount = 0;
				// abnehmende Differenz
				if (m_fMinDiff < m_fPidGatefilter * m_fActualSetpoint)
				{
					m_bGateFilterHit = TRUE;
				}
			}
			else
			{
				m_lMaxCount++;
				if (m_bGateFilterHit)
				{
					if (m_st > m_zt0) // Refill switch delay expired
					{
						bFinishStep = TRUE;
					}
				}
				else
				{
					if (m_lMaxCount > 10)
					{
						bFinishStep = TRUE;
					}
				}
			}
		}
	}
	if (bFinishStep)
	{
		FinishRefillStableMassflow();
	}
}
//*********************************************************************************************
//*********************************************************************************************
void CLwfControl::FinishRefillStableMassflow(void)
{
	LOGFUNCTION

	FinishVolumetric();
}
//*********************************************************************************************
//*********************************************************************************************
void CLwfControl :: StartFault ( void)
{
	StartVolumetric();
    InitWaitFaultStableMassflow();
}
//*********************************************************************************************
//*********************************************************************************************
void CLwfControl :: InitWaitFaultStableMassflow ( void)
{
	m_fMinDiff = FLT_MAX;
	m_lMaxCount = 0L;
	m_bGateFilterHit = FALSE;
	m_MassflowCtrl.SetBufferSize(4);
	assert(!m_MassflowCtrl.isEmpty());
	m_ControlSteps	= eControlSteps::eWaitFaultStableMassflow;

	LOGFUNCTION
}
//*********************************************************************************************
//*********************************************************************************************
void CLwfControl :: StepWaitFaultStableMassflow ( void)
{
	LOGFUNCTIONSTEP(eControlSteps::eWaitFaultStableMassflow)

	TotalizerSet(m_fMassflow);

	BOOL bVolumetric  = FALSE;
    BOOL bRefill   = FALSE;

	Dose_DSVGetVolumetricState(m_sID, &bVolumetric);
    Dose_DSVGetRefillState(m_sID, &bRefill);
    const BOOL bForceVolGravState =   m_RefillCtrl.isMinMinActive() || m_WeightCtrl.isLoadcellError();
	BOOL bFinishStep = bRefill || 
					   bVolumetric ||
                       bForceVolGravState      || 
                       m_bSetpointChanged      || 
                       m_bDosePerformanceChanged;
	if ( ! bFinishStep)
	{
		auto bPlausibilityUpdate = UpdateWeightAndMassflowAndPlausibility();
		if (bPlausibilityUpdate)
		{
			LOGFUNCTION

			// Tendenz fallend
			const float32_t dF = _F32(fabs(m_fCurrentMassflow - m_fActualSetpoint));
			if (dF < m_fMinDiff)
			{
				m_fMinDiff = dF;
				m_lMaxCount = 0;
				// abnehmende Differenz
				if (m_fMinDiff < m_fPidGatefilter * m_fActualSetpoint)
				{
					m_bGateFilterHit = TRUE;
				}
			}
			else
			{
				if (m_bGateFilterHit )
				{
					bFinishStep = TRUE;
				}
				else
				{
					m_lMaxCount++;
					if (m_lMaxCount > 3)
					{
						SetMassflow(m_fCurrentMassflow);
						if (m_lMaxCount > 5)
						{
							bFinishStep = TRUE;
						}
					}
				}
			}
		}
    }
	if ( bFinishStep )
	{
		FinishFault();
	}
}
//*********************************************************************************************
//*********************************************************************************************
void CLwfControl :: FinishFault ( void)
{
	LOGFUNCTION

	if ( ! m_aPlausibility.isNoError())
	{
		LOGDEBUG("m_MassflowCtrl.Init()");
		m_MassflowCtrl.Init();
	}
	FinishVolumetric();
}
//*********************************************************************************************
//*********************************************************************************************
void CLwfControl::FinishVolumetric(void)
{
	Dose_DSVSetGravVolState(m_sID, FALSE);
	//m_aPlausibility.SetMassflow(m_fMassflow);
	m_MassflowCtrl.SetBufferSize();
	//assert(!m_MassflowCtrl.isEmpty());

	BOOL bTransition = CheckTransition();
	if ( ! bTransition)
	{
		StartDosing();
	}
}

//*********************************************************************************************
//*********************************************************************************************
void CLwfControl :: StartForceVolumetric ( void)
{
	StartVolumetric();
    InitForceVolumetric();
}
//*********************************************************************************************
//*********************************************************************************************
void CLwfControl :: InitForceVolumetric ( void)
{
	m_ControlSteps	= eControlSteps::eForceVolumetric;
	LOGFUNCTION
}
//*********************************************************************************************
//*********************************************************************************************
void CLwfControl :: StepForceVolumetric ( void)
{
	LOGFUNCTIONSTEP(eControlSteps::eForceVolumetric)

	TotalizerSet(m_fMassflow);

    BOOL bRefill   = FALSE;

    Dose_DSVGetRefillState(m_sID, &bRefill) ;
	//Dose_DSVGetVolumetricState(m_sID, &bVolumetric) ;
    const BOOL bForceVolGravState =     m_RefillCtrl.isMinMinActive() || m_WeightCtrl.isLoadcellError();
	BOOL bFinishStep =  /*bVolumetric || */			// Im volumetrischen Betrieb würde sofort wieder in ForceVolumetrisch geschaltet werden
                        m_bSetpointChanged          || 
                        m_bDosePerformanceChanged   || // weil Stellbefehl sich ändert
/*
						Wenn unter MinMIn-Pegel, dann im volumetrisch bleiben auch wenn noch refill aktiv ist.
						Wenn MinMIn überschritten wird, dann findet Übergang nach StartRefill statt
                        bRefill				        ||
*/
                        ( ! bForceVolGravState );
	if ( ! bFinishStep)
	{
		auto bPlausibilityUpdate = UpdateWeightAndMassflowAndPlausibility();
		if ( bPlausibilityUpdate )
		{
			LOGFUNCTION
			if (m_aPlausibility.isNoError())
			{
				SetMassflow(m_fMassflowGleitend);
			}
			else if (m_aPlausibility.isErrorLimitExceeded())
			{
				SetMassflow(m_fCurrentMassflow);		// Aktuell berechnete Leistung
			}
		}
	}
	else // if (bFinishStep)
	{
		FinishForceVolumetric();
	}
}
//*********************************************************************************************
//*********************************************************************************************
void CLwfControl :: FinishForceVolumetric ( void)
{
	LOGFUNCTION

	FinishVolumetric();
}
//*********************************************************************************************
//*********************************************************************************************
void	CLwfControl :: Control()
{
	// assert(m_WeightCtrl.GetPriority() == base::LC_PRIORITY::LC_PRIORITY_HIGH);
	if (m_WeightCtrl.GetPriority() != base::LC_PRIORITY::LC_PRIORITY_HIGH)
	{
		m_WeightCtrl.SetPriority(base::LC_PRIORITY::LC_PRIORITY_HIGH);
	}

	m_RefillCtrl.CheckRefill();

	try
	{
		(this->*c_EditMap.at(m_ControlSteps))();
	}
	catch (std::out_of_range)
	{
		assert(FALSE);
		LOGERROR("Error not found in map : " << _S32(m_ControlSteps));
	}
	m_MassflowAlarmCtrl.Update(m_st);
	m_DosePerformanceAlarmCtrl.Update(m_st);
	m_NoiseAlarmCtrl.Update(m_st);
	m_DriveCtrl.Update(m_st);
	CheckExtAlarm();
}
//*********************************************************************************************
//*********************************************************************************************
void CLwfControl :: ClearAlarm   (const base::eAlarmError lError)
{
    CBaseClass :: ClearAlarm (lError);
    m_RefillCtrl.ClearAlarm (lError);
}
//*********************************************************************************************
//*********************************************************************************************
BOOL CLwfControl :: Stop ( void)
{
	auto result = CBaseClass::IsStarted();
    if (result)
    {
		LOGFUNCTION

		TotalizerSet(m_fMassflow);
		m_aTotalizer.Close();
	    m_aPlausibility.Close();

		m_DriveCtrl.Stop();
		m_MassflowAlarmCtrl.Stop();
		m_NoiseAlarmCtrl.Stop();
		m_DosePerformanceAlarmCtrl.Stop();

		SetMassflow(0.0f);
		m_WeightCtrl.SetPriority(LC_PRIORITY::LC_PRIORITY_NORMAL);
		m_ControlSteps = eControlSteps::eIdle;
		Dose_DSVPBSetPBRefillStop(m_sID);
		result = CBaseClass::Stop();
	}
	return result;
}
//*********************************************************************************************
//*********************************************************************************************
BOOL CLwfControl :: Start(const uint32_t t)
{
 	auto result = CBaseClass :: Start(t);
    if (result)
    {
		LOGFUNCTION

		InitPrepareStartupDelay();

		//m_DriveCtrl.Start(m_st);
        SetMassflow (0.0f);

        m_aPlausibility.Open();
		assert(m_RefillCtrl.IsStarted());
		m_WeightCtrl.SetPriority(LC_PRIORITY::LC_PRIORITY_HIGH);

		m_MassflowAlarmCtrl.Start(m_st);
		m_NoiseAlarmCtrl.Start(m_st);
		m_DosePerformanceAlarmCtrl.Start(m_st);
    }
	return result;
}
//*********************************************************************************************
//*********************************************************************************************
BOOL CLwfControl::GetAgitator()
{
	return m_AgitatorControl.Update(m_st);
}
//*********************************************************************************************
//*********************************************************************************************
BOOL CLwfControl::GetVolumetric(void) 
{
	auto bLoadCellInitialized = m_WeightCtrl.isLoadcellInitialized();
	if ( bLoadCellInitialized )
	{
		if (m_WeightCtrl.isLoadcellEnabled())
		{
			if ( ! m_bLoadCellInitialized)
			{
				m_bLoadCellInitialized = TRUE;
				BOOL bVolumetric = FALSE;
				Dose_DSVGetVolumetricState(m_sID, &bVolumetric);
				if (bVolumetric)
				{
					Dose_DSVPBSetPBGravimetric(m_sID);
				}
			}
		}
		else
		{
			BOOL bVolumetric = FALSE;
			Dose_DSVGetVolumetricState(m_sID, &bVolumetric);
			if ( ! bVolumetric)
			{
				Dose_DSVPBSetPBVolumetric(m_sID);
			}
		}
	}
	return CBaseClass::GetVolumetric();
}
//*********************************************************************************************
//*********************************************************************************************
BOOL CLwfControl::ExitExecute(void)
{
	LOGFUNCTION
	auto result = CBaseClass::IsInit();
	if ( result )
	{
		Stop();
		m_RefillCtrl.ExitExecute();
		m_WeightCtrl.SetPriority(base::LC_PRIORITY::LC_PRIORITY_LOW);
		m_WeightCtrl.Stop();
		m_WeightCtrl.ExitExecute();
		m_DriveCtrl.ExitExecute();
		m_MassflowAlarmCtrl.ExitExecute();
		m_NoiseAlarmCtrl.ExitExecute();
		m_DosePerformanceAlarmCtrl.ExitExecute();
		m_aPlausibility.Close();
		m_AgitatorControl.ExitExecute();
		m_EmptyFeederTask.ExitExecute();
		m_AdsClient.Exit();
		result = CBaseClass::ExitExecute();
	}
	return result;
}
//*********************************************************************************************
//*********************************************************************************************
BOOL CLwfControl::InitExecute(void)
{
	auto result = CBaseClass::InitExecute();
	if ( result)
	{
		LOGFUNCTION
		m_AdsClient.Init();
		m_EmptyFeederTask.InitExecute();
		m_RefillCtrl.InitExecute();
		m_RefillCtrl.Start(m_st);
		m_fActualSetpoint = 0.0f;
		m_fMassflow = 0.0f;
		m_fMassflowGefiltert = 0.0f;
		m_fMassflowGleitend = 0.0f;
		m_fCurrentMassflow = 0.0f;
		m_fDosePerformance = 0.0f;
		m_fPidPropGainGross = 0.0f;
		m_fPidGatefilter = 0.0f;
		m_fMassflowFilter = 0.0f;
		m_fLastError = 0.0f;
		m_lPlausibilityRetryCounter = START_PLAUSIBILITY_RETRY_COUNTER;
		m_dActualDoseperformance = 0.0;
		m_dLastDriveCommand = 0.0;
		m_lControlCount = 0L;
		m_bDosePerformanceChanged = FALSE;
		m_bSetpointChanged = FALSE;
		m_bPropGrossGainChanged = FALSE;
		m_ControlSteps = eControlSteps::eIdle;
		m_bLoadCellInitialized = FALSE;
#if _DEBUG
		m_bDosefaktorInitialized = FALSE;
#endif
		m_bGatefilterModified = FALSE;

		m_zt0 = 0L;
		m_tNext = 0L;
		m_tWeightNext = 0;
		m_tSampleInterval = 0;
		m_aPlausibility.SetMaxPlausibelCounter(MAXPLAUSIBILITYCOUNTER);

		m_DriveCtrl.InitExecute();
		m_AgitatorControl.InitExecute();
		m_WeightCtrl.InitExecute();
		m_WeightCtrl.Start(m_st);
		m_MassflowAlarmCtrl.InitExecute();
		m_NoiseAlarmCtrl.InitExecute();
		m_DosePerformanceAlarmCtrl.InitExecute();
		SetMassflow(0.0f);
		Dose_DSVSetActualNoise(m_sID, 0.0f);
		m_WeightCtrl.SetPriority(base::LC_PRIORITY::LC_PRIORITY_NORMAL);
	}
	return result;
}
//*********************************************************************************************
//*********************************************************************************************
BOOL CLwfControl :: Execute ( void)
{
	assert(IsInit());

	// aktuelle Zeit holen
	auto result = CBaseClass :: Execute();
	if (result)
	{
		// Gewicht holen
		GetWeight();

		// Gatefilter holen
		GetGatefilter();

		// Massflowfilter
		GetMassflowfilter();

		// Rührwerk
		GetAgitator();

		// aktuellen Sollwert holen
		auto bChanged = GetDoseSetpoint();
		if (bChanged)
		{
			m_bSetpointChanged = TRUE;
		}

		// SampleInterval
		(void)GetChangedSampleInterval();

		// Proportional Grobverstaerkung
		bChanged = GetPidPropGainGross();
		if (bChanged)
		{
			m_bPropGrossGainChanged = TRUE;
		}

		// aktuellen Maxdosierfaktor holen
		bChanged = GetDosePerformance();
		if (bChanged)
		{
			m_bDosePerformanceChanged = TRUE;
		}

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
				if ( ! bRelease)
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

			case eOperatingMode::TARING:
			{
				BOOL bTarierung = GetTaring();
				if (bTarierung)
				{
					m_TareTask.Update(m_st);
				}
				else
				{
					m_TareTask.Stop();
					m_TareTask.ExitExecute();
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
				//--------------------------------------
				// Tarierung testen
				BOOL bTarierung = GetTaring();
				if (bTarierung)
				{
					m_TareTask.InitExecute();
					m_TareTask.Start(m_st);
					SetOperatingMode(eOperatingMode::TARING);
				}
				else
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
							if (!bRelease)
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
			}
			break;
		}
		SetAlarmOutput();
	}
	return result;
}


