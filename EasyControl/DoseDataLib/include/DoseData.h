//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Declaration of module DoseData
///
/// @file   DoseData.h
///
///
/// @coypright Ing.b�ro Hafer
///            Branderweg 8A
///            D-91058 Erlangen
///
/// @author    Detlef Hafer
///
//------------------------------------------------------------------------------------
#pragma once

#include "BASE/Utils/public/StringUtil.h"
#include "BASE/include/AlarmTypes.h"
#include "BASE/include/DoseType.h"
#include "BASE/include/FeedingType.h"
#include "BASE/include/TotalizerType.h"
#include "BASE/include/EmptyFeederType.h"
#include "BASE/include/EncoderState.h"
#include "BASE/include/Gatefilter.h"
#include "BASE/include/LCType.h"
#include "BASE/include/WbmStates.h"
#include "BASE/include/CalibType.h"
#include "BASE/include/RefillSignalType.h"
#include "DoseDataDeclareMacro.h"
#include "DoseData.def"



//------------------------------------------------------------------------------------
///  @brief   Dose_Save
///
///           This method ....
///
///  @param[in] none
///  @return int32_t
///
//------------------------------------------------------------------------------------
int32_t Dose_Save(const std::string& szpath);

//------------------------------------------------------------------------------------
///  @brief   Dose_SaveBak
///
///           This method ....
///
///  @param[in] none
///  @return int32_t
///
//------------------------------------------------------------------------------------
int32_t Dose_SaveBak(const std::string& szpath);

//------------------------------------------------------------------------------------
///  @brief   Dose_Activate
///
///           This method ....
///
///  @param[in] none
///  @return int32_t
///
//------------------------------------------------------------------------------------
int32_t Dose_Activate();

//------------------------------------------------------------------------------------
///  @brief   Dose_Restore
///
///           This method ....
///
///  @param[in] none
///  @return int32_t
///
//------------------------------------------------------------------------------------
int32_t Dose_Restore(const std::string& szpath);



//------------------------------------------------------------------------------------
///  @brief   Dose_Load
///
///           This method ....
///
///  @param[in] none
///  @return int32_t
///
//------------------------------------------------------------------------------------
int32_t Dose_Load(const std::string& szpath);

//------------------------------------------------------------------------------------
///  @brief   Dose_ResetAll
///
///           This method ....
///
///  @param[in] none
///  @return int32_t
///
//------------------------------------------------------------------------------------
int32_t Dose_ResetAll		(void);

//------------------------------------------------------------------------------------
///  @brief   Dose_InitializeAll
///
///           This method ....
///
///  @param[in] none
///  @return int32_t
///
//------------------------------------------------------------------------------------
int32_t Dose_InitializeAll	(void);

int32_t System_GetDosierItems	(uint32_t*);
int32_t System_SetDosierItems	(uint32_t);
int32_t System_GetRemoteStatus		    (int32_t*);
int32_t System_SetRemoteStatus		    (const int32_t);


//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
int32_t Dose_EXSetLineDoseWaitStart         (const int32_t k, const BOOL b);
int32_t Dose_EXGetLineDoseWaitStart         (const int32_t k, BOOL* b);
int32_t Dose_EXDeleteLineDoseWaitStart(void);
int32_t Dose_EXGetAnyLineDoseWaitStart(BOOL* b);

int32_t Dose_EXSetLineDoseStarted           (const int32_t k, const BOOL b);
int32_t Dose_EXGetLineDoseStarted           (const int32_t k, BOOL *b);
int32_t Dose_EXDeleteLineDoseStarted        (void);
int32_t Dose_EXGetAnyLineDoseStarted        (BOOL *b);

int32_t Dose_EXSetDoseStarted			   (const int32_t k, const BOOL b);
int32_t Dose_EXGetDoseStarted			   (const int32_t k, BOOL *b);
int32_t Dose_EXDeleteDoseStarted            (void);

int32_t Dose_EXGetAnyDoseStarted		   (BOOL *b);
int32_t Dose_EXSetStartDose                (const int32_t k, const BOOL b);
int32_t Dose_EXGetStartDose                (const int32_t k, BOOL* b);

int32_t Dose_EXGetAlarm                     (int32_t *l);

DECLARE_DOSEEX_STRING(std::string, Name)
DECLARE_DOSEEX_STRING(std::string, ANNumber)
DECLARE_DOSEEX(uint64_t, QMNumber)
DECLARE_DOSEEX(base::eLcType, LCTyp);				// Waagentyp

DECLARE_DOSEEX(BOOL, LineStart)
DECLARE_DOSEEX(BOOL, IOLineStartInput)
DECLARE_DOSEEX(BOOL, IOExtAlarmInput)
DECLARE_DOSEEX(BOOL, IOCustomerAlarmInput)
DECLARE_DOSEEX(BOOL, IOAlarmShutDownInput);		// ShutDown extern
DECLARE_DOSEEX(BOOL, IOWindowsShutDownInput);	// Windows ShutDown extern
DECLARE_DOSEEX(BOOL, IOSyncErrorExtern);		// ShutDown extern
DECLARE_DOSEEX(BOOL, IOSyncError);				// SyncErrorState
DECLARE_DOSEEX(BOOL, IOLineSetpointEnableInput)
DECLARE_DOSEEX(BOOL, IOLineInvSetpointEnableInput)
DECLARE_DOSEEX(BOOL, IORampUpInput)
DECLARE_DOSEEX(BOOL, IORampDownInput)
DECLARE_DOSEEX(BOOL, IOSideFeeder1Input)
DECLARE_DOSEEX(BOOL, IOSideFeeder2Input)
DECLARE_DOSEEX(BOOL, IOMainFeederInput)
DECLARE_DOSEEX(BOOL, IOAlarm0Output)
DECLARE_DOSEEX(BOOL, IOAlarm1Output)
DECLARE_DOSEEX(BOOL, IOAlarm2Output)
DECLARE_DOSEEX(BOOL, IOAlarm3Output)
DECLARE_DOSEEX(BOOL, IOTotalizerPulseOutput)
DECLARE_DOSEEX(float32_t, IOLineSetpointInput);			// ext. Liniensollwert
DECLARE_DOSEEX(BOOL, IOAlarm1QuitInput)
DECLARE_DOSEEX(BOOL, IOAlarm03QuitInput)
DECLARE_DOSEEX(BOOL, IOOperateAutoInput)
DECLARE_DOSEEX(BOOL, IOOperateManualInput)
DECLARE_DOSEEX(BOOL, IOUPSLowInput)
DECLARE_DOSEEX(BOOL, IOOperateAutoOutput)			// IO operate auto output
DECLARE_DOSEEX(BOOL, IOServiceModeOutput)			// IO operate service output


DECLARE_DOSEEX(BOOL, ServiceMode)
DECLARE_DOSEEX(BOOL, LineStarted)
DECLARE_DOSEEX(BOOL, AlarmShutDown)
DECLARE_DOSEEX(float32_t, Hysterese)
DECLARE_DOSEEX(float32_t, RegeneratPercentage)
DECLARE_DOSEEX(float32_t, RecipeSetpoint)
DECLARE_DOSEEX(float32_t, ManualSetpoint)
DECLARE_DOSEEX(float32_t, ExternalSetpoint)
DECLARE_DOSEEX(float32_t, Skalierung)
DECLARE_DOSEEX(float32_t, TotalizerAll)
DECLARE_DOSEEX(float32_t, TotalizerLine)
DECLARE_DOSEEX(float32_t, TotalizerMF)
DECLARE_DOSEEX(float32_t, TotalizerSF1)
DECLARE_DOSEEX(float32_t, TotalizerSF2)
DECLARE_DOSEEX(float32_t, LineSetpoint)
DECLARE_DOSEEX(float32_t, LineMassflowMF)
DECLARE_DOSEEX(float32_t, LineMassflowSF1)
DECLARE_DOSEEX(float32_t, LineMassflowSF2)
DECLARE_DOSEEX(float32_t, LineMassflow)
DECLARE_DOSEEX(uint32_t, ProductionTime)
DECLARE_DOSEEX(BOOL, ExtLineSetpointState)
DECLARE_DOSEEX(BOOL, UPSLowInputState)					// UPS low
DECLARE_DOSEEX(BOOL, ExtLineInvSetpointState)
DECLARE_DOSEEX(BOOL, UPSLowInputState)					// UPS low
DECLARE_DOSEEX(float32_t, RampStep)
DECLARE_DOSEEX(float32_t, MaxLeistung)
DECLARE_DOSEEX(float32_t, MinTotband)
DECLARE_DOSEEX(uint32_t,  Filterung)
DECLARE_DOSEEX(uint32_t, RampDelay)
DECLARE_DOSEEX(BOOL, RampUp)
DECLARE_DOSEEX(BOOL, RampDown)
DECLARE_DOSEEX(BOOL, Alarm03QuitState)
DECLARE_DOSEEX(BOOL, Alarm1QuitState)
DECLARE_DOSEEX(BOOL, Alarm0State)
DECLARE_DOSEEX(BOOL, Alarm1State)
DECLARE_DOSEEX(BOOL, Alarm2State)
DECLARE_DOSEEX(BOOL, Alarm3State)
DECLARE_DOSEEX(BOOL, IOLineReleaseOutput)
DECLARE_DOSEEX(uint32_t, FeederAlarm)
DECLARE_DOSEEX(float32_t, TotalizerPulseStep);						// Schrittweite Totalisatorimpuls [kg]
DECLARE_DOSEEX(uint32_t,  TotalizerPulseDuration);					// Dauer des Impulses [s]


DECLARE_DOSEEX(BOOL,  MBLineSetpointExist);
DECLARE_DOSEEX(float32_t, MBLineSetpoint);
DECLARE_DOSEEX(float32_t, MBLinePercentage);
DECLARE_DOSEEX(float32_t, ExtLineSetpoint);
DECLARE_DOSEEX(float32_t, ExtLineOffset);
DECLARE_DOSEEX(BOOL, MBFlashlight);

DECLARE_DOSEEX(BOOL, ModuleExternalSetpoint)
DECLARE_DOSEEX(BOOL, ModuleSetpointRamp)
DECLARE_DOSEEX(BOOL, ModuleLineSetpoint)
DECLARE_DOSEEX(BOOL, ModuleLocalSetpoint)
DECLARE_DOSEEX(BOOL, ModuleBatcher)
DECLARE_DOSEEX(BOOL, ModuleTotalizer)
DECLARE_DOSEEX(BOOL, ModuleTotalizerPulse)
DECLARE_DOSEEX(BOOL, LineReleaseEnable);      // Enable Line Release
DECLARE_DOSEEX(BOOL, LineStartEnable);        // Enable Line Start
DECLARE_DOSEEX(BOOL, LineRelease)
DECLARE_DOSEEX(BOOL, ExtLineSetpointEnable)
DECLARE_DOSEEX(BOOL, ExtLineInvSetpointEnable)


DECLARE_DOSEEXPB(PBAlarm03Quit)
DECLARE_DOSEEXPB(PBAlarm1Quit)
DECLARE_DOSEEXPB(PBRampUp)
DECLARE_DOSEEXPB(PBRampDown)
DECLARE_DOSEEXPB(PBLineStart)
DECLARE_DOSEEXPB(PBLineStop)
DECLARE_DOSEEXPB(PBClearTotalizer)
DECLARE_DOSEEXPB(PBLineShutDown)
DECLARE_DOSEEXPB(PBAlarm0)
DECLARE_DOSEEXPB(PBAlarm1)
DECLARE_DOSEEXPB(PBAlarm2)
DECLARE_DOSEEXPB(PBAlarm3)
DECLARE_DOSEEXPB(PBServiceMode)

//------------------------------------------------------------------------------------



//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// Variablen
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
int32_t Dose_DSVSetRefillMinMaxLimit( const int32_t index, const float32_t f1, const float32_t f2, const float32_t f3);
int32_t Dose_DSVGetRefillMinMaxLimit( const int32_t index, float32_t *f1, float32_t *f2, float32_t *f3);
int32_t Dose_DSVSetAlarmCfgBit( const int32_t index, const base::eAlarmCategory k, const base::eAlarmError iBit, BOOL b);
int32_t Dose_DSVGetAlarmCfgBit( const int32_t index, const base::eAlarmCategory k, const base::eAlarmError iBit, BOOL* b);
int32_t Dose_DSVSetAlarmCfg(const int32_t index, const base::eAlarmCategory k, const base::eAlarmErrorBits& b);
int32_t Dose_DSVGetAlarmCfg(const int32_t index, const base::eAlarmCategory k, base::eAlarmErrorBits* b);
int32_t Dose_DSVSetAlarmCfg(const int32_t index, const base::eAlarmErrorBits& b);
int32_t Dose_DSVGetAlarmCfg(const int32_t index, base::eAlarmErrorBits* b);
int32_t Dose_DSVMaskAllAlarmCfg(const int32_t n, const base::eAlarmErrorBits& mask);
int32_t Dose_DSVMaskAlarmCfg(const int32_t n, const base::eAlarmCategory k, const base::eAlarmErrorBits& mask);



//----------------------------------------------------------------------

DECLARE_DOSEDSV_STRING(std::string, Name)
DECLARE_DOSEDSV_STRING(std::string, LogoString)
DECLARE_DOSEDSV(uint64_t, QMNumber)
DECLARE_DOSEDSV(base::eLcType, LCTyp);				// Waagentyp


DECLARE_DOSEDSV(BOOL, ModuleRefillEnable)

DECLARE_DOSEDSV(base::eDoseType, DoseType)
DECLARE_DOSEDSV(base::eFeedingType, FeederType)
DECLARE_DOSEDSV(float32_t, Percentage)
DECLARE_DOSEDSV(float32_t, NominalSetpoint)
DECLARE_DOSEDSV(float32_t, MaxSetpoint)
DECLARE_DOSEDSV(float32_t, MaxRotationalSpeed)
DECLARE_DOSEDSV(float32_t, MassflowFilter)

DECLARE_DOSEDSV(float32_t, LCCorrectionFactor);		// Korrekturwert Waage
DECLARE_DOSEDSV(float32_t, LCTaraWeight);				// Taraweight

DECLARE_DOSEDSV(float32_t, RefillLimitMin)
DECLARE_DOSEDSV(float32_t, RefillLimitMax)
DECLARE_DOSEDSV(float32_t, RefillLimitMinMin)
DECLARE_DOSEDSV(uint32_t,  RefillTime)
DECLARE_DOSEDSV(uint32_t,  RefillSwitchDelay)
DECLARE_DOSEDSV(uint32_t,  RefillDebounceMax);				// RefillDebounceMax
DECLARE_DOSEDSV(uint32_t,  RefillDebounceMin);				// RefillDebounceMin
DECLARE_DOSEDSV(uint32_t,  RefillFeederEmptyStart);		// MinDurationActive
DECLARE_DOSEDSV(BOOL,      RefillWeightAboveMax);
DECLARE_DOSEDSV(BOOL,      RefillWeightBelowMin);
DECLARE_DOSEDSV(BOOL,      RefillFeederEmptyStarted);
DECLARE_DOSEDSV(BOOL,      RefillFeederEmptyEnded);


DECLARE_DOSEDSV(float32_t, NominalAgitator)
DECLARE_DOSEDSV(float32_t, RefillFeederNominalSpeed)
DECLARE_DOSEDSV(float32_t, MinSetpointChange)
DECLARE_DOSEDSV(uint32_t, StartupDelay)
DECLARE_DOSEDSV(uint32_t, StartupRamp)

DECLARE_DOSEDSV(uint32_t, EncoderMonitor)
DECLARE_DOSEDSV(uint32_t, LogoEncoderSampleTime)
DECLARE_DOSEDSV(BOOL,     LogoRunControlEnable);		// RunControl Enabled

DECLARE_DOSEDSV(float32_t, AlarmNoiseLimit)
DECLARE_DOSEDSV(uint32_t, AlarmReactionDelay)
DECLARE_DOSEDSV(uint32_t, AlarmStartReactionDelay)
DECLARE_DOSEDSV(uint32_t, AlarmMassflowHigh)
DECLARE_DOSEDSV(uint32_t, AlarmMassflowLow)
DECLARE_DOSEDSV(uint32_t, AlarmDriveCommandHigh)
DECLARE_DOSEDSV(uint32_t, AlarmDriveCommandLow)
DECLARE_DOSEDSV(uint32_t, AlarmDosePerformance)
DECLARE_DOSEDSV(uint32_t, AlarmMaxBatchTime)
DECLARE_DOSEDSV(float32_t, MaxDriveCommand)
DECLARE_DOSEDSV(BOOL, Regenerat)
DECLARE_DOSEDSV(uint32_t, IOPulseCounterInput);


DECLARE_DOSEDSV(float32_t, WbfBeltLoadSetpoint)
DECLARE_DOSEDSV(float32_t, WbfBeltLoadVol)
DECLARE_DOSEDSV(float32_t, WbfReduction)
DECLARE_DOSEDSV(uint32_t, WbfWeighingLine)
DECLARE_DOSEDSV(uint32_t, WbfWheelSize)
DECLARE_DOSEDSV(float32_t, WbfTareDriveCommand)
DECLARE_DOSEDSV(uint32_t, WbfTareMeasurementTime)
DECLARE_DOSEDSV(float32_t, WbfAlarmMinBeltLoad)
DECLARE_DOSEDSV(float32_t, WbfAlarmMaxBeltLoad)
DECLARE_DOSEDSV(float32_t, WbfMinDriveCommand)
DECLARE_DOSEDSV(BOOL, WbfWeighingFull)

// PID
DECLARE_DOSEDSV(float32_t, PidPropGainGross)
DECLARE_DOSEDSV(uint32_t, PidSampleInterval)
DECLARE_DOSEDSV(float32_t, PidPropGainFine)
DECLARE_DOSEDSV(float32_t, PidPropGainSwitchGrossFine)
DECLARE_DOSEDSV(float32_t, PidIntegralGain)
DECLARE_DOSEDSV(BOOL, PidDriveCommandInv)

// VOL

// ICP
DECLARE_DOSEDSV(float32_t, SteepnessMassflow)

// LCL/IFL
DECLARE_DOSEDSV(float32_t, LclWeightMinLevel);
DECLARE_DOSEDSV(float32_t, LclWeightMaxLevel);
DECLARE_DOSEDSV(float32_t, LclWeightHysteresis);
DECLARE_DOSEDSV(BOOL, LclWeightMinLevelActive);
DECLARE_DOSEDSV(BOOL, LclWeightMaxLevelActive);

// IFL
DECLARE_DOSEDSV(float32_t, IflMinLineSetpoint);			// Oberer Gewichtswert
DECLARE_DOSEDSV(float32_t, IflMaxLineSetpoint);			// Oberer Gewichtswert


// IFS
DECLARE_DOSEDSV(BOOL, IfsSwitchHigh);					// Endschalter Einschneckenextruder oben bzw. �berlauf aktiviert
DECLARE_DOSEDSV(BOOL, IfsSwitchHighEnable);				// Endschalter Einschneckenextruder oben bzw. �berlauf aktiviert
DECLARE_DOSEDSV(BOOL, IfsSwitchLow);					// Endschalter Einschneckenextruder unter bzw. Leer  aktiviert
DECLARE_DOSEDSV(BOOL, IfsSwitchLowEnable);				// Endschalter Einschneckenextruder unter bzw. Leer  aktiviert
DECLARE_DOSEDSV(BOOL, IfsSwitchMax);					// Endschalter Max, oberer Umkehrpunkt  aktiviert
DECLARE_DOSEDSV(BOOL, IfsSwitchMaxEnable);				// Endschalter Max, oberer Umkehrpunkt  aktiviert
DECLARE_DOSEDSV(BOOL, IfsSwitchMin);					// Endschalter Min, unterer Umkehrpunkt  aktiviert
DECLARE_DOSEDSV(BOOL, IfsSwitchMinEnable);				// Endschalter Min, unterer Umkehrpunkt  aktiviert
DECLARE_DOSEDSV(BOOL, IfsEmptyFeeder);				    // Dosierer leer erreicht
DECLARE_DOSEDSV(BOOL, IfsOverflowFeeder);				// Dosierer Überlauf
DECLARE_DOSEDSV(float32_t, IfsReduceFactor);			// Um wieviel kg/h soll der Liniensollwert verringert werden
DECLARE_DOSEDSV(float32_t, IfsGainFactor);				// Um wieviel kg/h soll der Liniensollwert erhöht werden
DECLARE_DOSEDSV(uint32_t, IfsFeederOverflowTimeOut);	// Zeit, bis der Zustand "Feeder Überlauf" erkannt wird.
DECLARE_DOSEDSV(uint32_t, IfsFeederEmptyTimeOut);		// Zeit, bis der Zustand "Feeder leer" erkannt wird.
DECLARE_DOSEDSV(uint32_t, IfsStepTimeGain);		        // Zeitintervall in Sekunden bis der Sollwert erhöht wird.
DECLARE_DOSEDSV(uint32_t, IfsStepTimeReduce);		    // Zeitintervall in Sekunden bis der Sollwert verringert wird.
DECLARE_DOSEDSV(float32_t, IfsSetpointOverflow);		// Neuer Sollwert bei Erreichen von Overflow
DECLARE_DOSEDSV(uint32_t, IfsDebounceMax);				// Debounce Max / High
DECLARE_DOSEDSV(uint32_t, IfsDebounceMin);				// Debounce Min / Low



DECLARE_DOSEDSV(uint32_t, EmptyFeederRuntime)
DECLARE_DOSEDSV(float32_t, EmptyFeederSpeed);
DECLARE_DOSEDSV(base::eEmptyFeederType, EmptyFeederRestriction)

DECLARE_DOSEDSV(BOOL, IOStartInput)			    // Start extern
DECLARE_DOSEDSV(BOOL, IOReleaseInput)           // Freigabe extern
DECLARE_DOSEDSV(BOOL, LogoStartState)		        // Logo Start
DECLARE_DOSEDSV(BOOL, IOVolumetricInput)
DECLARE_DOSEDSV(BOOL, IOEmptyFeederStartInput);	// Externel empty feeder start
DECLARE_DOSEDSV(BOOL, IOEmptyFeederStopInput);	// Externel empty feeder stop
DECLARE_DOSEDSV(BOOL, IORefillAlarmInput);	    // Externel Refill alarm
DECLARE_DOSEDSV(BOOL, IORefillStopInput);		// Refillstopp durch (IO)
DECLARE_DOSEDSV(BOOL, IORefillRequestInput);	// Refillrequest durch (IO)
DECLARE_DOSEDSV(BOOL, IORefillReleaseInput);	// Refillrelease durch (IO)
DECLARE_DOSEDSV(BOOL, IORefillInput);			// Refill durch (IO)
DECLARE_DOSEDSV(BOOL, IORefillMaxSensorInput);	    // Refillrequest durch (IO)
DECLARE_DOSEDSV(BOOL, IORefillMinSensorInput);	    // Refillrelease durch (IO)
DECLARE_DOSEDSV(BOOL, IODriveErrorInput);	    // Fehler Drive ( Frequenzumrichter )
DECLARE_DOSEDSV(BOOL, IOMotorLoadInput);	    // Drive Motorlast
DECLARE_DOSEDSV(BOOL, IOExtAlarmInput);				// ext. Alarm
DECLARE_DOSEDSV(BOOL, IODriveErrorPaddleInput);	    // Fehler Drive Paddle ( Frequenzumrichter )
DECLARE_DOSEDSV(BOOL, IODriveErrorRefillFeederInput);	  // Fehler Drive RefillFeeder ( Frequenzumrichter )
DECLARE_DOSEDSV(float32_t, IOEncoderSpeedInput);	// Encoderspeed analog input


DECLARE_DOSEDSV(BOOL, IODriveReleaseOutput)
DECLARE_DOSEDSV(BOOL, IOOperateOutput)					// Betriebsleuchte, bei EncoderMonitor
DECLARE_DOSEDSV(BOOL, IOStartOutput)					// Start
DECLARE_DOSEDSV(BOOL, IOAlarmOutput);
DECLARE_DOSEDSV(BOOL, IORefillRequestOutput);	// Refillrequest output (IO)
DECLARE_DOSEDSV(float32_t, IOActualRefillFeederOutput)

DECLARE_DOSEDSV(float32_t,	    Massflow)
DECLARE_DOSEDSV(float32_t,      ActualSetpoint)				// Aktueller Sollwert
DECLARE_DOSEDSV(float32_t,	    ActualDoseperformance)
DECLARE_DOSEDSV(float32_t,	    ActualDriveCommand)
DECLARE_DOSEDSV(float32_t,	    Totalizer)
DECLARE_DOSEDSV(BOOL,		    ClearTotalizerState)
DECLARE_DOSEDSV(BOOL,		    VolumetricState)
DECLARE_DOSEDSV(BOOL,		    GravVolState)
DECLARE_DOSEDSV(BOOL,		    LocalModeState)
DECLARE_DOSEDSV(float32_t,      ActualAgitator)
DECLARE_DOSEDSV(float32_t,      ActualLogoEncoder)

DECLARE_DOSEDSV(float32_t,      ActualNoise);				// Aktuelle s Rauschen, siehe AlarmNoiseLimit

DECLARE_DOSEDSV(float32_t, PidGatefilter)

DECLARE_DOSEDSV(BOOL, RefillReleaseState);		    // Refill Freigabezustand
DECLARE_DOSEDSV(BOOL, RefillStartState);			// Refill Startzustand, kann gestarted werde
DECLARE_DOSEDSV(BOOL, RefillStopState);			    // Refill Stopzustand, wurde gestoppt
DECLARE_DOSEDSV(BOOL, RefillState);				    // Refill Zustand
DECLARE_DOSEDSV(BOOL, RefillFeederEnable);	        // Refill feeder enable


DECLARE_DOSEDSV(BOOL, AgitatorEnable);          // R�hrwerk enable
DECLARE_DOSEDSV(BOOL, AgitatorRunState);        // R�hrwerk l�uft

DECLARE_DOSEDSV(float32_t,	Drehgeber)
DECLARE_DOSEDSV(BOOL, DriveControlReady)
DECLARE_DOSEDSV(base::eEncoderState,EncoderState)


DECLARE_DOSEDSV(BOOL,      LCEnable)							// Waage aktiv
DECLARE_DOSEDSV(BOOL,      LCAktiv)							// Waage aktiv
DECLARE_DOSEDSV(float32_t, LCActualWeight)					// Aktuelles Gewicht
DECLARE_DOSEDSV(float32_t, LCMeanWeight)					// Gemitteltes Gewicht
DECLARE_DOSEDSV(float32_t, LCRawWeight)						// Rawweight

DECLARE_DOSEDSV(float32_t,	ActualRotationalSpeed)
DECLARE_DOSEDSV(BOOL,       SlaveMode)				    // SlaveMode
DECLARE_DOSEDSV(float32_t,	WbfBeltLoadMassflow)
DECLARE_DOSEDSV(float32_t,	WbfActVelocity)
DECLARE_DOSEDSV(base::eWbmState,WbfMinBeltLoadState)
DECLARE_DOSEDSV(BOOL,       WbfBeltLoadLimit)

DECLARE_DOSEDSV(float32_t, BatchGrossStream);				// Maximalgeschwindigkeit [%]
DECLARE_DOSEDSV(float32_t, BatchFineStream);				// Maximalgeschwindigkeit [%]
DECLARE_DOSEDSV(float32_t, BatchSwitchpoint);			    // Abbremsung [s]
DECLARE_DOSEDSV(uint32_t,  BatchTime);			            // BatchTime [s]
DECLARE_DOSEDSV(float32_t, BatchOverrunCompensation);	    // Batch Nachlaufkompensation [kg}
DECLARE_DOSEDSV(BOOL,      BatchStarted);			        // Batch started
DECLARE_DOSEDSV(BOOL,      BatchFinished);			        // Batch finished
DECLARE_DOSEDSV(int32_t,   GwbExtScale);			        // Gwb ext. Waagenindex

DECLARE_DOSEDSV(base::eGatefilter,	PidGatefilterState)
DECLARE_DOSEDSV( BOOL,      RefillControlEnable)
DECLARE_DOSEDSV( BOOL,	    CalibrationState)
DECLARE_DOSEDSV( BOOL,	    TaraState)			
DECLARE_DOSEDSV( BOOL,      EmptyFeederState)
DECLARE_DOSEDSV( BOOL,	    Release)			// Freigabe
DECLARE_DOSEDSV( BOOL,	    Start)				  // Startfreigabe extern/Modbus/User
DECLARE_DOSEDSV( BOOL,	    Service)

DECLARE_DOSEDSV(base::refillsignal::RefillSignalType, RefillSignalCfg);			// Alarme in Kategorien
DECLARE_DOSEDSV(base::calib::CalibType, CalibCfg)
DECLARE_DOSEDSV(base::calib::CalibType, CalibCfgSave)
DECLARE_DOSEDSV(int32_t, CalibAktIndex)
DECLARE_DOSEDSV(BOOL,   SWRefillRelease)            // Switch Refill release / disable
DECLARE_DOSEDSV(BOOL,   RefillReleaseTrigger)     // Pushbutton

DECLARE_DOSEDSVPB(PBDosiererStart)            // Pushbutton Dosierer start
DECLARE_DOSEDSVPB(PBDosiererStop)             // Pushbutton Dosierer stop
DECLARE_DOSEDSVPB(PBRefillStart)              // Pushbutton start refill
DECLARE_DOSEDSVPB(PBRefillStop)               // Pushbutton stop refill
DECLARE_DOSEDSVPB(PBCalibStart)				  // Pushbutton start calibration
DECLARE_DOSEDSVPB(PBCalibStop)                // Pushbutton stop calibration
DECLARE_DOSEDSVPB(PBTareStart)                // Pushbutton start taring
DECLARE_DOSEDSVPB(PBTareStop)                 // Pushbutton stop taring
DECLARE_DOSEDSVPB(PBEmptyFeederStart)         // Pushbutton start empty feeder 
DECLARE_DOSEDSVPB(PBEmptyFeederStop)          // Pushbutton stop empty feeder 
DECLARE_DOSEDSVPB(PBInline)                   // Pushbutton set line mode
DECLARE_DOSEDSVPB(PBLocal)                    // Pushbutton set local mode
DECLARE_DOSEDSVPB(PBVolumetric)               // Pushbutton
DECLARE_DOSEDSVPB(PBGravimetric)              // Pushbutton
DECLARE_DOSEDSVPB(PBClearAlarm)				  // Pushbutton Alarm zuruecksetzen
DECLARE_DOSEDSVPB(PBAcknowledgeAlarm)		  // Pushbutton Alarm quittieren
DECLARE_DOSEDSVPB(PBClearTotalizer);          // Pushbutton Totalizer nullen

DECLARE_DOSEDSVALARM(       Alarm);
DECLARE_DOSEDSVALARM(       Warning);
DECLARE_DOSEDSVALARM(       ClearAlarm);
DECLARE_DOSEDSVALARM(       ClearWarning);


//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//------------------------------------------------------------------------------------
///  @brief   Dose_Open
///
///           This method ....
///
///  @param[in] fName , LPCSTR
///  @return int32_t
///
//------------------------------------------------------------------------------------
int32_t Dose_Open							(  );


//------------------------------------------------------------------------------------
///  @brief   Dose_Close
///
///           This method ....
///
///  @param[in] none
///  @return int32_t
///
//------------------------------------------------------------------------------------
int32_t Dose_Close							( void );


