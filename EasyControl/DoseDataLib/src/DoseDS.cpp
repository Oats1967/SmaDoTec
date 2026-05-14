#include <cassert>
#include "Dose.h"

#include "Dosedata.def"
#include "DoseDataImplMacro.h"

IMPLEMENT_DOSEDSV_STRING( std::string, Name)						// Name
IMPLEMENT_DOSEDSV_STRING( std::string, LogoString)					// LogoString
IMPLEMENT_DOSEDSV(uint64_t, QMNumber)					// QM-Nummer
IMPLEMENT_DOSEDSV(base::eLcType, LCTyp)				// Waagentyp

IMPLEMENT_DOSEDSV(BOOL, ModuleRefillEnable)

// Parameter
IMPLEMENT_DOSEDSV(base::eDoseType, DoseType);			// Dosiertyp
IMPLEMENT_DOSEDSV(base::eFeedingType, FeederType);
IMPLEMENT_DOSEDSV(float32_t, Percentage)			// Nomineller Prozentwert
IMPLEMENT_DOSEDSV(float32_t, NominalSetpoint)			// Nomineller Sollwert in kg/h
IMPLEMENT_DOSEDSV(float32_t, MaxSetpoint)				// Max. Durchsatz fuer Anzeige
IMPLEMENT_DOSEDSV(float32_t, MaxRotationalSpeed);		// Maximaldrehzahl
IMPLEMENT_DOSEDSV(float32_t, MassflowFilter);			// Istwertfilter

IMPLEMENT_DOSEDSV(float32_t, RefillLimitMin);			// Min-Wert
IMPLEMENT_DOSEDSV(float32_t, RefillLimitMax);			// Maximal  zulaessiges Gewicht
IMPLEMENT_DOSEDSV(float32_t, RefillLimitMinMin);			// Untere Refill Begrenzung
IMPLEMENT_DOSEDSV(uint32_t, RefillTime);				// Max. Befuellzeit
IMPLEMENT_DOSEDSV(uint32_t, RefillSwitchDelay);	        // Umschaltverzoegerung
IMPLEMENT_DOSEDSV(uint32_t, RefillDebounceMax);				// RefillDebounceMax
IMPLEMENT_DOSEDSV(uint32_t, RefillDebounceMin);				// RefillDebounceMin
IMPLEMENT_DOSEDSV(uint32_t, RefillFeederEmptyStart);		// MinDurationActive
IMPLEMENT_DOSEDSV(BOOL,		RefillWeightAboveMax);
IMPLEMENT_DOSEDSV(BOOL,		RefillWeightBelowMin);
IMPLEMENT_DOSEDSV(BOOL,		RefillFeederEmptyStarted);
IMPLEMENT_DOSEDSV(BOOL,		RefillFeederEmptyEnded);


IMPLEMENT_DOSEDSV(float32_t, MinSetpointChange)			// Mindest Sollwert�nderung
IMPLEMENT_DOSEDSV(float32_t, NominalAgitator);			// R�hrwerk Soll-DriveCommand
IMPLEMENT_DOSEDSV(float32_t, RefillFeederNominalSpeed);	// RefillFeederNominalSpeed
IMPLEMENT_DOSEDSV(uint32_t, StartupDelay);				// StartupDelay
IMPLEMENT_DOSEDSV(uint32_t, StartupRamp);				// StartupRamp


IMPLEMENT_DOSEDSV(uint32_t, EncoderMonitor);				// RotationalSpeedueberwachung 0 ... 1000
IMPLEMENT_DOSEDSV(uint32_t, LogoEncoderSampleTime)		// Abtastintervall definiert in Logo
IMPLEMENT_DOSEDSV(BOOL,		LogoRunControlEnable);		// RunControl Enabled

IMPLEMENT_DOSEDSV(float32_t, LCCorrectionFactor);		// Korrekturwert Waage
IMPLEMENT_DOSEDSV(float32_t, LCTaraWeight);				// Taraweight

IMPLEMENT_DOSEDSV(float32_t, AlarmNoiseLimit);			//Waagenstoergrenze Alarmgrenze
IMPLEMENT_DOSEDSV(uint32_t, AlarmReactionDelay);
IMPLEMENT_DOSEDSV(uint32_t, AlarmStartReactionDelay);
IMPLEMENT_DOSEDSV(uint32_t, AlarmMassflowLow);
IMPLEMENT_DOSEDSV(uint32_t, AlarmMassflowHigh);
IMPLEMENT_DOSEDSV(uint32_t, AlarmDriveCommandHigh);
IMPLEMENT_DOSEDSV(uint32_t, AlarmDriveCommandLow);
IMPLEMENT_DOSEDSV(uint32_t, AlarmDosePerformance);
IMPLEMENT_DOSEDSV(uint32_t, AlarmMaxBatchTime);

IMPLEMENT_DOSEDSV(float32_t, MaxDriveCommand);
IMPLEMENT_DOSEDSV(BOOL, Regenerat);
IMPLEMENT_DOSEDSV(uint32_t, IOPulseCounterInput);

IMPLEMENT_DOSEDSV(float32_t, WbfBeltLoadSetpoint);		// BeltLoadSetpoint in kg/m
IMPLEMENT_DOSEDSV(float32_t, WbfBeltLoadVol);				// Umschaltgrenze f. volumetrisch
IMPLEMENT_DOSEDSV(float32_t, WbfReduction);				// Getriebeuntersetzung
IMPLEMENT_DOSEDSV(uint32_t, WbfWeighingLine);				// Laenge des Wiegebereichs
IMPLEMENT_DOSEDSV(uint32_t, WbfWheelSize);			// WheelSize
IMPLEMENT_DOSEDSV(float32_t, WbfTareDriveCommand);		// DriveCommand fuer Tarierung
IMPLEMENT_DOSEDSV(uint32_t, WbfTareMeasurementTime);			// Messzeit fuer Tarierung
IMPLEMENT_DOSEDSV(float32_t, WbfAlarmMinBeltLoad);			// Alarm minimale Bandlast
IMPLEMENT_DOSEDSV(float32_t, WbfAlarmMaxBeltLoad);			// Alarm maximale Bandlast
IMPLEMENT_DOSEDSV(float32_t, WbfMinDriveCommand);			// minimum drive command
IMPLEMENT_DOSEDSV(BOOL, WbfWeighingFull);

// PID
IMPLEMENT_DOSEDSV(float32_t, PidPropGainGross);			// Prop. Konstante
IMPLEMENT_DOSEDSV(uint32_t, PidSampleInterval);			// PID-SampleInterval
IMPLEMENT_DOSEDSV(float32_t, PidPropGainFine);			// Feinverstaerkung
IMPLEMENT_DOSEDSV(float32_t, PidPropGainSwitchGrossFine);		// Umschalten grob auf fein
IMPLEMENT_DOSEDSV(float32_t, PidIntegralGain);
IMPLEMENT_DOSEDSV(float32_t, PidGatefilter);
IMPLEMENT_DOSEDSV(BOOL, PidDriveCommandInv);


// VOL

// ICP
IMPLEMENT_DOSEDSV(float32_t, SteepnessMassflow);

// LCL / IFL
IMPLEMENT_DOSEDSV(float32_t, LclWeightMinLevel);
IMPLEMENT_DOSEDSV(float32_t, LclWeightMaxLevel);
IMPLEMENT_DOSEDSV(float32_t, LclWeightHysteresis);
IMPLEMENT_DOSEDSV(BOOL, LclWeightMinLevelActive);
IMPLEMENT_DOSEDSV(BOOL, LclWeightMaxLevelActive);

// IFL
IMPLEMENT_DOSEDSV(float32_t, IflMinLineSetpoint);			// Oberer Gewichtswert
IMPLEMENT_DOSEDSV(float32_t, IflMaxLineSetpoint);			// Oberer Gewichtswert

// IFS
IMPLEMENT_DOSEDSV(BOOL, IfsSwitchHigh);					// Endschalter Einschneckenextruder oben bzw. �berlauf aktiviert
IMPLEMENT_DOSEDSV(BOOL, IfsSwitchHighEnable);					// Endschalter Einschneckenextruder oben bzw. �berlauf aktiviert
IMPLEMENT_DOSEDSV(BOOL, IfsSwitchLow);					// Endschalter Einschneckenextruder unter bzw. Leer  aktiviert
IMPLEMENT_DOSEDSV(BOOL, IfsSwitchLowEnable);					// Endschalter Einschneckenextruder unter bzw. Leer  aktiviert
IMPLEMENT_DOSEDSV(BOOL, IfsSwitchMax);					// Endschalter Max, oberer Umkehrpunkt  aktiviert
IMPLEMENT_DOSEDSV(BOOL, IfsSwitchMaxEnable);					// Endschalter Max, oberer Umkehrpunkt  aktiviert
IMPLEMENT_DOSEDSV(BOOL, IfsSwitchMin);					// Endschalter Min, unterer Umkehrpunkt  aktiviert
IMPLEMENT_DOSEDSV(BOOL, IfsSwitchMinEnable);					// Endschalter Min, unterer Umkehrpunkt  aktiviert
IMPLEMENT_DOSEDSV(BOOL, IfsEmptyFeeder);				    // Dosierer leer erreicht
IMPLEMENT_DOSEDSV(BOOL, IfsOverflowFeeder);				// Dosierer Überlauf

IMPLEMENT_DOSEDSV(float32_t, IfsReduceFactor);			// Um wieviel kg/h soll der Liniensollwert verringert werden
IMPLEMENT_DOSEDSV(float32_t, IfsGainFactor);				// Um wieviel kg/h soll der Liniensollwert erhöht werden
IMPLEMENT_DOSEDSV(uint32_t, IfsFeederOverflowTimeOut);	// Zeit, bis der Zustand "Feeder Überlauf" erkannt wird.
IMPLEMENT_DOSEDSV(uint32_t, IfsFeederEmptyTimeOut);		// Zeit, bis der Zustand "Feeder leer" erkannt wird.
IMPLEMENT_DOSEDSV(uint32_t, IfsStepTimeGain);		        // Zeitintervall in Sekunden bis der Sollwert erhöht wird.
IMPLEMENT_DOSEDSV(uint32_t, IfsStepTimeReduce);		    // Zeitintervall in Sekunden bis der Sollwert verringert wird.
IMPLEMENT_DOSEDSV(float32_t, IfsSetpointOverflow);		// Neuer Sollwert bei Erreichen von Overflow
IMPLEMENT_DOSEDSV(uint32_t, IfsDebounceMax);				// Debounce Max / High
IMPLEMENT_DOSEDSV(uint32_t, IfsDebounceMin);				// Debounce Min / Low


IMPLEMENT_DOSEDSV(uint32_t, EmptyFeederRuntime);
IMPLEMENT_DOSEDSV(float32_t, EmptyFeederSpeed);
IMPLEMENT_DOSEDSV(base::eEmptyFeederType, EmptyFeederRestriction)

// IO-Belegung
IMPLEMENT_DOSEDSV(BOOL, IOStartInput)				// Startextern IO
IMPLEMENT_DOSEDSV(BOOL, IOReleaseInput)				// Release extern (IO)
IMPLEMENT_DOSEDSV(BOOL, LogoStartState);				// Freigabe Logo (IO)
IMPLEMENT_DOSEDSV(BOOL, IOVolumetricInput);			// gravimetrisch/volumetrisch extern ausgeloest
IMPLEMENT_DOSEDSV(BOOL, IOEmptyFeederStartInput);	// Externel empty feeder start
IMPLEMENT_DOSEDSV(BOOL, IOEmptyFeederStopInput);	// Externel empty feeder stop
IMPLEMENT_DOSEDSV(BOOL, IORefillAlarmInput)			// Externel Refill alarm
IMPLEMENT_DOSEDSV(BOOL, IORefillStopInput);			// Refillstopp durch (IO)
IMPLEMENT_DOSEDSV(BOOL, IORefillRequestInput);		// Refillstart durch (IO)
IMPLEMENT_DOSEDSV(BOOL, IORefillReleaseInput);		// Refillrelease durch (IO)
IMPLEMENT_DOSEDSV(BOOL, IORefillInput);				// Refill durch (IO)

IMPLEMENT_DOSEDSV(BOOL, IORefillMaxSensorInput);			// Refill-MinSensor (IO)
IMPLEMENT_DOSEDSV(BOOL, IORefillMinSensorInput);			// Refill-MaxSensor (IO)
IMPLEMENT_DOSEDSV(BOOL, IODriveErrorInput);			// Fehler Drive ( Frequenzumrichter )
IMPLEMENT_DOSEDSV(BOOL, IOMotorLoadInput);			// Drive Motorlast
IMPLEMENT_DOSEDSV(BOOL, IOExtAlarmInput);				// Ext. Alarm
IMPLEMENT_DOSEDSV(BOOL, IODriveErrorPaddleInput);			// Fehler Drive paddle ( Frequenzumrichter )
IMPLEMENT_DOSEDSV(BOOL, IODriveErrorRefillFeederInput);			// Fehler Drive RefillFeeder ( Frequenzumrichter )
IMPLEMENT_DOSEDSV(float32_t, IOEncoderSpeedInput);	    // Encoderspeed analog input




IMPLEMENT_DOSEDSV(BOOL, IODriveReleaseOutput)		// Freigabe zum Motor
IMPLEMENT_DOSEDSV(BOOL, IOOperateOutput)			// Betriebsleuchte, bei EncoderMonitor
IMPLEMENT_DOSEDSV(BOOL, IOStartOutput);				// Start
IMPLEMENT_DOSEDSV(BOOL, IOAlarmOutput);				// Alarm set IO
IMPLEMENT_DOSEDSV(BOOL, IORefillRequestOutput);		// Refillrequest output (IO)

// Aktuelle Zustands- Parameter
IMPLEMENT_DOSEDSV(BOOL, LCEnable)					// Waage enabled
IMPLEMENT_DOSEDSV(BOOL, LCAktiv)					// Waage aktiv
IMPLEMENT_DOSEDSV(BOOL, Service)					// Service
IMPLEMENT_DOSEDSV(BOOL, Release)					// Freigabe
IMPLEMENT_DOSEDSV(BOOL, Start)						// Startfreigabe extern/Modbus/User
IMPLEMENT_DOSEDSV(BOOL, RefillControlEnable);		// RefillControlEnable
IMPLEMENT_DOSEDSV(BOOL, CalibrationState)			// Kalibrieren Zustand : 1 : gestartet
IMPLEMENT_DOSEDSV(BOOL, TaraState)					// Tarieren Zustand : 1 : gestartet
IMPLEMENT_DOSEDSV(BOOL, EmptyFeederState)			// EmptyFeeder Zustand : 1 : gestartet
IMPLEMENT_DOSEDSV(BOOL, SlaveMode)					// SlaveMode
IMPLEMENT_DOSEDSV(BOOL, DriveControlReady)
IMPLEMENT_DOSEDSV(base::eEncoderState, EncoderState);

IMPLEMENT_DOSEDSV(float32_t, LCActualWeight)					// Aktuelles Gewicht
IMPLEMENT_DOSEDSV(float32_t, LCMeanWeight)					// Gemitteltes Gewicht
IMPLEMENT_DOSEDSV(float32_t, LCRawWeight)						// Rawweight

IMPLEMENT_DOSEDSV(float32_t, Massflow)					// Aktueller Durchsatz in kg/h
IMPLEMENT_DOSEDSV(float32_t, ActualSetpoint)				// Aktueller Sollwert
IMPLEMENT_DOSEDSV(float32_t, ActualDoseperformance)		// Aktueller Doseperformance
IMPLEMENT_DOSEDSV(float32_t, ActualDriveCommand)			// Aktueller DriveCommand
IMPLEMENT_DOSEDSV(float32_t, Totalizer);				// Aktueller Verbrauch
IMPLEMENT_DOSEDSV(float32_t, ActualRotationalSpeed);	// Ist-RotationalSpeed (EncoderMonitor)
IMPLEMENT_DOSEDSV(float32_t, ActualAgitator);			// R�hrwerk Ist-DriveCommand
IMPLEMENT_DOSEDSV(float32_t, IOActualRefillFeederOutput);	// Aktuelle Schrauben-Nachf�llgeschwindigkeit

IMPLEMENT_DOSEDSV(float32_t, ActualLogoEncoder);				// RuntimeMonitoring value, 
IMPLEMENT_DOSEDSV(float32_t, ActualNoise)				// Aktuelle s Rauschen, siehe AlarmNoiseLimit

IMPLEMENT_DOSEDSV(BOOL, LocalModeState);			// Anlagenmodus lokal/in Linie
IMPLEMENT_DOSEDSV(BOOL, VolumetricState);			// Betriebsmodus gravimetrisch/volumetrisch
IMPLEMENT_DOSEDSV(BOOL, GravVolState);	            // Betriebsmodus gravimetrisch, aber temporaer volumetrisch

IMPLEMENT_DOSEDSV(BOOL, RefillReleaseState);		// Refill Freigabezustand
IMPLEMENT_DOSEDSV(BOOL, RefillStartState);			// Refill Startzustand, kann gestarted werde
IMPLEMENT_DOSEDSV(BOOL, RefillStopState);			// Refill Stopzustand, wurde gestoppt
IMPLEMENT_DOSEDSV(BOOL, RefillState);				// Refill Zustand
IMPLEMENT_DOSEDSV(BOOL, RefillFeederEnable);	    // Refill screw enable


IMPLEMENT_DOSEDSV(BOOL, AgitatorEnable);			// R�hrwerk enable
IMPLEMENT_DOSEDSV(BOOL, AgitatorRunState);				// R�hrwerk l�uft

IMPLEMENT_DOSEDSV(float32_t, Drehgeber);					// Drehgeber
IMPLEMENT_DOSEDSV(BOOL, ClearTotalizerState);

IMPLEMENT_DOSEDSV(base::eWbmState, WbfMinBeltLoadState);		// Aktueller Bandlaststatus
IMPLEMENT_DOSEDSV(float32_t, WbfActVelocity);			// Aktuelle Bandgeschwindigkeit
IMPLEMENT_DOSEDSV(float32_t, WbfBeltLoadMassflow);		// Bandlastistwert in kg/m
IMPLEMENT_DOSEDSV(BOOL, WbfBeltLoadLimit)


IMPLEMENT_DOSEDSV(float32_t, BatchGrossStream);				// Maximalgeschwindigkeit [%]
IMPLEMENT_DOSEDSV(float32_t, BatchFineStream);				// Maximalgeschwindigkeit [%]
IMPLEMENT_DOSEDSV(float32_t, BatchSwitchpoint);				// Abbremsung [s]
IMPLEMENT_DOSEDSV(uint32_t, BatchTime);						// BatchTime [s]
IMPLEMENT_DOSEDSV(float32_t, BatchOverrunCompensation);	    // Batch Nachlaufkompensation [kg}


IMPLEMENT_DOSEDSV(BOOL, BatchStarted);
IMPLEMENT_DOSEDSV(BOOL, BatchFinished);
IMPLEMENT_DOSEDSV(int32_t, GwbExtScale);			    // Gwb ext. Waagenindex
IMPLEMENT_DOSEDSV(BOOL, SWRefillRelease);		// StartFreigabe durch Modbus
IMPLEMENT_DOSEDSV(BOOL, RefillReleaseTrigger);		// StartFreigabe durch Modbus


IMPLEMENT_DOSEDSV(base::eGatefilter, PidGatefilterState);


IMPLEMENT_DOSEDSVPB(PBDosiererStart)
IMPLEMENT_DOSEDSVPB(PBDosiererStop)
IMPLEMENT_DOSEDSVPB(PBRefillStart)
IMPLEMENT_DOSEDSVPB(PBRefillStop)
IMPLEMENT_DOSEDSVPB(PBCalibStart)
IMPLEMENT_DOSEDSVPB(PBCalibStop)
IMPLEMENT_DOSEDSVPB(PBTareStart)
IMPLEMENT_DOSEDSVPB(PBTareStop)
IMPLEMENT_DOSEDSVPB(PBEmptyFeederStart)
IMPLEMENT_DOSEDSVPB(PBEmptyFeederStop)
IMPLEMENT_DOSEDSVPB(PBInline)
IMPLEMENT_DOSEDSVPB(PBLocal)
IMPLEMENT_DOSEDSVPB(PBVolumetric)
IMPLEMENT_DOSEDSVPB(PBGravimetric)
IMPLEMENT_DOSEDSVPB(PBClearTotalizer);
IMPLEMENT_DOSEDSVPB(PBClearAlarm)				// Alarm zuruecksetzen
IMPLEMENT_DOSEDSVPB(PBAcknowledgeAlarm);		// Alarm quittieren

IMPLEMENT_DOSEDSVALARM(Alarm);		                // Alarme
IMPLEMENT_DOSEDSVALARM(ClearAlarm);	            // Alarme loeschen
IMPLEMENT_DOSEDSVALARM(Warning);		            // Warnung
IMPLEMENT_DOSEDSVALARM(ClearWarning);	            // Warnung loeschen


IMPLEMENT_DOSEDSV(int32_t, CalibAktIndex)
IMPLEMENT_DOSEDSV(base::calib::CalibType, CalibCfg)
IMPLEMENT_DOSEDSV(base::calib::CalibType, CalibCfgSave)
IMPLEMENT_DOSEDSV(base::refillsignal::RefillSignalType, RefillSignalCfg)

//*******************************************************************************************************************************
//*******************************************************************************************************************************
int32_t Dose_DSVSetRefillMinMaxLimit			( const int32_t index, const float32_t f1, const float32_t f2, const float32_t f3)
{
	assert(DOSEPOINTER);
	return  DOSEREF.ECSetRefillMinMaxLimit (index, f1, f2, f3);
}
//*******************************************************************************************************************************
//*******************************************************************************************************************************
int32_t Dose_DSVGetRefillMinMaxLimit			( const int32_t index, float32_t *f1, float32_t *f2, float32_t *f3)
{
	assert(f1);
	assert(f2);
	assert(f3);
	*f3 = *f2 = *f1 = 0.0f;
	assert(DOSEPOINTER);
	return DOSEREF.ECGetRefillMinMaxLimit (index, f1, f2, f3);
}
//*******************************************************************************************************************************
//*******************************************************************************************************************************
int32_t Dose_DSVSetAlarmCfgBit ( const int32_t index,  const base::eAlarmCategory k, const base::eAlarmError iBit, BOOL b)
{
	assert(DOSEPOINTER);
	return DOSEREF.ECSetAlarmCfgBit  (index,k, iBit, b);
}
//*******************************************************************************************************************************
//*******************************************************************************************************************************
int32_t Dose_DSVGetAlarmCfgBit ( const int32_t index,  const base::eAlarmCategory k, const base::eAlarmError iBit, BOOL *b)
{
	assert(DOSEPOINTER);
	return DOSEREF.ECGetAlarmCfgBit   (index,  k, iBit, b);
}
//*******************************************************************************************************************************
//*******************************************************************************************************************************
int32_t Dose_DSVSetAlarmCfg(const int32_t index, const base::eAlarmCategory k, const base::eAlarmErrorBits& b)
{
	assert(DOSEPOINTER);
	return DOSEREF.ECSetAlarmCfg(index, k, b);
}
//*******************************************************************************************************************************
//*******************************************************************************************************************************
int32_t Dose_DSVGetAlarmCfg(const int32_t index, const base::eAlarmCategory k, base::eAlarmErrorBits* b)
{
	assert(DOSEPOINTER);
	return DOSEREF.ECGetAlarmCfg(index, k, b);
}
//*******************************************************************************************************************************
//*******************************************************************************************************************************
int32_t Dose_DSVSetAllAlarmCfg(const int32_t index, const base::eAlarmErrorBits& b)
{
	assert(DOSEPOINTER);
	DECLARE_ALARMCATEGORY(field)

	for (const auto& item : field)
	{
		Dose_DSVSetAlarmCfg(index, item, b);
	}
	return DOSIERUNG_OK;
}
//*******************************************************************************************************************************
//*******************************************************************************************************************************
int32_t Dose_DSVMaskAlarmCfg(const int32_t index, const base::eAlarmCategory k, const base::eAlarmErrorBits& mask)
{
	DECLARE_ALARMCATEGORY(field)

	base::eAlarmErrorBits b;
	DOSEREF.ECGetAlarmCfg(index, k, &b);
	b.allflags &= mask.allflags;
	DOSEREF.ECSetAlarmCfg(index, k, b);
	return DOSIERUNG_OK;
}

//*******************************************************************************************************************************
//*******************************************************************************************************************************
int32_t Dose_DSVMaskAllAlarmCfg(const int32_t index, const base::eAlarmErrorBits& mask)
{
	DECLARE_ALARMCATEGORY(field)
	for (const auto& item : field)
	{
		Dose_DSVMaskAlarmCfg(index, item, mask);
	}
	return DOSIERUNG_OK;
}
