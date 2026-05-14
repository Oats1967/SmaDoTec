//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Declaration of module DSControl
///
/// @file   DSControl.hpp
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

#include "BASE/include/AlarmTypes.def"
#include "ECImplMacro.h"


//******************************************************************************************
//******************************************************************************************
 int32_t CDose :: ECSetRefillMinMaxLimit	( const int32_t index, const float32_t fMin, const float32_t fMax, const float32_t fMinMin)
{
	assert(index>= 0 && index < GLOBALDOSEMAXCOUNT);
	std::lock_guard<std::mutex> lock(mDGlobal[index]);
	auto& item = mDGlobal[index].get();
	if (item.RefillLimitMin.Test(fMin))
	{
		item.RefillLimitMin.Set(fMin);
	}
	if (item.RefillLimitMax.Test(fMax))
	{
		item.RefillLimitMax.Set(fMax);
	}
	if (item.RefillLimitMinMin.Test(fMinMin))
	{
		item.RefillLimitMinMin.Set(fMinMin);
	}
	return DOSIERUNG_OK;
}
//*************************************************************************************
 int32_t CDose :: ECGetRefillMinMaxLimit	( const int32_t index, float32_t *fMin, float32_t *fMax, float32_t *fMinMin) const
{
	assert(index  >= 0 && index < GLOBALDOSEMAXCOUNT);
	const auto& item = mDGlobal[index].get();
	*fMin		= item.RefillLimitMin.Get();
	*fMax		= item.RefillLimitMax.Get();
	*fMinMin	= item.RefillLimitMinMin.Get();
	return DOSIERUNG_OK;
}

 
 IMPLEMENT_EC_TSTRING(Name)
 IMPLEMENT_EC_STRING(LogoString)
 IMPLEMENT_EC(uint64_t, QMNumber)
 IMPLEMENT_EC(base::eLcType, LCTyp)

 IMPLEMENT_EC(BOOL, ModuleRefillEnable)

 IMPLEMENT_EC(base::eDoseType, DoseType)
 IMPLEMENT_EC(base::eFeedingType, FeederType)
 IMPLEMENT_EC(float32_t, Percentage)
 IMPLEMENT_EC(float32_t, NominalSetpoint)
 IMPLEMENT_EC(float32_t, MaxSetpoint)
 IMPLEMENT_EC(float32_t, MaxRotationalSpeed)
 IMPLEMENT_EC(float32_t, MassflowFilter)

 IMPLEMENT_EC(float32_t, RefillLimitMin)
 IMPLEMENT_EC(float32_t, RefillLimitMax)
 IMPLEMENT_EC(float32_t, RefillLimitMinMin)
 IMPLEMENT_EC(uint32_t,  RefillTime)
 IMPLEMENT_EC(uint32_t,  RefillSwitchDelay)
 IMPLEMENT_EC(uint32_t,  RefillDebounceMax)				// RefillDebounceMax
 IMPLEMENT_EC(uint32_t,  RefillDebounceMin)				// RefillDebounceMin
 IMPLEMENT_EC(uint32_t,  RefillFeederEmptyStart)		// MinDurationActive
 IMPLEMENT_EC(BOOL,		 RefillWeightAboveMax);
 IMPLEMENT_EC(BOOL,		 RefillWeightBelowMin);
 IMPLEMENT_EC(BOOL,		 RefillFeederEmptyStarted);
 IMPLEMENT_EC(BOOL,		 RefillFeederEmptyEnded);


 IMPLEMENT_EC(uint32_t, StartupDelay)
 IMPLEMENT_EC(uint32_t, StartupRamp)
 IMPLEMENT_EC(float32_t, NominalAgitator)
 IMPLEMENT_EC(float32_t, RefillFeederNominalSpeed);	// RefillFeederNominalSpeed
 IMPLEMENT_EC(float32_t, MinSetpointChange)			// Mindest Sollwert�nderung

 IMPLEMENT_EC(uint32_t, EncoderMonitor)
 IMPLEMENT_EC(uint32_t, LogoEncoderSampleTime)		// Abtastintervall definiert in Logo
 IMPLEMENT_EC(BOOL,		LogoRunControlEnable);		// RunControl Enabled


 IMPLEMENT_EC(float32_t, LCCorrectionFactor);		// Korrekturwert Waage
 IMPLEMENT_EC(float32_t, LCTaraWeight);				// Taraweight


 IMPLEMENT_EC(float32_t, AlarmNoiseLimit)
 IMPLEMENT_EC(uint32_t, AlarmReactionDelay)
 IMPLEMENT_EC(uint32_t, AlarmStartReactionDelay)
 IMPLEMENT_EC(uint32_t, AlarmMassflowHigh)
 IMPLEMENT_EC(uint32_t, AlarmMassflowLow)
 IMPLEMENT_EC(uint32_t, AlarmDriveCommandHigh)
 IMPLEMENT_EC(uint32_t, AlarmDriveCommandLow)
 IMPLEMENT_EC(uint32_t, AlarmDosePerformance)
 IMPLEMENT_EC(uint32_t, AlarmMaxBatchTime)
 IMPLEMENT_EC(float32_t, MaxDriveCommand)
 IMPLEMENT_EC(BOOL, Regenerat)
 IMPLEMENT_EC(uint32_t, IOPulseCounterInput)


 IMPLEMENT_EC(float32_t, WbfBeltLoadSetpoint)
 IMPLEMENT_EC(float32_t, WbfBeltLoadVol)
 IMPLEMENT_EC(float32_t, WbfReduction)
 IMPLEMENT_EC(uint32_t, WbfWeighingLine)
 IMPLEMENT_EC(uint32_t, WbfWheelSize)
 IMPLEMENT_EC(float32_t, WbfTareDriveCommand)
 IMPLEMENT_EC(uint32_t, WbfTareMeasurementTime)
 IMPLEMENT_EC(float32_t, WbfAlarmMinBeltLoad)
 IMPLEMENT_EC(float32_t, WbfAlarmMaxBeltLoad)
 IMPLEMENT_EC(float32_t, WbfMinDriveCommand)
 IMPLEMENT_EC(BOOL, WbfWeighingFull)

 // PID
 IMPLEMENT_EC(float32_t, PidPropGainGross)
 IMPLEMENT_EC(uint32_t, PidSampleInterval)
 IMPLEMENT_EC(float32_t, PidPropGainFine)
 IMPLEMENT_EC(float32_t, PidPropGainSwitchGrossFine)
 IMPLEMENT_EC(float32_t, PidIntegralGain)
 IMPLEMENT_EC(BOOL, PidDriveCommandInv)

 // VOL
 
 // ICP
 IMPLEMENT_EC(float32_t, SteepnessMassflow)

 // LCL / IFL
 IMPLEMENT_EC(float32_t, LclWeightMinLevel);
 IMPLEMENT_EC(float32_t, LclWeightMaxLevel);
 IMPLEMENT_EC(float32_t, LclWeightHysteresis);
 IMPLEMENT_EC(BOOL, LclWeightMinLevelActive);
 IMPLEMENT_EC(BOOL, LclWeightMaxLevelActive);

 // IFL
 IMPLEMENT_EC(float32_t, IflMinLineSetpoint);			// Oberer Gewichtswert
 IMPLEMENT_EC(float32_t, IflMaxLineSetpoint);			// Oberer Gewichtswert

 // IFS
 IMPLEMENT_EC(BOOL, IfsSwitchHigh);					// Endschalter Einschneckenextruder oben bzw. �berlauf aktiviert
 IMPLEMENT_EC(BOOL, IfsSwitchHighEnable);					// Endschalter Einschneckenextruder oben bzw. �berlauf aktiviert
 IMPLEMENT_EC(BOOL, IfsSwitchLow);					// Endschalter Einschneckenextruder unter bzw. Leer  aktiviert
 IMPLEMENT_EC(BOOL, IfsSwitchLowEnable);					// Endschalter Einschneckenextruder unter bzw. Leer  aktiviert
 IMPLEMENT_EC(BOOL, IfsSwitchMax);					// Endschalter Max, oberer Umkehrpunkt  aktiviert
 IMPLEMENT_EC(BOOL, IfsSwitchMaxEnable);					// Endschalter Max, oberer Umkehrpunkt  aktiviert
 IMPLEMENT_EC(BOOL, IfsSwitchMin);					// Endschalter Min, unterer Umkehrpunkt  aktiviert
 IMPLEMENT_EC(BOOL, IfsSwitchMinEnable);					// Endschalter Min, unterer Umkehrpunkt  aktiviert
 IMPLEMENT_EC(BOOL, IfsEmptyFeeder);				    // Dosierer leer erreicht
 IMPLEMENT_EC(BOOL, IfsOverflowFeeder);				// Dosierer Überlauf

 IMPLEMENT_EC(float32_t, IfsReduceFactor);			// Um wieviel kg/h soll der Liniensollwert verringert werden
 IMPLEMENT_EC(float32_t, IfsGainFactor);				// Um wieviel kg/h soll der Liniensollwert erhöht werden
 IMPLEMENT_EC(uint32_t, IfsFeederOverflowTimeOut);	// Zeit, bis der Zustand "Feeder Überlauf" erkannt wird.
 IMPLEMENT_EC(uint32_t, IfsFeederEmptyTimeOut);		// Zeit, bis der Zustand "Feeder leer" erkannt wird.
 IMPLEMENT_EC(uint32_t, IfsStepTimeGain);		        // Zeitintervall in Sekunden bis der Sollwert erhöht wird.
 IMPLEMENT_EC(uint32_t, IfsStepTimeReduce);		    // Zeitintervall in Sekunden bis der Sollwert verringert wird.
 IMPLEMENT_EC(float32_t, IfsSetpointOverflow);		// Neuer Sollwert bei Erreichen von Overflow
 IMPLEMENT_EC(uint32_t, IfsDebounceMax);				// Debounce Max / High
 IMPLEMENT_EC(uint32_t, IfsDebounceMin);				// Debounce Min / Low


 IMPLEMENT_EC(uint32_t, EmptyFeederRuntime)
 IMPLEMENT_EC(float32_t, EmptyFeederSpeed);
 IMPLEMENT_EC(base::eEmptyFeederType, EmptyFeederRestriction)

 IMPLEMENT_EC(BOOL, IOStartInput)			    // Start extern
 IMPLEMENT_EC(BOOL, IOReleaseInput)           // Freigabe extern
 IMPLEMENT_EC(BOOL, LogoStartState)		        // Logo Start
 IMPLEMENT_EC(BOOL, IOVolumetricInput)
 IMPLEMENT_EC(BOOL, IOEmptyFeederStartInput);	// Externel empty feeder start
 IMPLEMENT_EC(BOOL, IOEmptyFeederStopInput);	// Externel empty feeder stop
 IMPLEMENT_EC(BOOL, IORefillAlarmInput);	    // Externel Refill alarm
 IMPLEMENT_EC(BOOL, IORefillStopInput);			// Refillstopp durch (IO)
 IMPLEMENT_EC(BOOL, IORefillRequestInput);		// Refillrequest durch (IO)
 IMPLEMENT_EC(BOOL, IORefillReleaseInput);		// Refillrelease durch (IO)
 IMPLEMENT_EC(BOOL, IORefillInput);				// Refill durch (IO)
 IMPLEMENT_EC(BOOL, IORefillMaxSensorInput);			// Refill-MinSensor (IO)
 IMPLEMENT_EC(BOOL, IORefillMinSensorInput);			// Refill-MaxSensor (IO)
 IMPLEMENT_EC(BOOL, IODriveErrorInput);			// Fehler Drive ( Frequenzumrichter )
 IMPLEMENT_EC(BOOL, IOMotorLoadInput);			// Drive Motorlast
 IMPLEMENT_EC(BOOL, IOExtAlarmInput);				// Ext. Alarm
 IMPLEMENT_EC(BOOL, IODriveErrorPaddleInput)	// Fehler Drive paddle ( Frequenzumrichter )
 IMPLEMENT_EC(BOOL, IODriveErrorRefillFeederInput);	// Fehler Drive RefillFeeder ( Frequenzumrichter )

 IMPLEMENT_EC(float32_t, IOEncoderSpeedInput);				// Encoderspeed analog input



 IMPLEMENT_EC(BOOL, IODriveReleaseOutput)
 IMPLEMENT_EC(BOOL, IOOperateOutput)					// Betriebsleuchte, bei EncoderMonitor
 IMPLEMENT_EC(BOOL, IOAlarmOutput);
 IMPLEMENT_EC(BOOL, IOStartOutput);				// Start
 IMPLEMENT_EC(BOOL, IORefillRequestOutput);	// Refillrequest output (IO)

 IMPLEMENT_EC(float32_t, Massflow)
 IMPLEMENT_EC(float32_t, ActualSetpoint)				// Aktueller Sollwert
 IMPLEMENT_EC(float32_t, ActualDoseperformance)
 IMPLEMENT_EC(float32_t, ActualDriveCommand)
 IMPLEMENT_EC(float32_t, Totalizer)
 IMPLEMENT_EC(BOOL, ClearTotalizerState)
 IMPLEMENT_EC(BOOL, VolumetricState)
 IMPLEMENT_EC(BOOL, GravVolState)
 IMPLEMENT_EC(BOOL, LocalModeState)
 IMPLEMENT_EC(float32_t, ActualAgitator)
 IMPLEMENT_EC(float32_t, IOActualRefillFeederOutput);	// Aktuelle Schrauben-Nachf�llgeschwindigkeit

 IMPLEMENT_EC(float32_t, ActualLogoEncoder)
 IMPLEMENT_EC(float32_t, ActualNoise)				// Aktuelle s Rauschen, siehe AlarmNoiseLimit

 IMPLEMENT_EC(float32_t, PidGatefilter)

 IMPLEMENT_EC(BOOL, RefillReleaseState);		    // Refill Freigabezustand
 IMPLEMENT_EC(BOOL, RefillStartState);			// Refill Startzustand, kann gestarted werde
 IMPLEMENT_EC(BOOL, RefillStopState);			    // Refill Stopzustand, wurde gestoppt
 IMPLEMENT_EC(BOOL, RefillState);				    // Refill Zustand
 IMPLEMENT_EC(BOOL, RefillFeederEnable);	// Refill screw enable

 IMPLEMENT_EC(BOOL, AgitatorEnable);          // R�hrwerk enable
 IMPLEMENT_EC(BOOL, AgitatorRunState);        // R�hrwerk l�uft

 IMPLEMENT_EC(float32_t, Drehgeber)
 IMPLEMENT_EC(base::eEncoderState, EncoderState)
 IMPLEMENT_EC(BOOL, DriveControlReady)


 IMPLEMENT_EC(float32_t, LCActualWeight);					// Aktuelles Gewicht
 IMPLEMENT_EC(float32_t, LCMeanWeight);						// Gemitteltes Gewicht
 IMPLEMENT_EC(float32_t, LCRawWeight);						// Rawweight

 IMPLEMENT_EC(float32_t, ActualRotationalSpeed)
 IMPLEMENT_EC(BOOL, SlaveMode)				    // SlaveMode
 IMPLEMENT_EC(float32_t, WbfBeltLoadMassflow)
 IMPLEMENT_EC(float32_t, WbfActVelocity)
 IMPLEMENT_EC(BOOL, WbfBeltLoadLimit)

 IMPLEMENT_EC(float32_t, BatchGrossStream);				// Maximalgeschwindigkeit [%]
 IMPLEMENT_EC(float32_t, BatchFineStream);				// Maximalgeschwindigkeit [%]
 IMPLEMENT_EC(float32_t, BatchSwitchpoint);			// Abbremsung [s]
 IMPLEMENT_EC(uint32_t, BatchTime);						// BatchTime [s]
 IMPLEMENT_EC(float32_t, BatchOverrunCompensation);	    // Batch Nachlaufkompensation [kg}

 IMPLEMENT_EC(BOOL, BatchStarted);			    // Batch started
 IMPLEMENT_EC(BOOL, BatchFinished);			    // Batch finished
 IMPLEMENT_EC(int32_t, GwbExtScale);


 IMPLEMENT_EC(base::eWbmState, WbfMinBeltLoadState)
 IMPLEMENT_EC(base::eGatefilter, PidGatefilterState)
 IMPLEMENT_EC(BOOL, RefillControlEnable)
 IMPLEMENT_EC(BOOL, CalibrationState)
 IMPLEMENT_EC(BOOL, TaraState)
 IMPLEMENT_EC(BOOL, EmptyFeederState)

 IMPLEMENT_EC(BOOL, LCEnable)
 IMPLEMENT_EC(BOOL, LCAktiv)
 IMPLEMENT_EC(BOOL, Service)
 IMPLEMENT_EC(BOOL, Release)			// Freigabe
 IMPLEMENT_EC(BOOL, Start)				  // Startfreigabe extern/Modbus/User


 IMPLEMENT_EC(base::refillsignal::RefillSignalType, RefillSignalCfg)
 IMPLEMENT_EC(base::calib::CalibType, CalibCfg)
 IMPLEMENT_EC(base::calib::CalibType, CalibCfgSave)
 IMPLEMENT_EC(int32_t, CalibAktIndex)
 IMPLEMENT_EC(BOOL, SWRefillRelease)
 IMPLEMENT_EC(BOOL, RefillReleaseTrigger)

 IMPLEMENT_EC_PB(PBDosiererStart)
 IMPLEMENT_EC_PB(PBDosiererStop)
 IMPLEMENT_EC_PB(PBRefillStart)              // Start refill
 IMPLEMENT_EC_PB(PBRefillStop)               // Stop refill
 IMPLEMENT_EC_PB(PBCalibStart)
 IMPLEMENT_EC_PB(PBCalibStop)
 IMPLEMENT_EC_PB(PBTareStart)
 IMPLEMENT_EC_PB(PBTareStop)
 IMPLEMENT_EC_PB(PBEmptyFeederStart)
 IMPLEMENT_EC_PB(PBEmptyFeederStop)
 IMPLEMENT_EC_PB(PBInline)
 IMPLEMENT_EC_PB(PBLocal)
 IMPLEMENT_EC_PB(PBVolumetric)
 IMPLEMENT_EC_PB(PBGravimetric)
 IMPLEMENT_EC_PB(PBClearAlarm)				// Alarm zuruecksetzen
 IMPLEMENT_EC_PB(PBAcknowledgeAlarm)			// Alarm quittieren
 IMPLEMENT_EC_PB(PBClearTotalizer);
 

 
