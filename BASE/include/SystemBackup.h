//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Declaration of module SystemBackup
///
/// @file   SystemBackup.h
///
///
/// @coypright(c)  Ing.b�ro Hafer
///                Branderweg 8A
///                D-91058 Erlangen
///
/// @author        Detlef Hafer
///
//------------------------------------------------------------------------------------
#pragma once

#include <array>
#include <vector>
#include "BASE/Base.h"
#include "BASE/Base.def"
#include "BASE/Utils/public/StringUtil.h"
#include "BASE/include/DoseType.h"
#include "BASE/include/LCType.h"
#include "BASE/include/FeedingType.h"
#include "BASE/include/EmptyFeederType.h"
#include "BASE/include/CalibType.h"
#include "BASE/Utils/public/Memory.h"


namespace base
{
	//--------------------------------------------------------------------------------------------------------------------
	//--------------------------------------------------------------------------------------------------------------------
	struct CLineBackupConfig
	{
		std::string RecipeName;
		std::string ANNumber;
		float32_t MaxSetpoint;
		uint64_t  QMNumber;
		float32_t RecipeSetpoint;
		float32_t RegeneratPercentage;
		float32_t Setpoint;
		float32_t Percentage;
		float32_t Hysteresis;
		float32_t MinTotband;
		uint32_t  FilterTime;
		float32_t ExtSetpointScale;
		float32_t ExtSetpointOffset;
		float32_t RampStep;
		uint32_t  RampDelay;
		float32_t TotalizerPulseStep;
		uint32_t  TotalizerPulseDuration;

		CLineBackupConfig();
	};

	inline CLineBackupConfig::CLineBackupConfig() :
		RecipeName("")
		, ANNumber("")
		, MaxSetpoint(0.0F)
		, QMNumber(0)
		, RecipeSetpoint(0.0F)
		, RegeneratPercentage(0.0F)
		, Setpoint(0.0F)
		, Percentage(0.0F)
		, Hysteresis(0.0F)
		, MinTotband(0.0F)
		, FilterTime(0)
		, ExtSetpointScale(0.0F)
		, ExtSetpointOffset(0.0F)
		, RampStep(0.0F)
		, RampDelay(0)
		, TotalizerPulseStep(0.0F)
		, TotalizerPulseDuration(0)
	{}
	//--------------------------------------------------------------------------------------------------------------------
	//--------------------------------------------------------------------------------------------------------------------
	struct DoseBackupConfig
	{
		std::string			Name;				// Name
		uint32_t			ID;
		uint64_t			QMNumber;				// QM-Nummer
		// Parameter
		base::eDoseType		DoseType;				// Dosiertyp
		base::eLcType		LCTyp;					// Waagentyp
		base::eFeedingType	FeedingType;
		float32_t			Percentage;				// Nomineller Prozentwert
		float32_t			NominalSetpoint;		// Nomineller Sollwert in kg/h
		float32_t			MaxSetpoint;			// Max. Durchsatz fuer Anzeige
		float32_t			MaxRotationalSpeed;		// Maximaldrehzahl
		float32_t			MassflowFilter;			// Istwertfilter
		float32_t			RefillLimitMin;			// Min-Wert
		float32_t			RefillLimitMax;			// Maximal  zulaessiges Gewicht
		float32_t			RefillLimitMinMin;		// Untere Refill Begrenzung
		uint32_t			RefillTime;				// Max. Befuellzeit
		uint32_t			RefillSwitchDelay;	    // Umschaltverzoegerung
		uint32_t			RefillDebounceMax;		// RefillDebounceMax
		uint32_t			RefillDebounceMin;		// RefillDebounceMin
		uint32_t			RefillFeederEmptyStart;	// MinDurationActive
		float32_t			NominalAgitator;		// R�hrwerk Soll-DriveCommand
		float32_t			RefillFeederNominalSpeed;	// Schneckendrehzahl Nachf�lldosierer
		float32_t			MinSetpointChange;		// Mindest Sollwert�nderung

		uint32_t			StartupDelay;			// StartupDelay
		uint32_t			StartupRamp;			// StartupRamp

		float32_t			LCCorrectionFactor;		// Korrekturwert Waage
		float32_t			LCTaraWeight;			// Taraweight
		uint32_t			EncoderMonitor;			// RotationalSpeedueberwachung 0 ... 1000

		float32_t			AlarmNoiseLimit;		//Waagenstoergrenze Alarmgrenze
		uint32_t			AlarmReactionDelay;
		uint32_t			AlarmStartReactionDelay;
		uint32_t			AlarmMassflowLow;
		uint32_t			AlarmMassflowHigh;
		uint32_t			AlarmDriveCommandHigh;
		uint32_t			AlarmDriveCommandLow;
		uint32_t			AlarmDosePerformance;
		uint32_t			AlarmMaxBatchTime;

		float32_t			MaxDriveCommandChange;
		BOOL				Regenerat;

		// Beltweigher
		float32_t			WbfBeltLoadSetpoint;		// BeltLoadSetpoint in kg/m
		float32_t			WbfBeltLoadVolSwitch;		// Umschaltgrenze f. volumetrisch
		float32_t			WbfReduction;				// Getriebeuntersetzung
		uint32_t			WbfWeighingLine;			// Laenge des Wiegebereichs
		uint32_t			WbfWheelSize;				// WheelSize
		float32_t			WbfTareDriveCommand;		// DriveCommand fuer Tarierung
		uint32_t			WbfTareMeasurementTime;		// Messzeit fuer Tarierung
		float32_t			WbfAlarmMinBeltLoad;		// Alarm minimale Bandlast
		float32_t			WbfAlarmMaxBeltLoad;		// Alarm maximale Bandlast
		float32_t			WbfMinDriveCommand;			// minimum drive command
		BOOL				WbfWeighingFull;

		// PID
		float32_t			PidPropGainGross;			// Prop. Konstante
		uint32_t			PidSampleInterval;			// PID-SampleInterval
		float32_t			PidPropGainFine;			// Feinverstaerkung
		float32_t			PidPropGainSwitchGrossFine;		// Umschalten grob auf fein
		float32_t			PidIntegralGain;
		float32_t			PidGatefilter;
		BOOL				PidDriveCommandInv;

		//IFS
		float32_t IfsReduceFactor;			// Pegel ist �ber Max, Sollwert wird um Reducefaktor verringert
		float32_t IfsGainFactor;			// Pegel ist unter Min gefallen, Sollwert wird um Gainfactor erh�ht
		uint32_t IfsFeederOverflowTimeOut;	// Zeit, bis der Zustand "Feeder Überlauf" erkannt wird. ( Überfüttert )
		uint32_t IfsFeederEmptyTimeOut;		// Zeit, bis der Zustand "Feeder leer" erkannt wird.
		uint32_t IfsStepTimeGain;		    // Zeitintervall in Sekunden bis der Sollwert erhöht wird.
		uint32_t IfsStepTimeReduce;		    // Zeitintervall in Sekunden bis der Sollwert verringert wird.
		float32_t IfsSetpointOverflow;		// Neuer Sollwert bei Erreichen von Overflow
		uint32_t IfsDebounceMax;			// Debounce Max / High
		uint32_t IfsDebounceMin;			// Debounce Min / Low

		// VOL
		// ICP
		float32_t			SteepnessMassflow;

		uint32_t				EmptyFeederRuntime;
		float32_t				EmptyFeederSpeed;

		base::calib::CalibType calibType;

		DoseBackupConfig();
	};
//------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------

	inline DoseBackupConfig::DoseBackupConfig() :
	  Name("")
	, ID{ 0 }
	, QMNumber{ 0 }
	, DoseType{ base::eDoseType::DOSETYPE_NONE }				// Dosiertyp
	, LCTyp{ base::eLcType::LC_EMPTY }
	, FeedingType{ base::eFeedingType::FF_NONE }
	, Percentage{ 0.0F }
	, NominalSetpoint{ 0.0F }
	, MaxSetpoint { 0.0F }			// Max. Durchsatz fuer Anzeige
	, MaxRotationalSpeed { 0.0F }		// Maximaldrehzahl
	, MassflowFilter { 0.0F }			// Istwertfilter
	, RefillLimitMin{ 0.0F }			// Istwertfilter
	, RefillLimitMax{ 0.0F }			// Istwertfilter
	, RefillLimitMinMin{ 0.0F }			// Istwertfilter
	, RefillTime{ 0 }
	, RefillSwitchDelay{ 0 }
	, RefillDebounceMax{ 0 }
	, RefillDebounceMin{ 0 }
	, RefillFeederEmptyStart{ 0 }
	, NominalAgitator { 0.0F }		// R�hrwerk Soll-DriveCommand
	, RefillFeederNominalSpeed { 0.0F }	// Schneckendrehzahl Nachf�lldosierer
	, MinSetpointChange { 0.0F }		// Mindest Sollwert�nderung
	, StartupDelay { 0 }			// StartupDelay
	, StartupRamp { 0 }			// StartupRamp
	, LCCorrectionFactor { 1.0F }		// Korrekturwert Waage
	, LCTaraWeight { 0.0F }			// Taraweight
	, EncoderMonitor { 0 }			// RotationalSpeedueberwachung 0 ... 1000
	, AlarmNoiseLimit { 0.0F }		//Waagenstoergrenze Alarmgrenze
	, AlarmReactionDelay { 0 }
	, AlarmStartReactionDelay { 0 }
	, AlarmMassflowLow { 0 }
	, AlarmMassflowHigh { 0 }
	, AlarmDriveCommandHigh { 0 }
	, AlarmDriveCommandLow { 0 }
	, AlarmDosePerformance { 0 }
	, AlarmMaxBatchTime { 0 }
	, MaxDriveCommandChange { 0.0F }
	, Regenerat { FALSE }
	, WbfBeltLoadSetpoint { 0.0F }		// BeltLoadSetpoint in kg/m
	, WbfBeltLoadVolSwitch { 0.0F }		// Umschaltgrenze f. volumetrisch
	, WbfReduction { 0.0F }				// Getriebeuntersetzung
	, WbfWeighingLine { 0 }			// Laenge des Wiegebereichs
	, WbfWheelSize { 0 }				// WheelSize
	, WbfTareDriveCommand { 0.0F }		// DriveCommand fuer Tarierung
	, WbfTareMeasurementTime { 0 }		// Messzeit fuer Tarierung
	, WbfAlarmMinBeltLoad { 0.0F }		// Alarm minimale Bandlast
	, WbfAlarmMaxBeltLoad { 0.0F }		// Alarm maximale Bandlast
	, WbfMinDriveCommand { 0.0F }			// minimum drive command
	, WbfWeighingFull { FALSE }
	, PidPropGainGross { 0.0F }			// Prop. Konstante
	, PidSampleInterval { 0 }			// PID-SampleInterval
	, PidPropGainFine { 0.0F }			// Feinverstaerkung
	, PidPropGainSwitchGrossFine { 0.0F }		// Umschalten grob auf fein
	, PidIntegralGain { 0.0F }
	, PidGatefilter { 0.0F }
	, PidDriveCommandInv { FALSE }

	, IfsReduceFactor{ 0.0F }
	, IfsGainFactor{ 0.0F }
	, IfsFeederOverflowTimeOut{ 0 }
	, IfsFeederEmptyTimeOut{ 0 }
	, IfsStepTimeGain{ 0 }
	, IfsStepTimeReduce{ 0 }
	, IfsSetpointOverflow{ 0.0F }
	, IfsDebounceMax{ 0 }
	, IfsDebounceMin{ 0 }
	, SteepnessMassflow { 0.0F }
	, EmptyFeederRuntime { 0 }
	, EmptyFeederSpeed { 0.0F }
	{
		base::utils::baseZeroMemory(calibType);
	}

//------------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------------
struct CSystemBackup
{
	std::vector<DoseBackupConfig> m_Feeder;
	CLineBackupConfig			  m_Line;
};

};

