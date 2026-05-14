//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Implementation of module BaseControl
///
/// @file   BaseControl.cpp
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
#include "BaseControl.h"
#include "DoseDataLib/include/DoseData.h"
#include "BASE/Base.def"


using namespace base;


//******************************************************************************************************
//******************************************************************************************************
CBaseControl::CBaseControl(int32_t id) :	CTimerStatemachine(id)
, m_aAlarm(id)
, m_AdsClient(id)
, m_aTotalizer()
, m_bDoseIOStartInputLast(FALSE)
, m_bLineStartedLast{ FALSE }
, m_bEmptyFeederIOStartInputLast{ FALSE }
, m_bEmptyFeederIOStopInputLast{ FALSE }
, m_bIOVolumetricLast{ FALSE }
{
	assert(id >= 0);
}
//*********************************************************************************************
//*********************************************************************************************
void CBaseControl::SetAlarmOutput(void)
{
	BOOL bChanged = FALSE;

	Dose_DSVGetChangedIOAlarmOutput(m_sID, &bChanged);
	if (bChanged)
	{
		BOOL bExternAlarm = FALSE;
		Dose_DSVPopIOAlarmOutput(m_sID, &bExternAlarm);
		m_AdsClient.SetAlarm(bExternAlarm);
	}
}
//*********************************************************************************************
//*********************************************************************************************
void CBaseControl::CheckExtAlarm()
{
	BOOL	bAlarm = FALSE;

	Dose_DSVGetIOExtAlarmInput(m_sID, &bAlarm);
	if (bAlarm)
	{
		if ( ! m_aAlarm.IsAlarm(eAlarmError::ERROR_DOSE_EXTALARM))
		{
			m_aAlarm.SetAlarm(m_st, eAlarmError::ERROR_DOSE_EXTALARM, bAlarm, eAlarmClass::eALARMTYP);
		}
	}
	m_aAlarm.CheckWarningLevel(eAlarmError::ERROR_DOSE_EXTALARM);
}
//******************************************************************************************************
//******************************************************************************************************
BOOL CBaseControl :: GetLineSetpoint ( void ) const
{
	const float32_t epsilon = 1e-04F;

	BOOL	bLocalMode		    = FALSE;
    BOOL    bLokalChanged       = FALSE;

	Dose_DSVGetLocalModeState   (m_sID, &bLocalMode);
	if ( ! bLocalMode )
	{
		float32_t   fSetpoint = 0.0f;
		BOOL    bRegenerat = FALSE;
		Dose_DSVGetRegenerat(m_sID, &bRegenerat);
        if ( bRegenerat )
        {
		    Dose_EXGetLineSetpoint(&fSetpoint);
        }
        else
        {
		    Dose_EXGetRecipeSetpoint(&fSetpoint);
        }
		float32_t   fMischung = 0.0f;
		Dose_DSVGetPercentage(m_sID, &fMischung);
		const float32_t fNewSetpoint = fSetpoint * (fMischung / 100.0f);

		float32_t   fActualSetpoint = FALSE;
		Dose_DSVGetActualSetpoint(m_sID, &fActualSetpoint);
        if ( _F32(fabs(fActualSetpoint - fNewSetpoint)) > epsilon)
        {
		    Dose_DSVSetNominalSetpoint(m_sID, fNewSetpoint);
#if _DEBUG
			{
				BOOL bChanged = FALSE;
				Dose_DSVGetChangedNominalSetpoint(m_sID, &bChanged);
				assert(bChanged);
			}
#endif
			Dose_DSVSetChangedNominalSetpoint(m_sID, TRUE);
            bLokalChanged = TRUE;
        }
    }
    return bLokalChanged;
}
//******************************************************************************************************
//******************************************************************************************************
BOOL CBaseControl :: GetDoseSetpoint ( void )
{
	const float32_t epsilon = 1e-04F;

	GetLineSetpoint();

	BOOL bChanged = FALSE;
	Dose_DSVGetChangedNominalSetpoint(m_sID, &bChanged);
	if (bChanged)
	{
		float32_t fSetpoint = 0.0F;
		Dose_DSVPopNominalSetpoint(m_sID, &fSetpoint);
		float32_t fActualSetpoint = 0.0F;
		Dose_DSVGetActualSetpoint(m_sID, &fActualSetpoint);
		bChanged = _F32(fabs(fSetpoint - fActualSetpoint)) > epsilon;
		if (bChanged)
		{
			Dose_DSVSetActualSetpoint(m_sID, fSetpoint);
		}
	}
	return bChanged;
}
//******************************************************************************************************
//******************************************************************************************************
BOOL CBaseControl :: GetCalibration ( void) const
{
	BOOL bCalibration			= FALSE;

	Dose_DSVGetCalibrationState	(m_sID, &bCalibration);
	if ( ! bCalibration )
	{
		BOOL bChanged = FALSE;
		Dose_DSVPBGetPBCalibStart	(m_sID, &bChanged);
		if ( bChanged )
		{
			bCalibration = TRUE;
		}
	}
	else
	{
		BOOL bChanged = FALSE;
		Dose_DSVPBGetPBCalibStop  (m_sID, &bChanged)        ;
		if ( bChanged )
		{
			bCalibration = FALSE;
		}
	}
	Dose_DSVPBResetPBCalibStart(m_sID);
	Dose_DSVPBResetPBCalibStop(m_sID);
	return bCalibration;
}
//******************************************************************************************************
//******************************************************************************************************
BOOL CBaseControl :: GetTaring ( void) const
{
	BOOL bTarierung		 = FALSE;

	Dose_DSVGetTaraState (m_sID, &bTarierung) ;
	if ( ! bTarierung )
	{
		BOOL bChanged = FALSE;
		Dose_DSVPBGetPBTareStart(m_sID, &bChanged) ;
		if ( bChanged )
		{
			bTarierung = TRUE;
		}
	}
	else
	{
		BOOL bChanged = FALSE;
		Dose_DSVPBGetPBTareStop(m_sID, &bChanged) ;
		if (bChanged)
		{
			bTarierung = FALSE;
		}
	}
	Dose_DSVPBResetPBTareStart(m_sID);
	Dose_DSVPBResetPBTareStop(m_sID);
	return bTarierung;
}
//******************************************************************************************************
//******************************************************************************************************
BOOL CBaseControl::GetEmptyFeeder(void)
{
	BOOL bEmptyFeeder		= FALSE;
	Dose_DSVGetEmptyFeederState(m_sID, &bEmptyFeeder);
	if ( ! bEmptyFeeder)
	{
		BOOL bEmptyFeederIOStartInput = FALSE;
		Dose_DSVGetIOEmptyFeederStartInput(m_sID, &bEmptyFeederIOStartInput);
		if (bEmptyFeederIOStartInput || (bEmptyFeederIOStartInput != m_bEmptyFeederIOStartInputLast))
		{
			m_bEmptyFeederIOStartInputLast = bEmptyFeederIOStartInput;
			bEmptyFeeder				   = bEmptyFeederIOStartInput;
		}
		else
		{
			BOOL bChanged = FALSE;
			Dose_DSVPBGetPBEmptyFeederStart(m_sID, &bEmptyFeeder);
			if (bChanged)
			{
				bEmptyFeeder = TRUE;
			}
		}
	}
	else
	{
		BOOL bEmptyFeederIOStopInput = FALSE;
		Dose_DSVGetIOEmptyFeederStopInput(m_sID, &bEmptyFeederIOStopInput);
		if (bEmptyFeederIOStopInput || (bEmptyFeederIOStopInput != m_bEmptyFeederIOStopInputLast))
		{
			m_bEmptyFeederIOStopInputLast = bEmptyFeederIOStopInput;
			bEmptyFeeder				  = !bEmptyFeederIOStopInput;
		}
		else
		{
			BOOL bChanged = FALSE;
			Dose_DSVPBGetPBEmptyFeederStop(m_sID, &bChanged);
			if (bChanged)
			{
				bEmptyFeeder = FALSE;
			}
		}
	}
	Dose_DSVPBResetPBEmptyFeederStart(m_sID);
	Dose_DSVPBResetPBEmptyFeederStop(m_sID);
#if 0 // Do not set empty feeder state because it's set after stop
	if ( bEmptyFeeder != bEmptyFeederOld)
	{
		Dose_DSVSetEmptyFeederState(m_sID, bEmptyFeeder);

	}
#endif
	return bEmptyFeeder;
}
//******************************************************************************************************
//******************************************************************************************************
BOOL CBaseControl ::  GetClearTotalizer(void)
{
    BOOL bChanged   = FALSE;

	Dose_DSVPBGetPBClearTotalizer (m_sID,  &bChanged ) ;
    if ( bChanged )
    {
	    Dose_DSVPBResetPBClearTotalizer (m_sID);
        ClearTotalizer();
    }
	return bChanged;
}
//******************************************************************************************************
//******************************************************************************************************
void CBaseControl :: ClearTotalizer   (void)
{
	Dose_DSVInitTotalizer (m_sID, 0.0f) ;
    m_aTotalizer.Nullen(m_st);
}
//******************************************************************************************************
//******************************************************************************************************
BOOL CBaseControl :: GetClearAlarm ( void)
{
	BOOL bClearAlarm		= FALSE;
	BOOL bChanged			= FALSE;
    BOOL bAlarm03Quit       = FALSE;

    Dose_EXGetAlarm03QuitState  (&bAlarm03Quit) ;
    if ( bAlarm03Quit )
    {
	    bClearAlarm = TRUE;
    }
    else
    {
	    Dose_DSVPBGetPBClearAlarm	(m_sID, &bChanged);
	    if ( bChanged )
	    {
			Dose_DSVPBResetPBClearAlarm(m_sID);
			bClearAlarm = TRUE;
	    }
        else
        {
	        Dose_DSVPBGetPBAcknowledgeAlarm	(m_sID, &bChanged) ;
	        if ( bChanged )
	        {
		        Dose_DSVPBResetPBAcknowledgeAlarm (m_sID);
                bClearAlarm = TRUE;
	        }
            else
            {
				// Einzelalarm
                Dose_DSVIsAnyClearAlarm (m_sID, &bChanged);
                if ( bChanged )
                {
					base::eAlarmError lAlarm;
					Dose_DSVGetClearAlarm         (m_sID, &lAlarm) ;
                    ClearAlarm(lAlarm);
	                Dose_DSVDeleteClearAlarm      (m_sID, lAlarm);
                }
                else
                {
                    Dose_DSVIsAnyClearWarning        (m_sID, &bChanged)   ;
                    if ( bChanged )
                    {
						base::eAlarmError lWarning;
						Dose_DSVGetClearWarning      (m_sID, &lWarning)   ;
                        ClearAlarm(lWarning);
                        Dose_DSVDeleteClearWarning   (m_sID, lWarning)               ;
                    }
                }
            }
        }
    }
    if ( bClearAlarm )
    {
        ClearAlarm(eAlarmError::ERROR_MAX);
    }
	return bClearAlarm;
}
//******************************************************************************************************
//******************************************************************************************************
BOOL CBaseControl :: GetAnlagenModus ( void)
{
	BOOL bLocalMode		= FALSE;
	BOOL bModusChanged = FALSE;

    // Noch Alarmclass informieren
	Dose_DSVGetLocalModeState(m_sID, &bLocalMode);
	BOOL bLocalModeOld = bLocalMode;

	BOOL bSlaveMode = FALSE;
	Dose_DSVGetSlaveMode(m_sID, &bSlaveMode);
	if (bSlaveMode)
	{
		bLocalMode = TRUE;
	}
	else
	{
		if (bLocalMode)
		{
			BOOL bChanged = FALSE;
			Dose_DSVPBGetPBInline(m_sID, &bChanged);
			if (bChanged)
			{
				bLocalMode = FALSE;
			}
		}
		else
		{
			BOOL bChanged = FALSE;
			Dose_DSVPBGetPBLocal(m_sID, &bChanged);
			if (bChanged)
			{
				bLocalMode = TRUE;
			}
		}
	}
	Dose_DSVPBResetPBLocal(m_sID);
	Dose_DSVPBResetPBInline(m_sID);

	if ( bLocalMode != bLocalModeOld ) 
	{
		Dose_DSVSetLocalModeState		(m_sID, bLocalMode) ;
		if (bLocalMode )
		{
			Dose_DSVSetPercentage(m_sID, 0.0f)  ;
		}
        bModusChanged = TRUE;
	}
	return bModusChanged;
}
//******************************************************************************************************
//******************************************************************************************************
BOOL CBaseControl :: GetVolumetric ( void)
{
	BOOL	bVolumetric = FALSE;
	Dose_DSVGetVolumetricState	    (m_sID, &bVolumetric)	;
	BOOL	bIOVolumetric = FALSE;
	Dose_DSVGetIOVolumetricInput (m_sID, &bIOVolumetric);
	if (bIOVolumetric || (m_bIOVolumetricLast != bIOVolumetric)  )
	{
		m_bIOVolumetricLast = bIOVolumetric;
		bVolumetric = bIOVolumetric;
	}
	else
	{
		if (bVolumetric)
		{
			BOOL	bChanged = FALSE;
			Dose_DSVPBGetPBGravimetric(m_sID, &bChanged);
			if (bChanged)
			{
				bVolumetric = FALSE;
			}
		}
		else
		{
			BOOL	bChanged = FALSE;
			Dose_DSVPBGetPBVolumetric(m_sID, &bChanged);
			if (bChanged)
			{
				bVolumetric = TRUE;
			}
		}
	}
	Dose_DSVPBResetPBGravimetric(m_sID);
	Dose_DSVPBResetPBVolumetric(m_sID);
	Dose_DSVSetVolumetricState (m_sID, bVolumetric) ;
	return TRUE;
}
//******************************************************************************************************
//******************************************************************************************************
BOOL CBaseControl :: InitExecute ( void )
{
	auto result = CTimerStatemachine::InitExecute();
	if (result)
	{
		m_AdsClient.Init();
		m_aAlarm.ClearAllAlarms();
		Init();
		Dose_DSVInitVolumetricState(m_sID, TRUE);
		base::eDoseType lDoseType;
		Dose_DSVGetDoseType(m_sID, &lDoseType);
		Dose_DSVMaskAllAlarmCfg(m_sID, base::eAlarmErrorBits(lDoseType));
		Dose_DSVGetIOEmptyFeederStartInput(m_sID, &m_bEmptyFeederIOStartInputLast);
		Dose_DSVGetIOEmptyFeederStopInput(m_sID, &m_bEmptyFeederIOStopInputLast);
		Dose_DSVGetIOStartInput(m_sID, &m_bDoseIOStartInputLast);
		Dose_DSVGetIOVolumetricInput(m_sID, &m_bIOVolumetricLast);
		assert( ! m_bEmptyFeederIOStartInputLast);
		assert( ! m_bEmptyFeederIOStopInputLast);
		assert( ! m_bDoseIOStartInputLast);
		assert( ! m_bIOVolumetricLast);
	}
    return result;
}
//******************************************************************************************************
//******************************************************************************************************
BOOL CBaseControl :: ExitExecute ( void )
{
	auto result = CTimerStatemachine::IsInit();
	if (result)
	{
		Stop();
		ClearAlarm(eAlarmError::ERROR_MAX);
		m_bDoseIOStartInputLast = FALSE;
		m_bEmptyFeederIOStartInputLast = FALSE;
		m_bEmptyFeederIOStopInputLast = FALSE;
		m_AdsClient.Exit();
		result = CTimerStatemachine::ExitExecute();
	}
	return result;
}
//******************************************************************************************************
//******************************************************************************************************
BOOL CBaseControl :: Init ( void)
{
	Dose_DSVPBResetPBDosiererStop(m_sID);
	Dose_DSVPBResetPBDosiererStart(m_sID);
	Dose_DSVPBResetPBCalibStart(m_sID);
	Dose_DSVPBResetPBCalibStop(m_sID);
	Dose_DSVPBResetPBTareStart(m_sID);
	Dose_DSVPBResetPBTareStop(m_sID);
	Dose_DSVPBResetPBClearAlarm(m_sID);
	Dose_DSVPBResetPBAcknowledgeAlarm(m_sID);
	Dose_DSVPBResetPBGravimetric(m_sID);
	Dose_DSVPBResetPBVolumetric(m_sID);
	Dose_DSVPBResetPBEmptyFeederStart(m_sID);
	Dose_DSVPBResetPBEmptyFeederStop(m_sID);
	Dose_DSVPBResetPBInline(m_sID);
	Dose_DSVPBResetPBLocal(m_sID);

	Dose_DSVInitCalibrationState		(m_sID, FALSE);
	Dose_DSVInitTaraState				(m_sID, FALSE);
	Dose_DSVInitEmptyFeederState		(m_sID, FALSE);
	Dose_DSVInitGravVolState	        (m_sID, FALSE );
	Dose_DSVInitEncoderState			(m_sID, eEncoderState::ENCODERSTATE_OFF);
	Dose_DSVInitDrehgeber			    (m_sID, FALSE);
	Dose_DSVInitActualRotationalSpeed	(m_sID, 0.0f);
    Dose_DSVInitPidGatefilterState      (m_sID, base::eGatefilter::GATEFILTER_NONE );
    Dose_DSVInitActualNoise             (m_sID, 0.0f);

	float32_t fSetpoint = 0.0;
	Dose_DSVPopNominalSetpoint(m_sID, &fSetpoint);
	Dose_DSVInitActualSetpoint(m_sID, fSetpoint);

	BOOL bSlaveMode = FALSE;
	Dose_DSVGetSlaveMode(m_sID, &bSlaveMode);
	if (bSlaveMode)
	{
		Dose_DSVInitPercentage(m_sID, 0.0F);
	}
	return TRUE;
}
//******************************************************************************************************
//******************************************************************************************************
BOOL CBaseControl :: Stop ( void)
{
	auto result = CTimerStatemachine::IsStarted();
	if ( result )
	{
		Init();
		Dose_EXSetLineDoseStarted(m_sID, FALSE);
		Dose_EXSetDoseStarted(m_sID, FALSE);
		Dose_EXSetLineDoseWaitStart(m_sID, FALSE);
		Dose_DSVSetStart(m_sID, FALSE);
		Dose_DSVSetRelease(m_sID, FALSE);
		m_aAlarm.Stop();
		SetOperatingMode(eOperatingMode::IDLE);
		result = CTimerStatemachine::Stop();
	}
    return result;
}
//******************************************************************************************************
//******************************************************************************************************
BOOL CBaseControl :: Start ( const uint32_t t)
{
	auto result = CTimerStatemachine::Start(t);
	if (result)
	{
		BOOL bLocalMode = FALSE;
		Init();
		Dose_DSVGetLocalModeState(m_sID, &bLocalMode);
		Dose_EXSetLineDoseStarted(m_sID, !bLocalMode);
		Dose_EXSetDoseStarted(m_sID, TRUE);
		Dose_EXSetLineDoseWaitStart(m_sID, FALSE);
		m_aAlarm.Start(m_st);
		SetOperatingMode(eOperatingMode::RUNNING);
	}
    return result;
}
//******************************************************************************************************
//******************************************************************************************************
BOOL CBaseControl::GetPBStop(void) const
{
	BOOL bMBStop = FALSE;
	Dose_DSVPBGetPBDosiererStop(m_sID, &bMBStop);
	if (bMBStop)
	{
		Dose_DSVPBResetPBDosiererStop(m_sID);
		Dose_DSVPBSetPBEmptyFeederStop(m_sID);
		Dose_DSVPBSetPBCalibStop(m_sID);
		Dose_DSVPBSetPBTareStop(m_sID);
	}
	return bMBStop;
}
//******************************************************************************************************
//******************************************************************************************************
BOOL CBaseControl::GetPBStart(void) const
{
	BOOL bMBStart = FALSE;
	Dose_DSVPBGetPBDosiererStart(m_sID, &bMBStart);
	if (bMBStart)
	{
		Dose_DSVPBResetPBDosiererStart(m_sID);
	}
	return bMBStart;
}
//******************************************************************************************************
//******************************************************************************************************
BOOL CBaseControl::GetStart(void)
{
	BOOL bStart			= FALSE;
	Dose_DSVGetStart(m_sID, &bStart);
	BOOL bStartOld = bStart;

	BOOL bLocal = FALSE;
	Dose_DSVGetLocalModeState(m_sID, &bLocal);
	BOOL bIOStartInput = FALSE;
	Dose_DSVGetIOStartInput(m_sID, &bIOStartInput);
	if (bIOStartInput || (bIOStartInput != m_bDoseIOStartInputLast))
	{
		// Sollte eimmer false bleiben
		m_bDoseIOStartInputLast		= bIOStartInput;
		bStart						= bIOStartInput;
	}
	else
	{
		if (! bLocal)
		{
			BOOL bLineStarted		= FALSE;
			Dose_EXGetLineStarted(&bLineStarted);
			BOOL bStartDose = FALSE;
			Dose_EXGetLineDoseWaitStart(m_sID, &bStartDose);
			if (bLineStarted != m_bLineStartedLast)
			{
				m_bLineStartedLast = bLineStarted;
				if (bLineStarted)
				{
					BOOL bLineDoseStarted = FALSE;
					Dose_EXGetLineDoseStarted(m_sID, &bLineDoseStarted);
					bStartDose = !bLineDoseStarted;
#if _DEBUG
					if (bStartDose)
					{
						base::eDoseType lDoseType = base::eDoseType::DOSETYPE_NONE;
						Dose_DSVGetDoseType(m_sID, &lDoseType);
						if (base::__ISBATCHERTYPE(lDoseType))
						{
							BOOL bStarted = FALSE;
							Dose_DSVGetBatchStarted(m_sID, &bStarted);
							assert(!bStarted);
							BOOL bFinished = FALSE;
							Dose_DSVGetBatchFinished(m_sID, &bFinished);
							assert(!bFinished);
						}
					}
#endif
				}
				else
				{
					bStartDose = FALSE;
					bStart	   = FALSE;
				}
				Dose_EXSetLineDoseWaitStart(m_sID, bStartDose);
			}
			if (bStartDose)
			{
				bStart = TRUE;
			}
		}
		else
		{
			Dose_EXSetLineDoseWaitStart(m_sID, FALSE);
		}

		// No hardware trigger
		if (bStart)
		{
			BOOL bStop = GetPBStop();
			if (bStop)
			{
				bStart = FALSE;
			}
		}
		else
		{
			bStart = GetPBStart();
		}
	}
	if (bStart)
	{
		BOOL bTaraState = FALSE;
		Dose_DSVGetTaraState(m_sID, &bTaraState);
		BOOL bCalibration = FALSE;
		Dose_DSVGetCalibrationState(m_sID, &bCalibration);
		BOOL bEmptyFeeder = FALSE;
		Dose_DSVGetEmptyFeederState(m_sID, &bEmptyFeeder);
		BOOL bLogoStartState = FALSE;
		Dose_DSVGetLogoStartState(m_sID, &bLogoStartState);
		bStart = bLogoStartState && (!bCalibration) && (!bEmptyFeeder) && (!bTaraState);
	}
	if (bStart != bStartOld)
	{
		Dose_DSVSetStart(m_sID, bStart);
	}
	return bStart;
}
//******************************************************************************************************
//******************************************************************************************************
void CBaseControl :: GetLineAlarms ( void)
{
	auto setalarm = [this](BOOL bAlarm, eAlarmError errorcode)
	{
		if (bAlarm)
		{
			if (!m_aAlarm.IsAlarm(errorcode))
			{
				m_aAlarm.SetAlarm(m_st, errorcode, TRUE, eAlarmClass::eALARMTYP);
			}
		}
		else
		{
			m_aAlarm.ClearAlarm(errorcode);
		}
		m_aAlarm.CheckWarningLevel(errorcode);
	};

	// 42
	BOOL bAlarm = FALSE;
	Dose_EXGetIOSyncError(&bAlarm);
	setalarm(bAlarm, eAlarmError::ERROR_LINE_SYNCERROR);

	// 43
	Dose_EXGetAlarmShutDown(&bAlarm);
	setalarm(bAlarm, eAlarmError::ERROR_LINE_SHUTDOWN);

	// 44
	Dose_EXGetIOExtAlarmInput(&bAlarm);
	setalarm(bAlarm, eAlarmError::ERROR_LINE_EXTALARM);

	// 45
	Dose_EXGetIOCustomerAlarmInput(&bAlarm);
	setalarm(bAlarm, eAlarmError::ERROR_LINE_CUSTOMER_ALARM);

	// 46
	Dose_EXGetUPSLowInputState(&bAlarm);
	setalarm(bAlarm, eAlarmError::ERROR_LINE_UPS_ALARM);
}
//******************************************************************************************************
//******************************************************************************************************
BOOL CBaseControl::GetValidSetpoint(void)
{
	float32_t fSollwert = 0.0f;
	Dose_DSVGetActualSetpoint(m_sID, &fSollwert);
	return BOOL(fSollwert > 1e-06);
}

//******************************************************************************************************
//******************************************************************************************************
BOOL CBaseControl :: GetRelease ( void)
{
	// Release
	BOOL bIOSyncError		= FALSE;
	BOOL bAlarmShutDown		= FALSE;
	BOOL bRelease			= FALSE;
	Dose_DSVGetRelease	(m_sID, &bRelease);
	Dose_EXGetAlarmShutDown(&bAlarmShutDown);
	Dose_EXGetIOSyncError(&bIOSyncError);
	if ( (!bAlarmShutDown) && (!bIOSyncError) )
	{
		bRelease = GetStart();
		if (bRelease)
		{
			bRelease = GetValidSetpoint();
		}
	}
	Dose_DSVSetRelease	(m_sID, bRelease);
	return bRelease;
}

//*********************************************************************************************
//*********************************************************************************************
BOOL CBaseControl::Execute(void)
{
	auto result = CTimerStatemachine::Execute();
	if (result)
	{
		m_AdsClient.UpdateState();

		GetLineAlarms();

		// Verbrauch nullen
		GetClearTotalizer();

		// Alarmloeschen
		GetClearAlarm();

		// Freigabe
		GetRelease();

		// Volumetric/ Gravimetrisch
		GetVolumetric();

		// Alarmshutdown
		BOOL bAlarmShutdown = FALSE;
		Dose_EXGetAlarmShutDown(&bAlarmShutdown);

		// Interne Kommunikation gestoert
		BOOL bSyncError = FALSE;
		Dose_EXGetIOSyncError(&bSyncError);

		// Anlagenmodus
		auto AnlagenModusChanged = GetAnlagenModus();

		if (bAlarmShutdown || bSyncError || AnlagenModusChanged)
		{
			Stop();
			SetAlarmOutput();
			result = FALSE;
		}
	}
	return result;
}



