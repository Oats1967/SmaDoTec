//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Declaration of module dosetypen.h
///
/// @file   dosetypen.h
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

#include "BASE/Base.h"
#include "ECTypes.h"
#include "BASE/include/RemoteConfig.h"
#include "BASE/include/AlarmTypes.h"
#include "BASE/include/Gatefilter.h"
#include "BASE/include/DoseType.h"
#include "BASE/include/EmptyFeederType.h"
#include "BASE/include/LCType.h"
#include "BASE/include/WbmStates.h"
#include "BASE/include/EncoderState.h"
#include "BASE/include/FeedingType.h"
#include "BASE/include/CalibType.h"
#include "BASE/include/RefillSignalType.h"


#define PROGRAMFILE                 "EasyControl"

// CDosierData-Befehlsziel


#define DECLARE_VARIABLE(t,a) ECTypes<t> a;
#define	DECLARE_ALARM(a)    DECLARE_VARIABLE(base::eAlarmErrorBits, a)
#define DECLARE_PB(a)	    BOOL a;


struct CGlobalSystemItem
{
	uint32_t		    sMaxItems;
    DECLARE_VARIABLE( int32_t, RemoteStatus);
	char_t		        cDummy[100];
};


struct CGlobalDoseItem
{
	DECLARE_VARIABLE( TSTRINGCHARS,	Name )						// Name
	DECLARE_VARIABLE( STRINGCHARS,	LogoString)					// LogoString
	DECLARE_VARIABLE( uint64_t,		QMNumber)					// QM-Nummer
	DECLARE_VARIABLE(base::eLcType, LCTyp);					// Waagentyp
	DECLARE_VARIABLE(BOOL, ModuleRefillEnable);					// Module Refill enable

	// Parameter
	DECLARE_VARIABLE(base::eDoseType, DoseType);			// Dosiertyp
	DECLARE_VARIABLE(base::eFeedingType, FeederType);		// FeederType
	DECLARE_VARIABLE(float32_t, Percentage)					// Nomineller Prozentwert
	DECLARE_VARIABLE(float32_t, NominalSetpoint)			// Nomineller Sollwert in kg/h
	DECLARE_VARIABLE(float32_t, MaxSetpoint)				// Max. Durchsatz fuer Anzeige
	DECLARE_VARIABLE(float32_t, MaxRotationalSpeed);		// Maximaldrehzahl
	DECLARE_VARIABLE(float32_t, MassflowFilter);			// Istwertfilter

	DECLARE_VARIABLE(float32_t, RefillLimitMin);				// Min-Wert
	DECLARE_VARIABLE(float32_t, RefillLimitMax);				// Maximal  zulaessiges Gewicht
	DECLARE_VARIABLE(float32_t, RefillLimitMinMin);				// Untere Refill Begrenzung
	DECLARE_VARIABLE(uint32_t,  RefillTime);					// Max. Befuellzeit
	DECLARE_VARIABLE(uint32_t,  RefillSwitchDelay);				// Umschaltverzoegerung
	DECLARE_VARIABLE(uint32_t,  RefillDebounceMax);				// RefillDebounceMax
	DECLARE_VARIABLE(uint32_t,  RefillDebounceMin);				// RefillDebounceMin
	DECLARE_VARIABLE(uint32_t,  RefillFeederEmptyStart);		// Zeit, bis bei MinSensor == FALSE Dosierer leer ausgeloest wird
	DECLARE_VARIABLE(uint32_t,  RefillDummy1);					// 
	DECLARE_VARIABLE(uint32_t,  RefillDummy2);					// 
	DECLARE_VARIABLE(BOOL,		RefillWeightAboveMax);
	DECLARE_VARIABLE(BOOL,		RefillWeightBelowMin);
	DECLARE_VARIABLE(BOOL,		RefillFeederEmptyStarted);
	DECLARE_VARIABLE(BOOL,		RefillFeederEmptyEnded);

	DECLARE_VARIABLE(float32_t, NominalAgitator);				// R�hrwerk Soll-DriveCommand
	DECLARE_VARIABLE(float32_t, RefillFeederNominalSpeed);			// RefillFeederNominalSpeed
	DECLARE_VARIABLE(float32_t, MinSetpointChange)				// Mindest Sollwert�nderung
	DECLARE_VARIABLE(uint32_t, StartupDelay);					// StartupDelay
	DECLARE_VARIABLE(uint32_t, StartupRamp);					// StartupRamp

	DECLARE_VARIABLE(uint32_t,  EncoderMonitor);			// RotationalSpeedueberwachung 0 ... 1000
	DECLARE_VARIABLE(uint32_t,  LogoEncoderSampleTime)		// Abtastintervall definiert in Logo
	DECLARE_VARIABLE(BOOL,		LogoRunControlEnable);		// RunControl Enabled

	DECLARE_VARIABLE(float32_t, LCCorrectionFactor);		// Korrekturwert Waage
	DECLARE_VARIABLE(float32_t, LCTaraWeight);				// Taraweight

	DECLARE_VARIABLE(float32_t, AlarmNoiseLimit);			//Waagenstoergrenze Alarmgrenze
	DECLARE_VARIABLE(uint32_t,  AlarmReactionDelay);
	DECLARE_VARIABLE(uint32_t,  AlarmStartReactionDelay);
	DECLARE_VARIABLE(uint32_t,  AlarmMassflowLow);
	DECLARE_VARIABLE(uint32_t,  AlarmMassflowHigh);
	DECLARE_VARIABLE(uint32_t,  AlarmDriveCommandHigh);
	DECLARE_VARIABLE(uint32_t,  AlarmDriveCommandLow);
	DECLARE_VARIABLE(uint32_t,  AlarmDosePerformance);
	DECLARE_VARIABLE(uint32_t,  AlarmMaxBatchTime);

	DECLARE_VARIABLE(float32_t, MaxDriveCommand);
	DECLARE_VARIABLE(BOOL,		Regenerat);

	// Pulsecounter
	DECLARE_VARIABLE(uint32_t, IOPulseCounterInput);

	DECLARE_VARIABLE(float32_t, WbfBeltLoadSetpoint);		// BeltLoadSetpoint in kg/m
	DECLARE_VARIABLE(float32_t, WbfBeltLoadVol);			// Umschaltgrenze f. volumetrisch
	DECLARE_VARIABLE(float32_t, WbfReduction);				// Getriebeuntersetzung
	DECLARE_VARIABLE(uint32_t,	WbfWeighingLine);				// Laenge des Wiegebereichs
	DECLARE_VARIABLE(uint32_t,	WbfWheelSize);			// WheelSize
	DECLARE_VARIABLE(float32_t, WbfTareDriveCommand);		// DriveCommand fuer Tarierung
	DECLARE_VARIABLE(uint32_t,	WbfTareMeasurementTime);			// Messzeit fuer Tarierung
	DECLARE_VARIABLE(float32_t, WbfAlarmMinBeltLoad);			// Alarm minimale Bandlast
	DECLARE_VARIABLE(float32_t, WbfAlarmMaxBeltLoad);			// Alarm maximale Bandlast
	DECLARE_VARIABLE(float32_t, WbfMinDriveCommand);			// minimum drive command
	DECLARE_VARIABLE(BOOL,		WbfWeighingFull);

	// PID
	DECLARE_VARIABLE(float32_t, PidPropGainGross);			// Prop. Konstante
	DECLARE_VARIABLE(uint32_t, PidSampleInterval);			// PID-SampleInterval
	DECLARE_VARIABLE(float32_t, PidPropGainFine);			// Feinverstaerkung
	DECLARE_VARIABLE(float32_t, PidPropGainSwitchGrossFine);		// Umschalten grob auf fein
	DECLARE_VARIABLE(float32_t, PidIntegralGain);
	DECLARE_VARIABLE(float32_t, PidGatefilter);
	DECLARE_VARIABLE(BOOL, PidDriveCommandInv);



	// ICP
	DECLARE_VARIABLE(float32_t, SteepnessMassflow);

	// LCL/IFL
	DECLARE_VARIABLE(float32_t, LclWeightMinLevel);
	DECLARE_VARIABLE(float32_t, LclWeightMaxLevel);
	DECLARE_VARIABLE(float32_t, LclWeightHysteresis);
	DECLARE_VARIABLE(BOOL, LclWeightMinLevelActive);
	DECLARE_VARIABLE(BOOL, LclWeightMaxLevelActive);


	// IFL
	DECLARE_VARIABLE(float32_t, IflMinLineSetpoint);			// Oberer Gewichtswert
	DECLARE_VARIABLE(float32_t, IflMaxLineSetpoint);			// Oberer Gewichtswert

	// IFS
	DECLARE_VARIABLE(BOOL, IfsSwitchHigh);					// Endschalter Einschneckenextruder oben bzw. �berlauf aktiviert
	DECLARE_VARIABLE(BOOL, IfsSwitchHighEnable);			// Endschalter Einschneckenextruder oben bzw. �berlauf aktiviert
	DECLARE_VARIABLE(BOOL, IfsSwitchLow);					// Endschalter Einschneckenextruder unter bzw. Leer  aktiviert
	DECLARE_VARIABLE(BOOL, IfsSwitchLowEnable);				// Endschalter Einschneckenextruder unter bzw. Leer  aktiviert
	DECLARE_VARIABLE(BOOL, IfsSwitchMax);					// Endschalter Max, oberer Umkehrpunkt  aktiviert
	DECLARE_VARIABLE(BOOL, IfsSwitchMaxEnable);				// Endschalter Max, oberer Umkehrpunkt  aktiviert
	DECLARE_VARIABLE(BOOL, IfsSwitchMin);					// Endschalter Min, unterer Umkehrpunkt  aktiviert
	DECLARE_VARIABLE(BOOL, IfsSwitchMinEnable);				// Endschalter Min, unterer Umkehrpunkt  aktiviert
	DECLARE_VARIABLE(BOOL, IfsEmptyFeeder);				    // Dosierer leer erreicht
	DECLARE_VARIABLE(BOOL, IfsOverflowFeeder);				// Dosierer Überlauf


	DECLARE_VARIABLE(float32_t, IfsReduceFactor);			// Pegel ist �ber Max, Sollwert wird um Reducefaktor verringert
	DECLARE_VARIABLE(float32_t, IfsGainFactor);				// Pegel ist unter Min gefallen, Sollwert wird um Gainfactor erh�ht
	DECLARE_VARIABLE(uint32_t, IfsFeederOverflowTimeOut);	// Zeit, bis der Zustand "Feeder Überlauf" erkannt wird. ( Überfüttert )
	DECLARE_VARIABLE(uint32_t, IfsFeederEmptyTimeOut);		// Zeit, bis der Zustand "Feeder leer" erkannt wird.
	DECLARE_VARIABLE(uint32_t, IfsStepTimeGain);		    // Zeitintervall in Sekunden bis der Sollwert erhöht wird.
	DECLARE_VARIABLE(uint32_t, IfsStepTimeReduce);		    // Zeitintervall in Sekunden bis der Sollwert verringert wird.
	DECLARE_VARIABLE(float32_t, IfsSetpointOverflow);		// Neuer Sollwert bei Erreichen von Overflow
	DECLARE_VARIABLE(uint32_t, IfsDebounceMax);				// Debounce Max / High
	DECLARE_VARIABLE(uint32_t, IfsDebounceMin);				// Debounce Min / Low


	DECLARE_VARIABLE(uint32_t, EmptyFeederRuntime);
	DECLARE_VARIABLE(float32_t, EmptyFeederSpeed);
	DECLARE_VARIABLE(base::eEmptyFeederType, EmptyFeederRestriction)



	// IO-Belegung
	DECLARE_VARIABLE( BOOL,				IOStartInput)					// Startextern IO
	DECLARE_VARIABLE( BOOL,				IOReleaseInput)					// Release extern (IO)
	DECLARE_VARIABLE( BOOL,				IOVolumetricInput);				// gravimetrisch/volumetrisch extern ausgeloest
	DECLARE_VARIABLE( BOOL,				IOEmptyFeederStartInput);		// Externel empty feeder start
	DECLARE_VARIABLE( BOOL,				IOEmptyFeederStopInput);		// Externel empty feeder stop
	DECLARE_VARIABLE( BOOL,				IORefillAlarmInput)				// Externel Refill alarm
	DECLARE_VARIABLE(BOOL,				IORefillStopInput);				// Refillstopp durch (IO)
	DECLARE_VARIABLE(BOOL,				IORefillRequestInput);			// Refillstart durch (IO)
	DECLARE_VARIABLE(BOOL,				IORefillReleaseInput);			// Refillrelease durch (IO)
	DECLARE_VARIABLE(BOOL,				IORefillInput);					// Refill durch (IO)
	DECLARE_VARIABLE(BOOL,				IORefillMaxSensorInput);		// Refill-MinSensor (IO)
	DECLARE_VARIABLE(BOOL,				IORefillMinSensorInput);		// Refill-MaxSensor (IO)
	DECLARE_VARIABLE(BOOL,				IODriveErrorInput)				// Fehler Drive ( Frequenzumrichter )
	DECLARE_VARIABLE(BOOL,				IOMotorLoadInput)				// Motor load input
	DECLARE_VARIABLE(BOOL,				IOExtAlarmInput)				// Ext. alarm inpunt
	DECLARE_VARIABLE(BOOL,				IODriveErrorPaddleInput)		// Fehler Drive paddle ( Frequenzumrichter )
	DECLARE_VARIABLE(BOOL,				IODriveErrorRefillFeederInput);	// Fehler Drive RefillFeeder ( Frequenzumrichter )

	DECLARE_VARIABLE(float32_t,			IOEncoderSpeedInput);		// Encoderspeed analog input



	DECLARE_VARIABLE(BOOL,				IODriveReleaseOutput)		// Freigabe zum Motor
	DECLARE_VARIABLE(BOOL,				IOOperateOutput)			// Betriebsleuchte, bei EncoderMonitor
	DECLARE_VARIABLE(BOOL,				IOStartOutput);				// Start
	DECLARE_VARIABLE(BOOL,				IOAlarmOutput);				// Alarm set IO
	DECLARE_VARIABLE(BOOL,				IORefillRequestOutput);		// Refillrequest output (IO)
	DECLARE_VARIABLE(float32_t,			IOActualRefillFeederOutput);// Aktuelle Schrauben-Nachf�llgeschwindigkeit

	// Aktuelle Zustands- Parameter
	DECLARE_VARIABLE(BOOL,				LogoStartState);			// Freigabe Logo (IO)
	DECLARE_VARIABLE(BOOL,				LCEnable);					// Waage enabled
	DECLARE_VARIABLE(BOOL,				LCAktiv)					// Waage aktiv
	DECLARE_VARIABLE(BOOL,				Service)					// Service
	DECLARE_VARIABLE(BOOL,				Release)					// Freigabe
	DECLARE_VARIABLE( BOOL,				Start)						// Startfreigabe extern/Modbus/User
	DECLARE_VARIABLE( BOOL,				RefillControlEnable);		// RefillControl enable
	DECLARE_VARIABLE( BOOL,				CalibrationState)			// Kalibrieren Zustand : 1 : gestartet
	DECLARE_VARIABLE( BOOL,				TaraState)					// Tarieren Zustand : 1 : gestartet
	DECLARE_VARIABLE( BOOL,				EmptyFeederState)			// EmptyFeeder Zustand : 1 : gestartet
	DECLARE_VARIABLE( BOOL,				SlaveMode)					// SlaveMode
	DECLARE_VARIABLE(BOOL,				DriveControlReady)
	DECLARE_VARIABLE(base::eEncoderState, EncoderState);

	// Aktuelle Werte
	DECLARE_VARIABLE(float32_t, LCActualWeight);					// Aktuelles Gewicht
	DECLARE_VARIABLE(float32_t, LCMeanWeight);						// Gemitteltes Gewicht
	DECLARE_VARIABLE(float32_t, LCRawWeight);						// Rawweight

	DECLARE_VARIABLE( float32_t,		Massflow)					// Aktueller Durchsatz in kg/h
	DECLARE_VARIABLE(float32_t,			ActualSetpoint)				// Aktueller Sollwert
	DECLARE_VARIABLE( float32_t,		ActualDoseperformance)		// Aktueller Doseperformance
	DECLARE_VARIABLE( float32_t,		ActualDriveCommand)			// Aktueller DriveCommand
	DECLARE_VARIABLE( float32_t,		Totalizer);					// Aktueller Verbrauch
	DECLARE_VARIABLE( float32_t,		ActualRotationalSpeed);		// Ist-RotationalSpeed (EncoderMonitor)
	DECLARE_VARIABLE( float32_t,		ActualAgitator);			// R�hrwerk Ist-DriveCommand
	DECLARE_VARIABLE( float32_t,		ActualLogoEncoder);				// RuntimeMonitoring value, 
	DECLARE_VARIABLE( float32_t,		ActualNoise);				// Aktuelle s Rauschen, siehe AlarmNoiseLimit

	DECLARE_VARIABLE( BOOL,				LocalModeState);			// Anlagenmodus lokal/in Linie
	DECLARE_VARIABLE( BOOL,				VolumetricState);			// Betriebsmodus gravimetrisch/volumetrisch
	DECLARE_VARIABLE( BOOL,				GravVolState);	            // Betriebsmodus gravimetrisch, aber temporaer volumetrisch

	DECLARE_VARIABLE( BOOL,				RefillReleaseState);		// Refill Freigabezustand
	DECLARE_VARIABLE( BOOL,				RefillStartState);			// Refill Startzustand, kann gestarted werde
	DECLARE_VARIABLE( BOOL,				RefillStopState);			// Refill Stopzustand, wurde gestoppt
	DECLARE_VARIABLE( BOOL,				RefillState);				// Refill Zustand
	DECLARE_VARIABLE( BOOL,				RefillFeederEnable);		// Refill screw enable

	DECLARE_VARIABLE( BOOL,				AgitatorEnable);			// R�hrwerk enable
	DECLARE_VARIABLE( BOOL,				AgitatorRunState);			// R�hrwerk l�uft

	DECLARE_VARIABLE( float32_t,		Drehgeber);					// Drehgeber
	DECLARE_VARIABLE(BOOL, ClearTotalizerState);

	DECLARE_VARIABLE(base::eWbmState,	WbfMinBeltLoadState);		// Aktueller Bandlaststatus
	DECLARE_VARIABLE(float32_t,			WbfActVelocity);			// Aktuelle Bandgeschwindigkeit
	DECLARE_VARIABLE(float32_t,			WbfBeltLoadMassflow);		// Bandlastistwert in kg/m
	DECLARE_VARIABLE(BOOL,				WbfBeltLoadLimit)


	DECLARE_VARIABLE(float32_t,			BatchGrossStream);			// Maximalgeschwindigkeit [%]
	DECLARE_VARIABLE(float32_t,			BatchFineStream);			// Feingeschwindigkeit [%]
	DECLARE_VARIABLE(float32_t,			BatchSwitchpoint);			// Umschaltpunkt kg
	DECLARE_VARIABLE(uint32_t,			BatchTime);					// Batchtime in s
	DECLARE_VARIABLE(float32_t,			BatchOverrunCompensation);	    // Batch Nachlaufkompensation [kg}
	DECLARE_VARIABLE(BOOL,				BatchStarted);			    // Batch started
	DECLARE_VARIABLE(BOOL,				BatchFinished);			    // Batch finished
	DECLARE_VARIABLE(int32_t,			GwbExtScale);

	DECLARE_VARIABLE( base::eGatefilter,PidGatefilterState);
	DECLARE_VARIABLE(BOOL, SWRefillRelease)
	DECLARE_VARIABLE(BOOL, RefillReleaseTrigger)


	DECLARE_PB(PBDosiererStart)
	DECLARE_PB(PBDosiererStop)
	DECLARE_PB(PBRefillStart)
	DECLARE_PB(PBRefillStop)
	DECLARE_PB(PBCalibStart)
	DECLARE_PB(PBCalibStop)
	DECLARE_PB(PBTareStart)
	DECLARE_PB(PBTareStop)
	DECLARE_PB(PBEmptyFeederStart)
	DECLARE_PB(PBEmptyFeederStop)
	DECLARE_PB(PBInline)
	DECLARE_PB(PBLocal)
	DECLARE_PB(PBVolumetric)
	DECLARE_PB(PBGravimetric)
	DECLARE_PB(PBClearTotalizer);
	DECLARE_PB(PBClearAlarm)				// Alarm zuruecksetzen
	DECLARE_PB(PBAcknowledgeAlarm)		// Alarm quittieren
    
	DECLARE_ALARM(Alarm);		            // Alarme
	DECLARE_ALARM(ClearAlarm);	            // Alarme loeschen
	DECLARE_ALARM(Warning);		            // Warnung
	DECLARE_ALARM(ClearWarning);	        // Warnung loeschen

	DECLARE_VARIABLE(base::refillsignal::RefillSignalType, RefillSignalCfg);			// Alarme in Kategorien
	DECLARE_VARIABLE(base::CAlarmCategories, AlarmCfg);			// Alarme in Kategorien
	DECLARE_VARIABLE(base::calib::CalibType, CalibCfg);			// Kalibriereinstellungen, see CalibrationCount
	DECLARE_VARIABLE(base::calib::CalibType, CalibCfgSave);			// Kalibriereinstellungen, see CalibrationCount
	DECLARE_VARIABLE(int32_t, CalibAktIndex);					// Aktueller Kalibrierindex

	char_t	cDummy[200];
};



struct CGlobalLineItem
{
	DECLARE_VARIABLE( TSTRINGCHARS,		Name)						// Name
	DECLARE_VARIABLE( TSTRINGCHARS,		ANNumber)					// AN-Number
	DECLARE_VARIABLE( uint64_t,		    QMNumber)					// QM-Nummer
	DECLARE_VARIABLE( BOOL, 			LineStart);					// Linestart
	DECLARE_VARIABLE( BOOL,				LineStarted);				// line started

	DECLARE_VARIABLE( BOOL,				IOLineStartInput);			// externer Start
	DECLARE_VARIABLE( BOOL, 			IOCustomerAlarmInput);		// KundenAlarm
	DECLARE_VARIABLE( BOOL, 			IOExtAlarmInput);			// VorlageAlarm
	DECLARE_VARIABLE( BOOL,				IOLineSetpointEnableInput);		// Sensorbetrieb Extruder IO
	DECLARE_VARIABLE( BOOL,				IOLineInvSetpointEnableInput);	// Inverser Levelsensoreingang
	DECLARE_VARIABLE( BOOL,				IORampUpInput);				// Rampe hoch durch Knopfdruck
	DECLARE_VARIABLE( BOOL,				IORampDownInput);			// Rampe runter durch Knopfdruck
	DECLARE_VARIABLE( BOOL,				IOAlarmShutDownInput);		// ShutDown extern
	DECLARE_VARIABLE( BOOL,				IOWindowsShutDownInput)		// Windows ShutDown extern
	DECLARE_VARIABLE( BOOL,				IOAlarm1QuitInput);	        // Alarmquit extern 1
	DECLARE_VARIABLE( BOOL,				IOAlarm03QuitInput);		// Alarmquit extern 1-3
	DECLARE_VARIABLE( BOOL,				IOSyncErrorExtern);			// ShutDown extern
	DECLARE_VARIABLE( BOOL,				IOSyncError);				// SyncErrorState
	DECLARE_VARIABLE( BOOL,				IOMainFeederInput);			// Haupteinzug
	DECLARE_VARIABLE( BOOL,				IOSideFeeder1Input);		// Seiteneinzug 1
	DECLARE_VARIABLE( BOOL,				IOSideFeeder2Input);		// Seiteneinzug 2
	DECLARE_VARIABLE( BOOL,				IOOperateAutoInput)			// Automatikbetrieb Eingang
	DECLARE_VARIABLE( BOOL,				IOOperateManualInput)		// Handbetrieb Eingang
	DECLARE_VARIABLE( BOOL,				IOUPSLowInput)				// UPS low
	DECLARE_VARIABLE(BOOL,				IOAlarm0Output)				// Was aktuell am Ausgang 0 anliegt
	DECLARE_VARIABLE(BOOL,				IOAlarm1Output)				// Was aktuell am Ausgang 1 anliegt
	DECLARE_VARIABLE(BOOL,				IOAlarm2Output)				// Was aktuell am Ausgang 2 anliegt
	DECLARE_VARIABLE(BOOL,				IOAlarm3Output)				// Was aktuell am Ausgang 3 anliegt
	DECLARE_VARIABLE(BOOL,				IOTotalizerPulseOutput)		// Totalisator impulse
	DECLARE_VARIABLE(BOOL,				IOOperateAutoOutput )		// IO operate auto output
	DECLARE_VARIABLE(BOOL,				IOServiceModeOutput);		// IO operate service output
	DECLARE_VARIABLE(BOOL,				IOWatchDogOutput)			// IO WatchDog output

	DECLARE_VARIABLE(BOOL,				IOLineReleaseOutput);		// Was aktuell am Ausgang anliegt
	DECLARE_VARIABLE(float32_t,			IOLineSetpointInput);		// ext. Liniensollwert



	DECLARE_VARIABLE( BOOL,				AlarmShutDown);				// AlarmShutDownState
	DECLARE_VARIABLE( float32_t, 		Skalierung);				// Skalierung in Prozent
	DECLARE_VARIABLE( float32_t, 		Hysterese);				    // Hysterese [kg/h]
    DECLARE_VARIABLE( float32_t,        RegeneratPercentage);
    DECLARE_VARIABLE( float32_t,        RecipeSetpoint);			// Entspricht Liniensollwert ohne Regeneratanteil
	DECLARE_VARIABLE( float32_t,		ManualSetpoint);			// manueller Liniensollwert, wird Rezepteingabe
	DECLARE_VARIABLE( float32_t,		ExternalSetpoint);			// externer  Liniensollwert

	DECLARE_VARIABLE( float32_t, 		LineSetpoint);			    // Aktueller Liniensollwert
	DECLARE_VARIABLE( float32_t, 		LineMassflow);			    // Aktueller Extruderwert in kg/s, ueberall gleich
	DECLARE_VARIABLE(float32_t,			LineMassflowMF);			// Aktueller Extruderwert in kg/s, Main feeder
	DECLARE_VARIABLE(float32_t,			LineMassflowSF1);		    // Aktueller Extruderwert in kg/s, Side feeder 1
	DECLARE_VARIABLE(float32_t,			LineMassflowSF2);		    // Aktueller Extruderwert in kg/s, Side feeder 2

	DECLARE_VARIABLE( int16_t, 			StateBetriebszustand);		// Ready, Notaus (extern), Stoeerung
	DECLARE_VARIABLE( int16_t, 			ErrorCode);					// FehlerCode
	DECLARE_VARIABLE( BOOL,				ExtLineSetpointState);		// 
	DECLARE_VARIABLE( BOOL,				ExtLineInvSetpointState);	// 
	DECLARE_VARIABLE( float32_t, 		MaxLeistung);				// Maximaler Leistung Extruder bei 100%
	DECLARE_VARIABLE( float32_t, 		MinTotband);				// Minimales Totband
	DECLARE_VARIABLE( uint32_t, 		Filterung);				    // Filterung
	DECLARE_VARIABLE( float32_t, 		RampStep);					// Rampenschritt
	DECLARE_VARIABLE( int32_t, 			RampDelay);					// Rampenschrittverzoegerung
	DECLARE_VARIABLE( BOOL, 			RampUp);					// Zustand Rampe hoch 
	DECLARE_VARIABLE( BOOL, 			RampDown);					// Zustand Rampe runter
	DECLARE_VARIABLE( base::CAlarmCategories,	AlarmCfg);				    // Alarme in Kategorien

	DECLARE_VARIABLE( BOOL,	            Alarm03QuitState);			// Alarmquit 0-3
	DECLARE_VARIABLE( BOOL,	            Alarm1QuitState);			// Alarmquit 1

	DECLARE_VARIABLE(float32_t,			TotalizerAll);				// Aktueller Verbrauch
	DECLARE_VARIABLE(float32_t, 		TotalizerLine);				// Aktueller Verbrauch
	DECLARE_VARIABLE(float32_t,			TotalizerMF);				// Aktueller Totalizer Main feed
	DECLARE_VARIABLE(float32_t,			TotalizerSF1);				// Aktueller Totalizer SF1
	DECLARE_VARIABLE(float32_t,			TotalizerSF2);				// Aktueller Totalizer SF2
	DECLARE_VARIABLE( uint32_t, 		ProductionTime);			// Aktuelle Produktionszeit
    DECLARE_VARIABLE( uint32_t,         LineDoseStarted);			// Bitfield Dose started
	DECLARE_VARIABLE( uint32_t,			LineDoseWaitStart);			// Linedosierung wartet zu starten
	DECLARE_VARIABLE( uint32_t,			DoseStarted);				// Bitfield Dose started

	DECLARE_VARIABLE(float32_t,			MBLineSetpoint);			// Analogsollwert Host extern
	// Sollwert Host vorhande n
	DECLARE_VARIABLE(float32_t,			MBLinePercentage);			// Analogsollwert Host extern

	DECLARE_VARIABLE( BOOL, 			Alarm0State);			    // Alarm 0 Zustand, identisch zu IOAlarm0Output
	DECLARE_VARIABLE( BOOL,	            Alarm1State);			    // Alarm 1 Zustand, identisch zu IOAlarm1Output
	DECLARE_VARIABLE( BOOL,	            Alarm2State);			    // Alarm 2 Zustand, identisch zu IOAlarm2Output
	DECLARE_VARIABLE( BOOL,	            Alarm3State);			    // Alarm 3 Zustand, identisch zu IOAlarm3Output
	DECLARE_VARIABLE( BOOL,				ServiceMode);				// Aktueller Servicemode

	DECLARE_VARIABLE(uint32_t,			FeederAlarm);								// Unterer Gewichtswert
	DECLARE_VARIABLE(float32_t,			TotalizerPulseStep);						// Schrittweite Totalisatorimpuls [kg]
	DECLARE_VARIABLE(uint32_t,			TotalizerPulseDuration);					// Dauer des Impulses [s]
	DECLARE_VARIABLE(BOOL,				UPSLowInputState)					// UPS low
	DECLARE_VARIABLE(float32_t,			ExtLineSetpoint);					// Aktueller ext. Liniensollwert
	DECLARE_VARIABLE(float32_t,			ExtLineOffset);						// Unterer Gewichtswert

	// *** start RESERVED ++++
	DECLARE_VARIABLE(float32_t, Reserved6_F32);						// Unterer Gewichtswert
	DECLARE_VARIABLE(float32_t, Reserved7_F32);						// Unterer Gewichtswert
	DECLARE_VARIABLE(float32_t, Reserved8_F32);						// Unterer Gewichtswert
	DECLARE_VARIABLE(float32_t, Reserved9_F32);						// Unterer Gewichtswert
	// *** end RESERVED ++++

	// *** start RESERVED ++++
	DECLARE_VARIABLE(int32_t, Reserved1_S32);								// Unterer Gewichtswert
	DECLARE_VARIABLE(int32_t, Reserved2_S32);								// Unterer Gewichtswert
	DECLARE_VARIABLE(int32_t, Reserved3_S32);								// Unterer Gewichtswert
	DECLARE_VARIABLE(int32_t, Reserved4_S32);								// Unterer Gewichtswert
	DECLARE_VARIABLE(int32_t, Reserved5_S32);								// Unterer Gewichtswert
	DECLARE_VARIABLE(int32_t, Reserved6_S32);								// Unterer Gewichtswert
	DECLARE_VARIABLE(int32_t, Reserved7_S32);								// Unterer Gewichtswert
	DECLARE_VARIABLE(int32_t, Reserved8_S32);								// Unterer Gewichtswert
	DECLARE_VARIABLE(int32_t, Reserved9_S32);								// Unterer Gewichtswert
	// *** end RESERVED ++++


	DECLARE_VARIABLE(BOOL, ModuleExternalSetpoint);
	DECLARE_VARIABLE(BOOL, ModuleSetpointRamp);
	DECLARE_VARIABLE(BOOL, ModuleLineSetpoint);
	DECLARE_VARIABLE(BOOL, ModuleLocalSetpoint);
	DECLARE_VARIABLE(BOOL, ModuleBatcher)
	DECLARE_VARIABLE(BOOL, ModuleTotalizer)
	DECLARE_VARIABLE(BOOL, ModuleTotalizerPulse)

	// *** start RESERVED ++++
	DECLARE_VARIABLE(BOOL, ModuleReserved2)
	DECLARE_VARIABLE(BOOL, ModuleReserved3)
	DECLARE_VARIABLE(BOOL, ModuleReserved4)
	DECLARE_VARIABLE(BOOL, ModuleReserved5)
	DECLARE_VARIABLE(BOOL, ModuleReserved6)
	DECLARE_VARIABLE(BOOL, ModuleReserved7)
	DECLARE_VARIABLE(BOOL, ModuleReserved8)
	DECLARE_VARIABLE(BOOL, ModuleReserved9)
	// *** end RESERVED ++++

	DECLARE_VARIABLE(BOOL, LineReleaseEnable);      // Enable Line Release
	DECLARE_VARIABLE(BOOL, LineStartEnable);		// Enable Line Start
	DECLARE_VARIABLE(BOOL, LineRelease);			// Line Release
	DECLARE_VARIABLE(BOOL, ExtLineSetpointEnable);							// Sensorbetrieb durch Button
	DECLARE_VARIABLE(BOOL, ExtLineInvSetpointEnable);						// 

    DECLARE_PB(  PBAlarm03Quit);
    DECLARE_PB(  PBAlarm1Quit);
	DECLARE_PB(  PBRampUp);
	DECLARE_PB(  PBRampDown);
	DECLARE_PB(  PBLineStart);
	DECLARE_PB(  PBLineStop);
	DECLARE_PB(  PBClearTotalizer);
	DECLARE_PB(  PBLineShutDown);			// Line Shutdown
	DECLARE_PB(  PBServiceMode);				// ServiceMode

	DECLARE_PB(PBAlarm0);										// Alarm 0 setzen in AlarmClass
	DECLARE_PB(PBAlarm1);										// Alarm 1 setzen in AlarmClass
	DECLARE_PB(PBAlarm2);										// Alarm 2 setzen in AlarmClass
	DECLARE_PB(PBAlarm3);										// Alarm 3 setzen in AlarmClass

	char_t	cDummy[1000];
};


