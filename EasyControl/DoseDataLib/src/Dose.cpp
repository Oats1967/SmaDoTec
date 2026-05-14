//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Implementation of module Dose.cpp
///
/// @file   Dose.cpp
///
///
/// @coypright Ing.b�ro Hafer
///            Branderweg 8A
///            D-91058 Erlangen
///
/// @author    Detlef Hafer
///
//------------------------------------------------------------------------------------



#include <cassert>
#include <fstream>
#include <sstream>      // std::ostringstream
#include <string>
#include <mutex>
#include "BASE/types.h"
#include "BASE/Utils/public/BitOp.h"
#include "BASE/crc/src/crc.h"
#include "Dose.h"
#include "DoseDataLib/include/DoseData.def"

#include "Logger/include/Logger.h"

#include "SYControl.hpp"
#include "EXControl.hpp"
#include "DSControl.hpp"
#include "DSAlarm.hpp"



using namespace base;

#define INIT_ITEM(a) aItem.##a.Reset();
#define INIT_PB(a) aItem.##a = FALSE;

#define SAVEBAK "BAK"

static const uint32_t cExpectedSize = _U32(sizeof(CGlobalSystemItem) + sizeof(CGlobalLineItem) + GLOBALDOSEMAXCOUNT * sizeof(CGlobalDoseItem) + sizeof(uint32_t));


CDose* CDose :: pDose = NULL;

//******************************************************************************************
//******************************************************************************************
CDose::CDose(void)
{
	pDose			= this;
}
//******************************************************************************************
//******************************************************************************************
CDose::~CDose(void)
{
	Close ();
	pDose = NULL;
}
//******************************************************************************************
//******************************************************************************************
int32_t CDose::Save(const std::string& szFilename)
{
	uint8_t* buffer = new uint8_t[cExpectedSize];
	uint32_t length = 0;
	{
		std::lock_guard<std::mutex> lock(mSGlobal);
		const auto& aG = mSGlobal.get();
		memcpy(&buffer[length], reinterpret_cast<const char*>(&aG), sizeof(aG));
		length += _U32(sizeof(aG));
	}
	{
		std::lock_guard<std::mutex> lock(mEGlobal);
		const auto& aG = mEGlobal.get();
		memcpy(&buffer[length], reinterpret_cast<const char*>(&aG), sizeof(aG));
		length += _U32(sizeof(aG));
	}
	for (int32_t i = 0; i < mDGlobal.size(); i++)
	{
		std::lock_guard<std::mutex> lock(mDGlobal[i]);
		const auto& aG = mDGlobal[i].get();
		memcpy(&buffer[length], reinterpret_cast<const char*>(&aG), sizeof(aG));
		length += _U32(sizeof(aG));
	}
	{
		uint32_t crcchecksum = crc_calculateCRC32(buffer, length, 0xFFFFFFFF);
		memcpy(&buffer[length], &crcchecksum, sizeof(uint32_t));
		length += sizeof(uint32_t);
	}
	std::ofstream ofs(szFilename, std::ios::binary);
	ofs.write((const char*)buffer, cExpectedSize);
	ofs.close();
	delete[] buffer;
	return DOSIERUNG_OK;
}
//******************************************************************************************
//******************************************************************************************
int32_t CDose::parse(const uint8_t* buffer, const uint32_t totallength)
{
	{
		uint32_t crcsrc = 0;
		memcpy(&crcsrc, &buffer[totallength - sizeof(uint32_t)], sizeof(uint32_t));
		uint32_t crcdest = crc_calculateCRC32(buffer, totallength - sizeof(uint32_t), 0xFFFFFFFF);
		if (crcsrc != crcdest)
		{
			return DOSIERUNG_CRCERROR;
		}
	}
	uint32_t length = 0;
	{
		std::lock_guard<std::mutex> lock(mSGlobal);
		CGlobalSystemItem& aG = mSGlobal.get();
		memcpy(&aG, &buffer[length], sizeof(aG));
		length += _U32(sizeof(aG));
	}
	{
		std::lock_guard<std::mutex> lock(mEGlobal);
		CGlobalLineItem& aG = mEGlobal.get();
		memcpy(&aG, &buffer[length], sizeof(aG));
		length += _U32(sizeof(aG));
	}
	for (int32_t i = 0; i < GLOBALDOSEMAXCOUNT; i++)
	{
		std::lock_guard<std::mutex> lock(mDGlobal[i]);
		CGlobalDoseItem& aG = mDGlobal[i].get();
		memcpy(&aG, &buffer[length], sizeof(aG));
		length += _U32(sizeof(aG));
	}
	return DOSIERUNG_OK;
}
//******************************************************************************************
//******************************************************************************************
int32_t CDose::Activate()
{
	ResetSystem();
	ResetLineItem();
	ResetDoseItems();
	return DOSIERUNG_OK;
}
//******************************************************************************************
//******************************************************************************************
int32_t CDose::SaveBak(const std::string& szFilename)
{
	int32_t result = DOSIERUNG_SAVEERROR;
	auto found = szFilename.rfind('.');
	if (found != std::string::npos)
	{
		std::string filename = szFilename.substr(0, found + 1);
		result = Save(filename + SAVEBAK);
	}
	return result;
}
//******************************************************************************************
//******************************************************************************************
int32_t CDose::Restore(const std::string& szFilename)
{
	int32_t result = DOSIERUNG_LOADERROR;

	auto found = szFilename.rfind('.');
	if (found != std::string::npos)
	{
		std::string filename = szFilename.substr(0, found + 1);
		result = Load(filename +  SAVEBAK);
		if (result == DOSIERUNG_OK)
		{
			result = Save(szFilename);
		}
	}
	return result;
}
//******************************************************************************************
//******************************************************************************************
int32_t CDose::Load(const std::string& szFilename)
{
	int32_t result = DOSIERUNG_LOADERROR;

	std::ifstream is(szFilename, std::ios::binary);
	if (is)
	{
		// get length of file:
		is.seekg(0, is.end);
		int32_t length = _S32(is.tellg());
		if (length == cExpectedSize)
		{
			uint8_t* buffer = new uint8_t[cExpectedSize];
			is.seekg(0, is.beg);
			is.read((char*)buffer, cExpectedSize);
			if (! is )
			{
				LOGERROR("Error reading file = ");
				result = DOSIERUNG_LOADERROR;
			}
			else
			{
				result = parse(buffer, cExpectedSize);
			}
			delete[] buffer;
		}
		else
		{
			LOGERROR("Filesize not valid, expected = " << _S32(cExpectedSize) << " Filesize = " << _S32(length));
			result = DOSIERUNG_INVALIDLENGTH;
		}
	}
	return result;
}
//******************************************************************************************
//******************************************************************************************
BOOL CDose::Open()
{
	return TRUE;
}
//******************************************************************************************
//******************************************************************************************
BOOL CDose::Close(void)
{
	return TRUE;
}
//******************************************************************************************
//******************************************************************************************
void CDose::ResetCalibDoseItem(int32_t n)
{
	CGlobalDoseItem& aItem = mDGlobal[n].get();

	aItem.CalibAktIndex.Reset();
	auto& rItem = aItem.CalibCfg.Get();
	if (__ISVALIDDOSETYPE(aItem.DoseType.Get()) )
	{
		rItem.count = RANGE(rItem.count, 1U, base::calib::MAXCALIBITEMS);

		const float32_t cLimitMinDriveCommand    = 0.05f;
		const float32_t cLimitMinDosePerformance = 0.2f;
		const float32_t cMinDriveCommand = 30.0f;
		const uint32_t  cMinMeasureTime = 20U;
		const float32_t cMinDosePerformance = 1000.0f;
		for (uint32_t i = 0; i < rItem.count; i++)
		{
			assert(rItem.fDriveCommand[i] > cLimitMinDriveCommand);;
			if (rItem.fDriveCommand[i] < cLimitMinDriveCommand)
			{
				rItem.fDriveCommand[i] = cMinDriveCommand;
			}
			else
			{
				rItem.fDriveCommand[i] = RANGE(rItem.fDriveCommand[i], cLimitMinDriveCommand, 100.0f);
			}
			assert(rItem.lMeasureTime[i] > 0);
			if (rItem.lMeasureTime[i] == 0)
			{
				rItem.lMeasureTime[i] = cMinMeasureTime;
			}
			else
			{
				rItem.lMeasureTime[i] = RANGE(rItem.lMeasureTime[i], 0, 100);
			}
			assert(rItem.fDosePerformance[i] > cLimitMinDosePerformance);
			if (rItem.fDosePerformance[i] < cLimitMinDosePerformance)
			{
				rItem.fDosePerformance[i] = cMinDosePerformance;
			}
			else
			{
				rItem.fDosePerformance[i] = RANGE(rItem.fDosePerformance[i], cLimitMinDosePerformance, 10000.0F);
			}
		}
		{
			const float32_t c_Epsilon = 0.1F;
			if (aItem.ActualDoseperformance.Get() < c_Epsilon)
			{
				assert(rItem.fDosePerformance[0] > c_Epsilon);
				if (rItem.fDosePerformance[0] > c_Epsilon)
				{
					aItem.ActualDoseperformance.Init(rItem.fDosePerformance[0]);
				}
			}
		}
	}
	else
	{
		rItem.count = 0;
		INIT_ITEM(ActualDoseperformance);
	}
	for (uint32_t i = rItem.count; i < base::calib::MAXCALIBITEMS; i++)
	{
		rItem.fDriveCommand[i] = 0.0F;
		rItem.lMeasureTime[i] = 0L;
		rItem.fDosePerformance[i] = 0.0F;
	}
	aItem.CalibCfgSave.Init(rItem);
}
//******************************************************************************************
//******************************************************************************************
void CDose :: ResetDoseItem (int32_t n)
{ 	
	std::lock_guard<std::mutex> lock(mDGlobal[n]);			        \

	CGlobalDoseItem& aItem = mDGlobal[n].get();
	INIT_ITEM(Massflow);
	INIT_ITEM(ActualRotationalSpeed);
	INIT_ITEM(ActualAgitator);
	INIT_ITEM(IOActualRefillFeederOutput);


    if (aItem.MaxDriveCommand.Get() < 1e-06)
    {
        if (__ISLWVTYPE(aItem.DoseType.Get()))
        {
            aItem.MaxDriveCommand.Init(2.0f);
        }
        else
            aItem.MaxDriveCommand.Init(20.0f);
    }
    if  (! __ISVALIDDOSETYPE(aItem.DoseType.Get()) )
	{
		INIT_ITEM(ActualDriveCommand);
		INIT_ITEM(Totalizer);
		INIT_ITEM(Percentage);
		if (!__ISIFLTYPE(aItem.DoseType.Get()))
		{
			INIT_ITEM(NominalSetpoint);
			INIT_ITEM(ActualSetpoint);
			aItem.LocalModeState.Init(TRUE);
		}
	}

	ResetCalibDoseItem(n);

    if (__ISWBMTYPE(aItem.DoseType.Get()))
    {
        aItem.LocalModeState.Init(TRUE);
    }


	aItem.EncoderState.Init(eEncoderState::ENCODERSTATE_OFF);	// 
	aItem.WbfMinBeltLoadState.Init(base::eWbmState::WBFMINBELTLOAD_INVALID);
    aItem.PidGatefilterState.Init(base::eGatefilter::GATEFILTER_NONE);

	INIT_ITEM(LCAktiv);
	INIT_ITEM(LCEnable);
	INIT_ITEM(LCActualWeight);
	INIT_ITEM(LCMeanWeight);
	INIT_ITEM(LCRawWeight);
	INIT_ITEM(RefillWeightAboveMax);
	INIT_ITEM(RefillWeightBelowMin);
	INIT_ITEM(RefillFeederEmptyStarted);
	INIT_ITEM(RefillFeederEmptyEnded);
	INIT_ITEM(DriveControlReady);

	INIT_ITEM(Service);					// Kalibrierug gestartet
	INIT_ITEM(Release);					// Freigabe
	INIT_ITEM(IOReleaseInput);					// Freigabe extern
	INIT_ITEM(IOStartInput);
	INIT_ITEM(Start);						// Nicht setzen !
	INIT_ITEM(IOOperateOutput);					// Betrieb
	INIT_ITEM(IOStartOutput);					// Start


	INIT_ITEM(IODriveReleaseOutput);
	INIT_ITEM(IOMotorLoadInput);	    // Drive Motorlast
	INIT_ITEM(IOExtAlarmInput);				// ext. Alarm


	INIT_ITEM(IOVolumetricInput);

	INIT_ITEM(IOEmptyFeederStartInput);	// Externel empty feeder start
	INIT_ITEM(IOEmptyFeederStopInput);	// Externel empty feeder stop

	INIT_ITEM(IORefillReleaseInput);			// Refillrelease durch (IO)
	INIT_ITEM(IORefillInput);			// Refill durch (IO)

	INIT_ITEM(IORefillAlarmInput)				// Externel Refill alarm
	INIT_ITEM(IORefillRequestInput);			// Refill start durch IO
	INIT_ITEM(IORefillStopInput);				// Refill stop ddurch IO
	INIT_ITEM(IORefillMaxSensorInput);	    // Refillrequest durch (IO)
	INIT_ITEM(IORefillMinSensorInput);	    // Refillrelease durch (IO)
	INIT_ITEM(IODriveErrorInput);	    // Fehler Drive ( Frequenzumrichter
	INIT_ITEM(IODriveErrorPaddleInput);	    // Fehler Drive ( Frequenzumrichter
	INIT_ITEM(IODriveErrorRefillFeederInput);	  // Fehler Drive RefillFeeder ( Frequenzumrichter )

	aItem.IOEncoderSpeedInput.Init(0.0F);




	INIT_ITEM(IORefillRequestOutput);			// Refillrequest output (IO)
	// INIT_ITEM(RefillReleaseState);			// Refillfreigabezustand use last state before shutdown
	INIT_ITEM(RefillStartState);					// Refillzustand start
	INIT_ITEM(RefillStopState);					// Refillzustand start
	INIT_ITEM(RefillState);						// Nachf�llzustand
	INIT_ITEM(AgitatorRunState);
	INIT_ITEM(SWRefillRelease);
	INIT_ITEM(RefillFeederEnable);	    // Refill screw enable


	INIT_PB(PBDosiererStart);
	INIT_PB(PBDosiererStop);
	INIT_PB(PBClearAlarm);
	INIT_PB(PBAcknowledgeAlarm);
	INIT_PB(PBRefillStart);
	INIT_PB(PBRefillStop);
	INIT_PB(PBVolumetric);
	INIT_PB(PBGravimetric);
	INIT_PB(PBInline);
	INIT_PB(PBLocal);
	INIT_PB(PBCalibStart);
	INIT_PB(PBCalibStop);
	INIT_PB(PBTareStart);
	INIT_PB(PBTareStop);
	INIT_PB(PBEmptyFeederStart);
	INIT_PB(PBEmptyFeederStop);
	INIT_PB(PBClearTotalizer);

	INIT_ITEM(RefillReleaseTrigger);
	INIT_ITEM(RefillControlEnable)
	INIT_ITEM(CalibrationState);
	INIT_ITEM(TaraState);
	INIT_ITEM(EmptyFeederState);
	INIT_ITEM(GravVolState)

	INIT_ITEM(WbfBeltLoadMassflow);

	aItem.Alarm.Reset();
	aItem.Warning.Reset();
	aItem.ClearAlarm.Reset();
	aItem.ClearWarning.Reset();
	INIT_ITEM(Drehgeber);
	INIT_ITEM(ActualNoise);

	INIT_ITEM(IfsSwitchHigh);					// Endschalter Einschneckenextruder oben bzw. �berlauf aktiviert
	INIT_ITEM(IfsSwitchHighEnable);					// Endschalter Einschneckenextruder oben bzw. �berlauf aktiviert
	INIT_ITEM(IfsSwitchLow);					// Endschalter Einschneckenextruder unter bzw. Leer  aktiviert
	INIT_ITEM(IfsSwitchLowEnable);					// Endschalter Einschneckenextruder unter bzw. Leer  aktiviert
	INIT_ITEM(IfsSwitchMax);					// Endschalter Max, oberer Umkehrpunkt  aktiviert
	INIT_ITEM(IfsSwitchMaxEnable);					// Endschalter Max, oberer Umkehrpunkt  aktiviert
	INIT_ITEM(IfsSwitchMin);					// Endschalter Min, unterer Umkehrpunkt  aktiviert
	INIT_ITEM(IfsSwitchMinEnable);					// Endschalter Min, unterer Umkehrpunkt  aktiviert
	INIT_ITEM(IfsEmptyFeeder);				    // Dosierer leer erreicht
	INIT_ITEM(IfsOverflowFeeder);				// Dosierer Überlauf

    //aItem.bChangedSetpoint           = TRUE;

	INIT_ITEM(LogoEncoderSampleTime);
	INIT_ITEM(LogoRunControlEnable);
	INIT_ITEM(LogoStartState);
	INIT_ITEM(ActualLogoEncoder);
}
//******************************************************************************************
//******************************************************************************************
void CDose :: ResetLineItem ()
{ 	
	std::lock_guard<std::mutex> lock(mEGlobal);			        \

	CGlobalLineItem& aItem = mEGlobal.get();
	INIT_ITEM(LineMassflow);
	INIT_ITEM(LineMassflowMF);
	INIT_ITEM(LineMassflowSF1);
	INIT_ITEM(LineMassflowSF2);

#if 0 // Darf nicht auf 0 gesetzt werden
	INIT_ITEM(TotalizerAll);
	INIT_ITEM(TotalizerLine);
	INIT_ITEM(TotalizerMF);
	INIT_ITEM(TotalizerSF1);
	INIT_ITEM(TotalizerSF2);
#endif


	INIT_ITEM(StateBetriebszustand)
	INIT_ITEM(ErrorCode)
	INIT_ITEM(LineDoseStarted)
	INIT_ITEM(DoseStarted)

	INIT_ITEM(ServiceMode)
	INIT_ITEM(MBLineSetpoint)
	INIT_ITEM(ExtLineSetpoint)
	INIT_ITEM(ExtLineOffset)
	INIT_ITEM(MBLinePercentage)
	INIT_ITEM(LineSetpoint);
	INIT_ITEM(ManualSetpoint);
	INIT_ITEM(ExternalSetpoint);


	INIT_ITEM(IORampUpInput)
	INIT_ITEM(IORampDownInput)
	INIT_ITEM(RampUp)
	INIT_ITEM(RampDown)
	INIT_ITEM(LineRelease)

	INIT_PB(PBAlarm03Quit)
	INIT_PB(PBRampUp)
	INIT_PB(PBRampDown)
	INIT_PB(PBLineStart)
	INIT_PB(PBLineStop)
	INIT_PB(PBAlarm03Quit)
	INIT_PB(PBAlarm1Quit)
	INIT_PB(PBClearTotalizer)
	INIT_PB(PBAlarm0);			// Alarmquit extern 1-3
	INIT_PB(PBAlarm1)
	INIT_PB(PBAlarm2)
	INIT_PB(PBAlarm3)
	INIT_PB(PBServiceMode)

	INIT_ITEM(IOLineSetpointEnableInput)
	INIT_ITEM(LineStart)
	INIT_ITEM(IOExtAlarmInput)
	INIT_ITEM(IOAlarmShutDownInput)			// ShutDown extern
	INIT_ITEM(IOWindowsShutDownInput)		// Windows ShutDown extern
	INIT_ITEM(AlarmShutDown)				// AlarmShutDownState
	INIT_ITEM(IOSyncErrorExtern)			// ShutDown extern
	INIT_ITEM(IOSyncError)					// SyncErrorState

	INIT_ITEM(IOCustomerAlarmInput)
	INIT_ITEM(IOLineSetpointEnableInput)
	INIT_ITEM(RampUp)
	INIT_ITEM(RampDown)
	INIT_ITEM(IORampUpInput)
	INIT_ITEM(IORampDownInput)
	INIT_ITEM(IOMainFeederInput)
	INIT_ITEM(IOSideFeeder1Input)
	INIT_ITEM(IOSideFeeder2Input)
	INIT_ITEM(IOOperateAutoInput)
	INIT_ITEM(IOOperateManualInput)
	INIT_ITEM(IOOperateAutoOutput)			// IO operate auto output
	INIT_ITEM(IOServiceModeOutput);			// IO operate service output


	INIT_ITEM(LineDoseStarted)
	INIT_ITEM(LineDoseWaitStart)
	INIT_ITEM(DoseStarted)

	INIT_ITEM(Alarm1QuitState)
	INIT_ITEM(Alarm03QuitState)
	INIT_ITEM(IOAlarm1QuitInput)
	INIT_ITEM(IOAlarm03QuitInput)

	INIT_ITEM(Alarm0State)
	INIT_ITEM(Alarm1State)
	INIT_ITEM(Alarm2State)
	INIT_ITEM(Alarm3State)
	INIT_ITEM(IOLineReleaseOutput)
	INIT_ITEM(FeederAlarm);								// Unterer Gewichtswert

	INIT_ITEM(IOAlarm0Output)
	INIT_ITEM(IOAlarm1Output)
	INIT_ITEM(IOAlarm2Output)
	INIT_ITEM(IOAlarm3Output)
	INIT_ITEM(IOTotalizerPulseOutput)
	INIT_ITEM(IOLineSetpointInput);			// ext. Liniensollwert
	INIT_ITEM(IOUPSLowInput)
	INIT_ITEM(IOOperateAutoOutput)			// IO operate auto output
	INIT_ITEM(IOServiceModeOutput);			// IO operate service output


	INIT_ITEM(ExtLineInvSetpointState)
	INIT_ITEM(ExtLineSetpointState)		// Save state
	INIT_ITEM(UPSLowInputState)					// UPS low

	base::utils::baseZeroMemory(aItem.AlarmCfg);
}
//******************************************************************************************
//******************************************************************************************
void CDose :: InitializeDoseItem (int32_t s)
{ 	
	std::lock_guard<std::mutex> lock(mDGlobal[s]);			        \

	CGlobalDoseItem& aItem = mDGlobal[s].get();
	base::utils::baseZeroMemory(aItem);
	//aItem.VNr.Init(_S32(s+1));
	aItem.DoseType.Init(base::eDoseType::DOSETYPE_LWS);
	aItem.ActualDoseperformance.Init(100.0f);
	aItem.ActualDriveCommand.Init(0.0f);
	aItem.MaxSetpoint.Init(100.0f);
	aItem.Percentage.Init(10.0f);
	aItem.NominalSetpoint.Init(10.0f);

	aItem.RefillLimitMax.Init(30.0f);
	aItem.RefillLimitMin.Init(10.0f);
	aItem.RefillLimitMinMin.Init(5.0f);
	aItem.RefillTime.Init(30);
    aItem.RefillSwitchDelay.Init(10);
	aItem.RefillDebounceMax.Init(4U);				// RefillDebounceMax
	aItem.RefillDebounceMin.Init(4U);				// RefillDebounceMin
	aItem.RefillFeederEmptyStart.Init(20U);		// MinDurationActive
	INIT_ITEM(RefillWeightAboveMax);
	INIT_ITEM(RefillWeightBelowMin);
	INIT_ITEM(RefillFeederEmptyStarted);
	INIT_ITEM(RefillFeederEmptyEnded);


	{
		base::refillsignal::RefillSignalType lType;
		base::utils::baseZeroMemory(lType);
		aItem.RefillSignalCfg.Init(lType);
	}

	{
		aItem.CalibAktIndex.Init(0);
		auto& rItem = aItem.CalibCfg.Get();
		rItem.count= 1U;

		uint32_t i = 0;
		for (; i < rItem.count; i++)
		{
			rItem.fDriveCommand[i] = 30.0F;
			rItem.lMeasureTime[i] = 60U;
			rItem.fDosePerformance[i] = 100.0F;
		}
		for (; i < base::calib::MAXCALIBITEMS; i++)
		{
			rItem.fDriveCommand[i] = 0.0F;
			rItem.lMeasureTime[i] = 0L;
			rItem.fDosePerformance[i] = 0.0F;
		}
	}

	aItem.MinSetpointChange.Init(0.0F);
	aItem.MaxRotationalSpeed.Init(3000.0f);
	aItem.PidSampleInterval.Init(4);		// PID-SampleInterval
	aItem.EncoderMonitor.Init(10);
	aItem.MaxDriveCommand.Init(20);
	aItem.SteepnessMassflow.Init(1.0F);

	// LCL
	aItem.LclWeightMinLevel.Init(1.0F);
	aItem.LclWeightMaxLevel.Init(10.0F);
	aItem.LclWeightHysteresis.Init(1.0F);
	INIT_ITEM(LclWeightMinLevelActive);
	INIT_ITEM(LclWeightMaxLevelActive);

	aItem.IflMinLineSetpoint.Init(0.0F);			// Oberer Gewichtswert
	aItem.IflMaxLineSetpoint.Init(100.0F);			// Oberer Gewichtswert

	// IFS / IFL
	INIT_ITEM(IfsSwitchHigh);					// Endschalter Einschneckenextruder oben bzw. �berlauf aktiviert
	INIT_ITEM(IfsSwitchHighEnable);					// Endschalter Einschneckenextruder oben bzw. �berlauf aktiviert
	INIT_ITEM(IfsSwitchLow);					// Endschalter Einschneckenextruder unter bzw. Leer  aktiviert
	INIT_ITEM(IfsSwitchLowEnable);					// Endschalter Einschneckenextruder unter bzw. Leer  aktiviert
	INIT_ITEM(IfsSwitchMax);					// Endschalter Max, oberer Umkehrpunkt  aktiviert
	INIT_ITEM(IfsSwitchMaxEnable);					// Endschalter Max, oberer Umkehrpunkt  aktiviert
	INIT_ITEM(IfsSwitchMin);					// Endschalter Min, unterer Umkehrpunkt  aktiviert
	INIT_ITEM(IfsSwitchMinEnable);					// Endschalter Min, unterer Umkehrpunkt  aktiviert
	INIT_ITEM(IfsEmptyFeeder);				    // Dosierer leer erreicht
	INIT_ITEM(IfsOverflowFeeder);				// Dosierer Überlauf


	aItem.IfsReduceFactor.Init(10.0);			// Pegel ist �ber Max, Sollwert wird um Reducefaktor verringert
	aItem.IfsGainFactor.Init(10.0);				// Pegel ist unter Min gefallen, Sollwert wird um Gainfactor erh�ht
	aItem.IfsFeederOverflowTimeOut.Init(20);				// Pegel ist �ber Max,  Sollwert wurde verringert, aber Zeit um unter Max zu fallen ist verstrichen
	aItem.IfsFeederEmptyTimeOut.Init(20);	// Anteil um wieviel der Sollwert weiter verringert wird, wenn LimitMaxTimeOut erreicht wurde
	aItem.IfsStepTimeGain.Init(20);		// Anteil um wieviel der Sollwert weiter verringert wird, wenn LimitMaxTimeOut erreicht wurde
	aItem.IfsStepTimeReduce.Init(20);		// Anteil um wieviel der Sollwert weiter verringert wird, wenn LimitMaxTimeOut erreicht wurde
	aItem.IfsDebounceMax.Init(3U);				// Debounce Max / High
	aItem.IfsDebounceMin.Init(3U);				// Debounce Min / Low
	aItem.IfsSetpointOverflow.Init(0.0F);


	aItem.WbfReduction.Init(86);
	aItem.WbfWheelSize.Init(114L);
	aItem.WbfWeighingLine.Init(360L);
	aItem.WbfTareDriveCommand.Init(20.0f);		// DriveCommand fuer Tarierung
	aItem.WbfTareMeasurementTime.Init(30L);			// Messzeit fuer Tarierung
	aItem.WbfBeltLoadSetpoint.Init(1.0f);
	aItem.WbfBeltLoadMassflow.Init(0.0f);			// Bandlastistwert in kg/m
	aItem.WbfBeltLoadVol.Init(1.0f);				// Umschaltgrenze f. volumetrisch
	aItem.WbfActVelocity.Init(0.0f);				// Aktuelle Bandgeschwindigkeit
	aItem.WbfAlarmMinBeltLoad.Init(0.0f);
	aItem.WbfMinBeltLoadState.Init(base::eWbmState::WBFMINBELTLOAD_INVALID);
	INIT_ITEM(WbfBeltLoadLimit);


	aItem.BatchGrossStream.Init(100.0F);			// Maximalgeschwindigkeit [%]
	aItem.BatchFineStream.Init(10.0F);			// Maximalgeschwindigkeit [%]

	aItem.BatchSwitchpoint.Init(10U);			// Abbremsung [s]
	aItem.BatchTime.Init(0);
	aItem.BatchOverrunCompensation.Init(0.0F);
	INIT_ITEM(BatchStarted);
	INIT_ITEM(BatchFinished);			    // Batch finished
	aItem.GwbExtScale.Init(0);

	// Allgemeine Alarme
	aItem.AlarmNoiseLimit.Init(400);
	aItem.AlarmReactionDelay.Init(30);
	aItem.AlarmStartReactionDelay.Init(60);
	aItem.AlarmMassflowLow.Init(10L);
	aItem.AlarmMassflowHigh.Init(10L);
	aItem.AlarmDriveCommandLow.Init(3);
	aItem.AlarmDriveCommandHigh.Init(99);
	aItem.AlarmMaxBatchTime.Init(100);


	aItem.WbfAlarmMinBeltLoad.Init(0.5f);
	aItem.PidPropGainFine.Init(20.0f);
	aItem.PidPropGainGross.Init(20.0f);
	aItem.PidPropGainSwitchGrossFine.Init(20.0f);
	aItem.PidIntegralGain.Init(10.0f);
	aItem.MassflowFilter.Init(10.0f);
	aItem.PidDriveCommandInv.Init(FALSE);
	aItem.PidGatefilter.Init(100);
    aItem.PidGatefilterState.Init(base::eGatefilter::GATEFILTER_NONE);


	aItem.EmptyFeederRuntime.Init(60*60);
	aItem.EmptyFeederRestriction.Init(eEmptyFeederType::eNone);
	aItem.EmptyFeederSpeed.Init(10.0F);

	aItem.FeederType.Init(base::eFeedingType::FF_MF);
	// ICP
	INIT_ITEM(LCAktiv);
	INIT_ITEM(LCEnable);
	INIT_ITEM(Release);
	INIT_ITEM(IOStartInput);
	INIT_ITEM(IOReleaseInput);
	INIT_ITEM(IOOperateOutput);
	INIT_ITEM(IOStartOutput);					// Start

	INIT_ITEM(SlaveMode)					//SlaveMode
	INIT_ITEM(Service);
	INIT_ITEM(IODriveReleaseOutput);
	INIT_ITEM(IOMotorLoadInput);	    // Drive Motorlast
	INIT_ITEM(IOExtAlarmInput);				// ext. Alarm


	INIT_PB(PBDosiererStart);
	INIT_PB(PBDosiererStop);
	INIT_PB(PBClearAlarm);

	INIT_ITEM(IOVolumetricInput);
	INIT_ITEM(IOEmptyFeederStartInput);	// Externel empty feeder start
	INIT_ITEM(IOEmptyFeederStopInput);	// Externel empty feeder stop
	INIT_ITEM(IORefillAlarmInput)		// Externel Refill alarm
	INIT_ITEM(IORefillStopInput);		// Refillstopp durch (IO)
	INIT_ITEM(IORefillReleaseInput);	// Refillrelease durch (IO)
	INIT_ITEM(IORefillInput);			// Refill durch (IO)
	INIT_ITEM(IORefillRequestInput);	// Refillstart durch (IO)
	INIT_ITEM(IORefillMaxSensorInput);	    // Refillrequest durch (IO)
	INIT_ITEM(IORefillMinSensorInput);	    // Refillrelease durch (IO)
	INIT_ITEM(IODriveErrorInput);	    // Fehler Drive ( Frequenzumrichter
	INIT_ITEM(IODriveErrorPaddleInput);	    // Fehler Drive Paddle ( Frequenzumrichter )
	INIT_ITEM(IODriveErrorRefillFeederInput);	  // Fehler Drive RefillFeeder ( Frequenzumrichter )

	aItem.IOEncoderSpeedInput.Init(0.0F);


	INIT_ITEM(IORefillRequestOutput);	// Refillrequest output (IO)

	INIT_ITEM(RefillReleaseState);		// bIORefillStopInput || bRefillStopUser
	INIT_ITEM(RefillStartState);		// Refillzustand start
	INIT_ITEM(RefillStopState);			// Refillzustand start
	INIT_ITEM(RefillState);				// Refill Zustand
	INIT_ITEM(RefillFeederEnable);	    // Refill screw enable

	INIT_ITEM(AgitatorEnable);
	INIT_ITEM(AgitatorRunState);

	INIT_PB(PBDosiererStart);
	INIT_PB(PBDosiererStop);
	INIT_PB(PBClearAlarm);
	INIT_PB(PBAcknowledgeAlarm);
	INIT_PB(PBRefillStart);
	INIT_PB(PBRefillStop);
	INIT_PB(PBVolumetric);
	INIT_PB(PBGravimetric);
	INIT_PB(PBInline);
	INIT_PB(PBLocal);
	INIT_PB(PBCalibStart);
	INIT_PB(PBCalibStop);
	INIT_PB(PBTareStart);
	INIT_PB(PBTareStop);
	INIT_PB(PBEmptyFeederStart);
	INIT_PB(PBEmptyFeederStop);
	INIT_PB(PBClearTotalizer);

	INIT_ITEM(RefillReleaseTrigger);
	INIT_ITEM(Regenerat);
	INIT_ITEM(LocalModeState);					// Nicht setzten

    INIT_ITEM(SWRefillRelease)
    INIT_ITEM(GravVolState)

	INIT_ITEM(RefillControlEnable)
	INIT_ITEM(CalibrationState);
	INIT_ITEM(TaraState);
	INIT_ITEM(EmptyFeederState);
	INIT_ITEM(DriveControlReady);


	INIT_ITEM(AlarmCfg);

	INIT_ITEM(Alarm);
	INIT_ITEM(Warning);
	INIT_ITEM(ClearAlarm);
	INIT_ITEM(ClearWarning);

	aItem.Name.Init("TEST");
	aItem.QMNumber.Init(123456789U);
	aItem.LCTyp.Init(base::eLcType::LC_EMPTY);

	aItem.LCCorrectionFactor.Init(1.0F);
	aItem.LCTaraWeight.Init(0.0F);
	aItem.LCActualWeight.Init(0.0F);
	aItem.LCMeanWeight.Init(0.0F);
	aItem.LCRawWeight.Init(0.0F);


	INIT_ITEM(ModuleRefillEnable);
	INIT_ITEM(LogoEncoderSampleTime);
	INIT_ITEM(LogoRunControlEnable);
	INIT_ITEM(LogoStartState);
	INIT_ITEM(ActualLogoEncoder);
}
//******************************************************************************************
//******************************************************************************************
void CDose :: InitializeLineItem ()
{ 	
	std::lock_guard<std::mutex> lock(mEGlobal);

	CGlobalLineItem& aItem = mEGlobal.get();
	base::utils::baseZeroMemory(aItem);
	aItem.MaxLeistung.Init(1000.f);
	aItem.RampStep.Init(10.f);
	aItem.Skalierung.Init(100.f);
    aItem.Hysterese.Init(0);
	aItem.RampDelay.Init(30);
    aItem.MinTotband.Init(0.0f);
    aItem.Filterung.Init(0);
	aItem.ANNumber.Set("AN12345678");

	aItem.TotalizerPulseStep.Init(10.0F);						// Schrittweite Totalisatorimpuls [kg]
	aItem.TotalizerPulseDuration.Init(1U);							// Dauer des Impulses [s]
	aItem.ProductionTime.Init(0L);

	INIT_ITEM(ModuleExternalSetpoint);
	INIT_ITEM(ModuleSetpointRamp);
	INIT_ITEM(ModuleLineSetpoint);
	INIT_ITEM(ModuleLocalSetpoint);
	INIT_ITEM(ModuleBatcher);
	INIT_ITEM(ModuleTotalizer)
	INIT_ITEM(ModuleTotalizerPulse)

	INIT_ITEM(MBLineSetpoint)
	INIT_ITEM(ExtLineSetpoint)
	INIT_ITEM(ExtLineOffset)
	INIT_ITEM(MBLinePercentage)
	INIT_ITEM(LineSetpoint);
	INIT_ITEM(ManualSetpoint);
	INIT_ITEM(ExternalSetpoint);
	INIT_ITEM(ServiceMode)
	INIT_ITEM(ExtLineSetpointEnable)		// Save state
	INIT_ITEM(ExtLineInvSetpointEnable)
	INIT_ITEM(ExtLineInvSetpointState)
	INIT_ITEM(ExtLineSetpointState)		// Save state


	INIT_ITEM(IOLineReleaseOutput)
	INIT_ITEM(FeederAlarm);		
	
	INIT_ITEM( IOLineStartInput);			// externer Start
	INIT_ITEM( IOCustomerAlarmInput);		// KundenAlarm
	INIT_ITEM( IOExtAlarmInput);			// VorlageAlarm
	INIT_ITEM( IOLineSetpointEnableInput);		// Sensorbetrieb Extruder IO
	INIT_ITEM( IOLineInvSetpointEnableInput);		// Inverser Levelsensoreingang
	INIT_ITEM( IORampUpInput);				// Rampe hoch durch Knopfdruck
	INIT_ITEM( IORampDownInput);			// Rampe runter durch Knopfdruck
	INIT_ITEM( IOAlarmShutDownInput);		// ShutDown extern
	INIT_ITEM( IOWindowsShutDownInput)		// Windows ShutDown extern
	INIT_ITEM( IOAlarm1QuitInput);	        // Alarmquit extern 1
	INIT_ITEM( IOAlarm03QuitInput);			// Alarmquit extern 1-3
	INIT_ITEM( IOSyncErrorExtern);			// ShutDown extern
	INIT_ITEM( IOSyncError);				// SyncErrorState
	INIT_ITEM( IOMainFeederInput);			// Haupteinzug
	INIT_ITEM( IOSideFeeder1Input);		// Seiteneinzug 1
	INIT_ITEM( IOSideFeeder2Input);		// Seiteneinzug 2
	INIT_ITEM( IOOperateAutoInput)				// Automatikbetrieb
	INIT_ITEM( IOOperateManualInput)			// Handbetrieb
	INIT_ITEM( IOAlarm0Output);			// Was aktuell am Ausgang 0 anliegt
	INIT_ITEM( IOAlarm1Output);			// Was aktuell am Ausgang 1 anliegt
	INIT_ITEM( IOAlarm2Output);			// Was aktuell am Ausgang 2 anliegt
	INIT_ITEM( IOAlarm3Output);			// Was aktuell am Ausgang 3 anliegt
	INIT_ITEM( IOTotalizerPulseOutput);	// Totalisator impulse
	INIT_ITEM( IOLineSetpointInput);			// ext. Liniensollwert
	INIT_ITEM( IOUPSLowInput)
	INIT_ITEM(IOOperateAutoOutput)			// IO operate auto output
	INIT_ITEM(IOServiceModeOutput);			// IO operate service output

	INIT_ITEM(TotalizerAll);
	INIT_ITEM(TotalizerLine);
	INIT_ITEM(TotalizerMF);
	INIT_ITEM(TotalizerSF1);
	INIT_ITEM(TotalizerSF2);

	INIT_PB(PBServiceMode)

// Unterer Gewichtswert
}
//******************************************************************************************
//******************************************************************************************
void CDose :: ResetSystem (void)
{ 	
	std::lock_guard<std::mutex> lock(mSGlobal);

	CGlobalSystemItem& aG = mSGlobal.get();
	aG.sMaxItems          = _U32(RANGE(aG.sMaxItems, 1, GLOBALDOSEMAXCOUNT));
}
//******************************************************************************************
//******************************************************************************************
void CDose :: InitializeSystem (void)
{ 	
	std::lock_guard<std::mutex> lock(mSGlobal);
	CGlobalSystemItem& aG = mSGlobal.get();
	base::utils::baseZeroMemory(aG);
	aG.sMaxItems                    = __min(4, GLOBALDOSEMAXCOUNT);
}
//******************************************************************************************
//******************************************************************************************
void CDose :: InitializeDoseItems (void)
{ 	
	for (int32_t i = 0; i < GLOBALDOSEMAXCOUNT; i++)
	{
		InitializeDoseItem(i);
	}
}
//******************************************************************************************
//******************************************************************************************
void CDose :: InitializeAll (void)
{ 	
	InitializeDoseItems ();
	InitializeLineItem ();
	InitializeSystem ();
}
//******************************************************************************************
//******************************************************************************************
void CDose :: ResetDoseItems (void)
{ 	
	for (int32_t i = 0; i < GLOBALDOSEMAXCOUNT; i++)
	{
		ResetDoseItem(i);
	}
	uint32_t sN = 0;
	System_GetDosierItems(&sN);
    for (int32_t i = sN; i < GLOBALDOSEMAXCOUNT; i++)
    {
        ECSetLocalModeState(i, TRUE);
    }
}
//******************************************************************************************
//******************************************************************************************
void CDose :: ResetAll ( void )
{ 	
	ResetLineItem  ();
	ResetDoseItems    ();
}

