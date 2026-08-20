//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Declaration of module dose.h
///
/// @file   dose.h
///
///
/// @coypright Ing.büro Hafer
///            Branderweg 8A
///            D-91058 Erlangen
///
/// @author    Detlef Hafer
///
//------------------------------------------------------------------------------------
#pragma once

#include <array>
#include <mutex>
#include "DoseTypes.h"
#include "ExDeclareMacro.h"
#include "ECDeclareMacro.h"
#include "BASE/Utils/public/Memory.h"
#include "BASE/Utils/public/StringUtil.h"

template<class T>
class CLockableData : public std::mutex
{
	T	mGlobal;
public:
	CLockableData()
	{
		base::utils::baseZeroMemory(mGlobal);
	}
	~CLockableData() = default;
	CLockableData (const CLockableData&) = delete;
	CLockableData& operator = (const CLockableData&) = delete;

	T& get()
	{ return mGlobal; }
	const T& get() const
	{ return mGlobal; }
};

class CDose
{
private:
	CLockableData<CGlobalSystemItem>	mSGlobal;
	CLockableData<CGlobalLineItem>		mEGlobal;
	std::array<CLockableData<CGlobalDoseItem>, GLOBALDOSEMAXCOUNT> mDGlobal;

private:
	void	ResetCalibDoseItem		(int32_t n);
	void	ResetDoseItem			(int32_t n);
	void	ResetLineItem			(void);
	void	ResetDoseItems			(void);
	void	ResetSystem				(void);

	void	InitializeDoseItem(int32_t s);
	void	InitializeDoseItems(void);
	void	InitializeSystem(void);
	void	InitializeLineItem(void);


	static int32_t ECSetAlarmHelper(const base::eAlarmError l, ECTypes<base::eAlarmErrorBits>& field);
	static int32_t ECGetAlarmHelper(const ECTypes<base::eAlarmErrorBits>& field, base::eAlarmError& l);
	static int32_t ECDeleteAlarmHelper(const base::eAlarmError l, ECTypes<base::eAlarmErrorBits>& field);
	static int32_t ECIsAlarmHelper(const ECTypes<base::eAlarmErrorBits>& field, const base::eAlarmError l, BOOL& b);
	static int32_t ECIsAnyAlarmHelper(const ECTypes<base::eAlarmErrorBits>& field, BOOL& b);

	int32_t parse(const uint8_t* buffer, const uint32_t length);

public:
	static CDose* pDose;

	CDose (void);
	virtual ~CDose(void);


	//------------------------------------------------------------------------------------
	///  @brief   Open
	///
	///           This method ....
	///
	///  @param[in] none
	///  @return BOOL
	///
	//------------------------------------------------------------------------------------
	BOOL Open	( );
	BOOL Close	( void );


	//------------------------------------------------------------------------------------
	///  @brief   ResetAll
	///
	///           This method ....
	///
	///  @param[in] none
	///  @return none
	///
	//------------------------------------------------------------------------------------
	void  ResetAll				(void);

	//------------------------------------------------------------------------------------
	///  @brief   InitializeAll
	///
	///           This method ....
	///
	///  @param[in] none
	///  @return none
	///
	//------------------------------------------------------------------------------------
	void  InitializeAll			(void);

	//------------------------------------------------------------------------------------
	///  @brief   Save
	///
	///           This method ....
	///
	///  @param[in] none
	///  @return int16_t
	///
	//------------------------------------------------------------------------------------
	int32_t Save				(const std::string& _filename);

	//------------------------------------------------------------------------------------
	///  @brief   Load
	///
	///           This method ....
	///
	///  @param[in] none
	///  @return int16_t
	///
	//------------------------------------------------------------------------------------
	int32_t Load				(const std::string& _filename);
	int32_t Activate();
	int32_t Restore(const std::string& _filename);
	int32_t SaveBak(const std::string& szFilename);



	int32_t System_GetDosierItems					(uint32_t*) const;
	int32_t System_SetDosierItems					(const uint32_t);


//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	DECLARE_INLINE_EX_STRING(std::string, Name)
	DECLARE_INLINE_EX_STRING(std::string, ANNumber)
	DECLARE_INLINE_EX(uint64_t, QMNumber)
	DECLARE_INLINE_EX( BOOL, LineStart)
	DECLARE_INLINE_EX( BOOL, IOLineStartInput) // Externes Signal
	DECLARE_INLINE_EX( BOOL, LineStarted)
	DECLARE_INLINE_EX( BOOL, IOExtAlarmInput)
	DECLARE_INLINE_EX( BOOL, AlarmShutDown)
	DECLARE_INLINE_EX( BOOL, IOAlarmShutDownInput)		
	DECLARE_INLINE_EX( BOOL, IOWindowsShutDownInput)	// Windows ShutDown extern	
	DECLARE_INLINE_EX( BOOL, IOSyncErrorExtern)		// ShutDown extern
	DECLARE_INLINE_EX( BOOL, IOSyncError)				// SyncErrorState
	DECLARE_INLINE_EX( BOOL, IOCustomerAlarmInput)
	DECLARE_INLINE_EX( BOOL, IOLineInvSetpointEnableInput)
	DECLARE_INLINE_EX( BOOL, IOLineSetpointEnableInput)
	DECLARE_INLINE_EX( BOOL, ServiceExtern)
	DECLARE_INLINE_EX( BOOL, ExtLineSetpointState)
	DECLARE_INLINE_EX(BOOL, UPSLowInputState)					// UPS low


	DECLARE_INLINE_EX( BOOL, ExtLineInvSetpointState)
	DECLARE_INLINE_EX( BOOL, ServiceMode)
	DECLARE_INLINE_EX( BOOL, RampUp)
	DECLARE_INLINE_EX( BOOL, RampDown)
	DECLARE_INLINE_EX( BOOL, IORampUpInput)
	DECLARE_INLINE_EX( BOOL, IORampDownInput)
	DECLARE_INLINE_EX( BOOL, IOMainFeederInput)
	DECLARE_INLINE_EX( BOOL, IOSideFeeder1Input)
	DECLARE_INLINE_EX( BOOL, IOSideFeeder2Input)
	DECLARE_INLINE_EX( BOOL, IOOperateAutoInput)
	DECLARE_INLINE_EX( BOOL, IOOperateManualInput)

	DECLARE_INLINE_EX( float32_t, LineMassflow)
	DECLARE_INLINE_EX(float32_t, LineMassflowMF)
	DECLARE_INLINE_EX(float32_t, LineMassflowSF1)
	DECLARE_INLINE_EX(float32_t, LineMassflowSF2)
	DECLARE_INLINE_EX( float32_t, LineSetpoint)
	//DECLARE_INLINE_EX( float32_t, DosiererMischung)
	DECLARE_INLINE_EX( float32_t, MaxLeistung)
	DECLARE_INLINE_EX( float32_t, MinTotband)
	DECLARE_INLINE_EX( uint32_t,  Filterung)
	DECLARE_INLINE_EX( float32_t, RampStep)
	DECLARE_INLINE_EX( uint32_t,  RampDelay)
	DECLARE_INLINE_EX(float32_t, TotalizerAll)
	DECLARE_INLINE_EX(float32_t, TotalizerLine)
	DECLARE_INLINE_EX(float32_t, TotalizerMF)
	DECLARE_INLINE_EX(float32_t, TotalizerSF1)
	DECLARE_INLINE_EX(float32_t, TotalizerSF2)
	DECLARE_INLINE_EX( float32_t, Hysterese)
	DECLARE_INLINE_EX( float32_t, Skalierung)
    DECLARE_INLINE_EX( float32_t, RegeneratPercentage)
    DECLARE_INLINE_EX( float32_t, RecipeSetpoint)
	DECLARE_INLINE_EX(float32_t, ManualSetpoint)
	DECLARE_INLINE_EX(float32_t, ExternalSetpoint)

	DECLARE_INLINE_EX( uint32_t, ProductionTime)
    DECLARE_INLINE_EX( uint32_t, LineDoseStarted)
	DECLARE_INLINE_EX( uint32_t, LineDoseWaitStart)
	DECLARE_INLINE_EX( uint32_t, DoseStarted)
	DECLARE_INLINE_EX( uint32_t, StartDose)
	
	DECLARE_INLINE_EX( BOOL,  Alarm1QuitState)
	DECLARE_INLINE_EX( BOOL,  Alarm03QuitState)
	DECLARE_INLINE_EX( BOOL,  IOAlarm1QuitInput)
	DECLARE_INLINE_EX( BOOL,  IOAlarm03QuitInput)
	DECLARE_INLINE_EX( BOOL,  IOUPSLowInput)	// Totalisator impulse
	DECLARE_INLINE_EX( BOOL, IOOperateAutoOutput)			// IO operate auto output
	DECLARE_INLINE_EX( BOOL, IOServiceModeOutput)			// IO operate service output
	DECLARE_INLINE_EX(BOOL, IOWatchDogOutput)			// IO operate service output


	DECLARE_INLINE_EX( BOOL,  Alarm0State)
	DECLARE_INLINE_EX( BOOL,  Alarm1State)
	DECLARE_INLINE_EX( BOOL,  Alarm2State)
	DECLARE_INLINE_EX( BOOL,  Alarm3State)

	DECLARE_INLINE_EX( BOOL,  IOAlarm0Output)
	DECLARE_INLINE_EX( BOOL,  IOAlarm1Output)
	DECLARE_INLINE_EX( BOOL,  IOAlarm2Output)
	DECLARE_INLINE_EX( BOOL,  IOAlarm3Output)
	DECLARE_INLINE_EX( BOOL,  IOTotalizerPulseOutput);	// Totalisator impulse
	DECLARE_INLINE_EX( float32_t, IOLineSetpointInput);		// ext. Liniensollwert


	DECLARE_INLINE_EX( BOOL,  IOLineReleaseOutput)
	DECLARE_INLINE_EX(uint32_t, FeederAlarm)								// Unterer Gewichtswert
	DECLARE_INLINE_EX(float32_t, TotalizerPulseStep)						// Schrittweite Totalisatorimpuls [kg]
	DECLARE_INLINE_EX(uint32_t, TotalizerPulseDuration)					// Dauer des Impulses [s]


	//DECLARE_INLINE_EX( uint32_t, Alarm)
	DECLARE_INLINE_EX( float32_t, MBLinePercentage);
	DECLARE_INLINE_EX( float32_t, MBLineSetpoint);
	DECLARE_INLINE_EX( float32_t, ExtLineSetpoint)
	DECLARE_INLINE_EX( float32_t, ExtLineOffset)


	DECLARE_INLINE_EX(BOOL, ModuleExternalSetpoint)
	DECLARE_INLINE_EX(BOOL, ModuleSetpointRamp)
	DECLARE_INLINE_EX(BOOL, ModuleLineSetpoint)
	DECLARE_INLINE_EX(BOOL, ModuleLocalSetpoint)
	DECLARE_INLINE_EX(BOOL, ModuleBatcher)
	DECLARE_INLINE_EX(BOOL, ModuleTotalizer)
	DECLARE_INLINE_EX(BOOL, ModuleTotalizerPulse)

	DECLARE_INLINE_EX(BOOL, LineReleaseEnable);      // Enable Line Release
	DECLARE_INLINE_EX(BOOL, LineStartEnable);        // Enable Line Start
	DECLARE_INLINE_EX(BOOL, LineRelease)
	DECLARE_INLINE_EX(BOOL, ExtLineSetpointEnable)
	DECLARE_INLINE_EX(BOOL, ExtLineInvSetpointEnable)


	DECLARE_INLINE_EXPB(PBRampUp)
	DECLARE_INLINE_EXPB(PBRampDown)
	DECLARE_INLINE_EXPB(PBLineStart)
	DECLARE_INLINE_EXPB(PBLineStop)
	DECLARE_INLINE_EXPB(PBAlarm03Quit)
	DECLARE_INLINE_EXPB(PBAlarm1Quit)
	DECLARE_INLINE_EXPB(PBClearTotalizer)
	DECLARE_INLINE_EXPB(PBLineShutDown)
	DECLARE_INLINE_EXPB(PBAlarm0)
	DECLARE_INLINE_EXPB(PBAlarm1)
	DECLARE_INLINE_EXPB(PBAlarm2)
	DECLARE_INLINE_EXPB(PBAlarm3)
	DECLARE_INLINE_EXPB(PBServiceMode)



    int32_t EXSetAlarmBit                     ( const base::eAlarmCategory k, const base::eAlarmError  iBit, BOOL b);
    int32_t EXGetAlarmBit                     ( const base::eAlarmCategory k, const base::eAlarmError  iBit, BOOL *b) const;

	int32_t EXSetLineDoseWaitStart(const int32_t k, const BOOL b);
	int32_t EXGetLineDoseWaitStart(const int32_t k, BOOL* b) const;
	int32_t EXDeleteLineDoseWaitStart(void);
	int32_t EXGetAnyLineDoseWaitStart(BOOL* b) const;


    int32_t EXSetLineDoseStarted       ( const int32_t k, const BOOL b);
    int32_t EXGetLineDoseStarted       ( const int32_t k, BOOL *b) const;
	int32_t EXDeleteLineDoseStarted	   ( void );
    int32_t EXGetAnyLineDoseStarted    ( BOOL *b) const;
    
    int32_t EXSetDoseStarted		   ( const int32_t k, const BOOL b);
    int32_t EXGetDoseStarted		   ( const int32_t k, BOOL *b) const;

	int32_t EXSetStartDose				(const int32_t k, const BOOL b);
	int32_t EXGetStartDose				(const int32_t k, BOOL* b) const;

    int32_t EXGetAnyDoseStarted		  ( BOOL *b) const;
    int32_t EXDeleteDoseStarted		  (  void );
 
	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

	DECLARE_DOSEDATA_STRING(std::string, Name)
	DECLARE_DOSEDATA_STRING(std::string, LogoString)					// LogoString
	DECLARE_DOSEDATA(uint64_t, QMNumber)					// QM-Nummer
	DECLARE_DOSEDATA(base::eLcType, LCTyp);				// Waagentyp
	DECLARE_DOSEDATA(BOOL, ModuleRefillEnable);

	// Parameter
	DECLARE_DOSEDATA(base::eDoseType, DoseType);			// Dosiertyp
	DECLARE_DOSEDATA(base::eFeedingType, FeederType);
	DECLARE_DOSEDATA(float32_t, Percentage)					// Nomineller Prozentwert
	DECLARE_DOSEDATA(float32_t, NominalSetpoint)			// Nomineller Sollwert in kg/h
	DECLARE_DOSEDATA(float32_t, MaxSetpoint)				// Max. Durchsatz fuer Anzeige
	DECLARE_DOSEDATA(float32_t, MaxRotationalSpeed);		// Maximaldrehzahl
	DECLARE_DOSEDATA(float32_t, MassflowFilter);			// Istwertfilter
	DECLARE_DOSEDATA(float32_t, RefillLimitMin);			// Min-Wert
	DECLARE_DOSEDATA(float32_t, RefillLimitMax);			// Maximal  zulaessiges Gewicht
	DECLARE_DOSEDATA(float32_t, RefillLimitMinMin);			// Untere Refill Begrenzung
	DECLARE_DOSEDATA(uint32_t,  RefillTime);				// Max. Befuellzeit
	DECLARE_DOSEDATA(uint32_t,  RefillSwitchDelay);	        // Umschaltverzoegerung
	DECLARE_DOSEDATA(uint32_t,  RefillDebounceMax);				// RefillDebounceMax
	DECLARE_DOSEDATA(uint32_t,  RefillDebounceMin);				// RefillDebounceMin
	DECLARE_DOSEDATA(uint32_t,  RefillFeederEmptyStart);		// MinDurationActive
	DECLARE_DOSEDATA(BOOL,		RefillWeightAboveMax);
	DECLARE_DOSEDATA(BOOL,		RefillWeightBelowMin);
	DECLARE_DOSEDATA(BOOL,		RefillFeederEmptyStarted);
	DECLARE_DOSEDATA(BOOL,		RefillFeederEmptyEnded);

	DECLARE_DOSEDATA(float32_t, NominalAgitator);			// Rührwerk Soll-DriveCommand
	DECLARE_DOSEDATA(float32_t, RefillFeederNominalSpeed);		// RefillFeederNominalSpeed
	DECLARE_DOSEDATA(float32_t, MinSetpointChange)			// Mindest Sollwertänderung
	DECLARE_DOSEDATA(uint32_t, StartupDelay);				// StartupDelay
	DECLARE_DOSEDATA(uint32_t, StartupRamp);				// StartupRamp

	DECLARE_DOSEDATA(uint32_t, EncoderMonitor);				// RotationalSpeedueberwachung 0 ... 1000
	DECLARE_DOSEDATA(uint32_t, LogoEncoderSampleTime)		// Abtastintervall definiert in Logo
	DECLARE_DOSEDATA(BOOL,	   LogoRunControlEnable);		// RunControl Enabled

	DECLARE_DOSEDATA(float32_t, LCCorrectionFactor);		// Korrekturwert Waage
	DECLARE_DOSEDATA(float32_t, LCTaraWeight);				// Taraweight

	DECLARE_DOSEDATA(float32_t, AlarmNoiseLimit);			//Waagenstoergrenze Alarmgrenze
	DECLARE_DOSEDATA(uint32_t, AlarmReactionDelay);
	DECLARE_DOSEDATA(uint32_t, AlarmStartReactionDelay);
	DECLARE_DOSEDATA(uint32_t, AlarmMassflowLow);
	DECLARE_DOSEDATA(uint32_t, AlarmMassflowHigh);
	DECLARE_DOSEDATA(uint32_t, AlarmDriveCommandHigh);
	DECLARE_DOSEDATA(uint32_t, AlarmDriveCommandLow);
	DECLARE_DOSEDATA(uint32_t, AlarmDosePerformance);
	DECLARE_DOSEDATA(uint32_t, AlarmMaxBatchTime)
	DECLARE_DOSEDATA(float32_t, MaxDriveCommand);
	DECLARE_DOSEDATA(BOOL, Regenerat);
	DECLARE_DOSEDATA(uint32_t, IOPulseCounterInput);



	DECLARE_DOSEDATA(float32_t, WbfBeltLoadSetpoint);		// BeltLoadSetpoint in kg/m
	DECLARE_DOSEDATA(float32_t, WbfBeltLoadVol);				// Umschaltgrenze f. volumetrisch
	DECLARE_DOSEDATA(float32_t, WbfReduction);				// Getriebeuntersetzung
	DECLARE_DOSEDATA(uint32_t, WbfWeighingLine);				// Laenge des Wiegebereichs
	DECLARE_DOSEDATA(uint32_t, WbfWheelSize);			// WheelSize
	DECLARE_DOSEDATA(float32_t, WbfTareDriveCommand);		// DriveCommand fuer Tarierung
	DECLARE_DOSEDATA(uint32_t, WbfTareMeasurementTime);			// Messzeit fuer Tarierung
	DECLARE_DOSEDATA(float32_t, WbfAlarmMinBeltLoad);			// Alarm minimale Bandlast
	DECLARE_DOSEDATA(float32_t, WbfAlarmMaxBeltLoad);			// Alarm maximale Bandlast
	DECLARE_DOSEDATA(float32_t, WbfMinDriveCommand);			// minimum drive command
	DECLARE_DOSEDATA(BOOL, WbfWeighingFull);

	// PID
	DECLARE_DOSEDATA(float32_t, PidPropGainGross);			// Prop. Konstante
	DECLARE_DOSEDATA(uint32_t, PidSampleInterval);			// PID-SampleInterval
	DECLARE_DOSEDATA(float32_t, PidPropGainFine);			// Feinverstaerkung
	DECLARE_DOSEDATA(float32_t, PidPropGainSwitchGrossFine);		// Umschalten grob auf fein
	DECLARE_DOSEDATA(float32_t, PidIntegralGain);
	DECLARE_DOSEDATA(float32_t, PidGatefilter);
	DECLARE_DOSEDATA(BOOL, PidDriveCommandInv);


	// VOL
	
	// ICP
	DECLARE_DOSEDATA(float32_t, SteepnessMassflow);


	// LCL / IFL
	DECLARE_DOSEDATA(float32_t, LclWeightMinLevel);
	DECLARE_DOSEDATA(float32_t, LclWeightMaxLevel);
	DECLARE_DOSEDATA(float32_t, LclWeightHysteresis);
	DECLARE_DOSEDATA(BOOL, LclWeightMinLevelActive);
	DECLARE_DOSEDATA(BOOL, LclWeightMaxLevelActive);

	// IFL
	DECLARE_DOSEDATA(float32_t, IflMinLineSetpoint);			// Oberer Gewichtswert
	DECLARE_DOSEDATA(float32_t, IflMaxLineSetpoint);			// Oberer Gewichtswert


	// IFS
	DECLARE_DOSEDATA(BOOL, IfsSwitchHigh);					// Endschalter Einschneckenextruder oben bzw. �berlauf aktiviert
	DECLARE_DOSEDATA(BOOL, IfsSwitchHighEnable);					// Endschalter Einschneckenextruder oben bzw. �berlauf aktiviert
	DECLARE_DOSEDATA(BOOL, IfsSwitchLow);					// Endschalter Einschneckenextruder unter bzw. Leer  aktiviert
	DECLARE_DOSEDATA(BOOL, IfsSwitchLowEnable);					// Endschalter Einschneckenextruder unter bzw. Leer  aktiviert
	DECLARE_DOSEDATA(BOOL, IfsSwitchMax);					// Endschalter Max, oberer Umkehrpunkt  aktiviert
	DECLARE_DOSEDATA(BOOL, IfsSwitchMaxEnable);					// Endschalter Max, oberer Umkehrpunkt  aktiviert
	DECLARE_DOSEDATA(BOOL, IfsSwitchMin);					// Endschalter Min, unterer Umkehrpunkt  aktiviert
	DECLARE_DOSEDATA(BOOL, IfsSwitchMinEnable);					// Endschalter Min, unterer Umkehrpunkt  aktiviert
	DECLARE_DOSEDATA(BOOL, IfsEmptyFeeder);				    // Dosierer leer erreicht
	DECLARE_DOSEDATA(BOOL, IfsOverflowFeeder);				// Dosierer Überlauf
	DECLARE_DOSEDATA(float32_t, IfsReduceFactor);			// Um wieviel kg/h soll der Liniensollwert verringert werden
	DECLARE_DOSEDATA(float32_t, IfsGainFactor);				// Um wieviel kg/h soll der Liniensollwert erhöht werden
	DECLARE_DOSEDATA(uint32_t, IfsFeederOverflowTimeOut);	// Zeit, bis der Zustand "Feeder Überlauf" erkannt wird.
	DECLARE_DOSEDATA(uint32_t, IfsFeederEmptyTimeOut);		// Zeit, bis der Zustand "Feeder leer" erkannt wird.
	DECLARE_DOSEDATA(uint32_t, IfsStepTimeGain);		        // Zeitintervall in Sekunden bis der Sollwert erhöht wird.
	DECLARE_DOSEDATA(uint32_t, IfsStepTimeReduce);		    // Zeitintervall in Sekunden bis der Sollwert verringert wird.
	DECLARE_DOSEDATA(float32_t, IfsSetpointOverflow);		// Neuer Sollwert bei Erreichen von Overflow

	DECLARE_DOSEDATA(uint32_t, IfsDebounceMax);				// Debounce Max / High
	DECLARE_DOSEDATA(uint32_t, IfsDebounceMin);				// Debounce Min / Low



	DECLARE_DOSEDATA(uint32_t, EmptyFeederRuntime);
	DECLARE_DOSEDATA(float32_t, EmptyFeederSpeed);
	DECLARE_DOSEDATA(base::eEmptyFeederType, EmptyFeederRestriction)

	// IO-Belegung
	DECLARE_DOSEDATA(BOOL, IOStartInput)				// Startextern IO
	DECLARE_DOSEDATA(BOOL, IOReleaseInput)				// Release extern (IO)
	DECLARE_DOSEDATA(BOOL, LogoStartState);				// Freigabe Logo (IO)
	DECLARE_DOSEDATA(BOOL, IOVolumetricInput);			// gravimetrisch/volumetrisch extern ausgeloest
	DECLARE_DOSEDATA(BOOL, IOEmptyFeederStartInput);	// Externel empty feeder start
	DECLARE_DOSEDATA(BOOL, IOEmptyFeederStopInput);	// Externel empty feeder stop
	DECLARE_DOSEDATA(BOOL, IORefillAlarmInput)			// Externel Refill alarm
	DECLARE_DOSEDATA(BOOL, IORefillStopInput);			// Refillstopp durch (IO)
	DECLARE_DOSEDATA(BOOL, IORefillRequestInput);		// Refillstart durch (IO)
	DECLARE_DOSEDATA(BOOL, IORefillReleaseInput);		// Refillrelease durch (IO)
	DECLARE_DOSEDATA(BOOL, IORefillInput);				// Refill durch (IO)
	DECLARE_DOSEDATA(BOOL, IORefillMaxSensorInput);			// Refill-MinSensor (IO)
	DECLARE_DOSEDATA(BOOL, IORefillMinSensorInput);			// Refill-MaxSensor (IO)
	DECLARE_DOSEDATA(BOOL, IODriveErrorInput);			// Fehler Drive ( Frequenzumrichter )
	DECLARE_DOSEDATA(BOOL, IOMotorLoadInput);			// Drive Motorlast
	DECLARE_DOSEDATA(BOOL, IOExtAlarmInput);					// Ext. Alarm
	DECLARE_DOSEDATA(BOOL, IODriveErrorPaddleInput);			// Fehler Drive ( Frequenzumrichter )
	DECLARE_DOSEDATA(BOOL, IODriveErrorRefillFeederInput);			// Fehler Drive RefillFeeder ( Frequenzumrichter )
	DECLARE_DOSEDATA(float32_t, IOEncoderSpeedInput);	    // Encoderspeed analog input



	DECLARE_DOSEDATA(BOOL, IODriveReleaseOutput);		// Freigabe zum Motor
	DECLARE_DOSEDATA(BOOL, IOOperateOutput);				// Betriebsleuchte, bei EncoderMonitor
	DECLARE_DOSEDATA(BOOL, IOStartOutput);					// Start

	DECLARE_DOSEDATA(BOOL, IOAlarmOutput);				// Alarm set IO
	DECLARE_DOSEDATA(BOOL, IORefillRequestOutput);		// Refillrequest output (IO)
	//DECLARE_DOSEDATA(BOOL, IOSyncError)					// IOSyncError

	// Aktuelle Zustands- Parameter
	DECLARE_DOSEDATA(BOOL, LCAktiv)						// Waage aktive
	DECLARE_DOSEDATA(BOOL, LCEnable)					// Waage enabled
	DECLARE_DOSEDATA(BOOL, Service)						// Service
	DECLARE_DOSEDATA(BOOL, Release)						// Freigabe
	DECLARE_DOSEDATA(BOOL, Start)						// Startfreigabe extern/Modbus/User
	DECLARE_DOSEDATA(BOOL, RefillControlEnable);		// RefillControlEnable
	DECLARE_DOSEDATA(BOOL, CalibrationState)			// Kalibrieren Zustand : 1 : gestartet
	DECLARE_DOSEDATA(BOOL, TaraState)					// Tarieren Zustand : 1 : gestartet
	DECLARE_DOSEDATA(BOOL, EmptyFeederState)			// EmptyFeeder Zustand : 1 : gestartet
	//DECLARE_DOSEDATA(BOOL, AlarmShutDown)				// Alarmshutdown
	DECLARE_DOSEDATA(BOOL, SlaveMode)					// SlaveMode
	DECLARE_DOSEDATA(BOOL, DriveControlReady)
	DECLARE_DOSEDATA(base::eEncoderState, EncoderState);

	DECLARE_DOSEDATA(float32_t, LCActualWeight)					// Aktuelles Gewicht
	DECLARE_DOSEDATA(float32_t, LCMeanWeight)					// Gemitteltes Gewicht
	DECLARE_DOSEDATA(float32_t, LCRawWeight)						// Rawweight

	DECLARE_DOSEDATA(float32_t, Massflow)						// Aktueller Durchsatz in kg/h
	DECLARE_DOSEDATA(float32_t, ActualSetpoint)					// Aktueller Sollwert
	DECLARE_DOSEDATA(float32_t, ActualDoseperformance)			// Aktueller Doseperformance
	DECLARE_DOSEDATA(float32_t, ActualDriveCommand)				// Aktueller DriveCommand
	DECLARE_DOSEDATA(float32_t, Totalizer);						// Aktueller Verbrauch
	DECLARE_DOSEDATA(float32_t, ActualRotationalSpeed);			// Ist-RotationalSpeed (EncoderMonitor)
	DECLARE_DOSEDATA(float32_t, ActualAgitator);				// Rührwerk Ist-DriveCommand
	DECLARE_DOSEDATA(float32_t, IOActualRefillFeederOutput);	// Aktuelle Schrauben-Nachfüllgeschwindigkeit

	DECLARE_DOSEDATA(float32_t, ActualLogoEncoder);				// RuntimeMonitoring value, 
	DECLARE_DOSEDATA(float32_t, ActualNoise);				// Aktuelle s Rauschen, siehe AlarmNoiseLimit


	DECLARE_DOSEDATA(BOOL, LocalModeState);			// Anlagenmodus lokal/in Linie
	DECLARE_DOSEDATA(BOOL, VolumetricState);			// Betriebsmodus gravimetrisch/volumetrisch
	DECLARE_DOSEDATA(BOOL, GravVolState);	            // Betriebsmodus gravimetrisch, aber temporaer volumetrisch

	DECLARE_DOSEDATA(BOOL, RefillReleaseState);		// Refill Freigabezustand
	DECLARE_DOSEDATA(BOOL, RefillStartState);			// Refill Startzustand, kann gestarted werde
	DECLARE_DOSEDATA(BOOL, RefillStopState);			// Refill Stopzustand, wurde gestoppt
	DECLARE_DOSEDATA(BOOL, RefillState);				// Refill Zustand
	DECLARE_DOSEDATA(BOOL, RefillFeederEnable);	    // Refill screw enable


	DECLARE_DOSEDATA(BOOL, AgitatorEnable);			// Rührwerk enable
	DECLARE_DOSEDATA(BOOL, AgitatorRunState);				// Rührwerk läuft

	DECLARE_DOSEDATA(float32_t, Drehgeber);					// Drehgeber
	DECLARE_DOSEDATA(BOOL, ClearTotalizerState);

	DECLARE_DOSEDATA(base::eWbmState, WbfMinBeltLoadState);		// Aktueller Bandlaststatus
	DECLARE_DOSEDATA(float32_t, WbfActVelocity);			// Aktuelle Bandgeschwindigkeit
	DECLARE_DOSEDATA(float32_t, WbfBeltLoadMassflow);		// Bandlastistwert in kg/m
	DECLARE_DOSEDATA(BOOL, WbfBeltLoadLimit)


	DECLARE_DOSEDATA(float32_t, BatchGrossStream);				// Maximalgeschwindigkeit [%]
	DECLARE_DOSEDATA(float32_t, BatchFineStream);				// Maximalgeschwindigkeit [%]
	DECLARE_DOSEDATA(float32_t, BatchSwitchpoint);				// Abbremsung [s]
	DECLARE_DOSEDATA(uint32_t,  BatchTime);						// BatchTime [s]
	DECLARE_DOSEDATA(float32_t, BatchOverrunCompensation);	    // Batch Nachlaufkompensation [kg}

	DECLARE_DOSEDATA(BOOL, BatchStarted);			    // Batch started
	DECLARE_DOSEDATA(BOOL, BatchFinished);			    // Batch finished
	DECLARE_DOSEDATA(int32_t, GwbExtScale);			        // Gwb ext. Waagenindex

	DECLARE_DOSEDATA(base::eGatefilter, PidGatefilterState);
	DECLARE_DOSEDATA(BOOL, SWRefillRelease)
	DECLARE_DOSEDATA(BOOL, RefillReleaseTrigger)

	DECLARE_DOSEDATAPB(PBDosiererStart)
	DECLARE_DOSEDATAPB(PBDosiererStop)
	DECLARE_DOSEDATAPB(PBRefillStart)
	DECLARE_DOSEDATAPB(PBRefillStop)
	DECLARE_DOSEDATAPB(PBCalibStart)
	DECLARE_DOSEDATAPB(PBCalibStop)
	DECLARE_DOSEDATAPB(PBTareStart)
	DECLARE_DOSEDATAPB(PBTareStop)
	DECLARE_DOSEDATAPB(PBEmptyFeederStart)
	DECLARE_DOSEDATAPB(PBEmptyFeederStop)
	DECLARE_DOSEDATAPB(PBInline)
	DECLARE_DOSEDATAPB(PBLocal)
	DECLARE_DOSEDATAPB(PBVolumetric)
	DECLARE_DOSEDATAPB(PBGravimetric)
	DECLARE_DOSEDATAPB(PBClearTotalizer);
	DECLARE_DOSEDATAPB(PBClearAlarm)				// Alarm zuruecksetzen
	DECLARE_DOSEDATAPB(PBAcknowledgeAlarm)		// Alarm quittieren

    DECLARE_DOSEDATAALARM  (      Alarm);		        // anstehende Alarme
    DECLARE_DOSEDATAALARM  (      ClearAlarm);		// zu loeschende Alarme
	DECLARE_DOSEDATAALARM  (      Warning);           // anstehende Warnungen
    DECLARE_DOSEDATAALARM  (      ClearWarning);		// zu loeschende Warnungen

	int32_t ECSetRefillMinMaxLimit					  ( const int32_t, const float32_t, const float32_t, const float32_t);	// OK
	int32_t ECGetRefillMinMaxLimit					  ( const int32_t, float32_t*, float32_t*, float32_t*) const;	// OK
    int32_t ECSetAlarmCfgBit                         ( const int32_t n, const base::eAlarmCategory k, const base::eAlarmError iBit, BOOL b);
    int32_t ECGetAlarmCfgBit                         ( const int32_t n, const base::eAlarmCategory k, const base::eAlarmError iBit, BOOL *b) const;
	int32_t ECSetAlarmCfg							  ( const int32_t n, const base::eAlarmCategory k, const base::eAlarmErrorBits&);
	int32_t ECGetAlarmCfg							  ( const int32_t n, const base::eAlarmCategory k, base::eAlarmErrorBits*) const;


	DECLARE_DOSEDATA(base::refillsignal::RefillSignalType, RefillSignalCfg)
	DECLARE_DOSEDATA(base::calib::CalibType,  CalibCfg)
	DECLARE_DOSEDATA(base::calib::CalibType, CalibCfgSave)
	DECLARE_DOSEDATA(int32_t, CalibAktIndex)

	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
};





#define DOSEPOINTER (CDose::pDose)
#define DOSEREF (*DOSEPOINTER)

