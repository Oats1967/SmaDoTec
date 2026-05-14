//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Declaration of module EXControl.h
///
/// @file   EXControl.h
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

#include "DoseTypes.h"
#include "ExImplMacro.h"


IMPLEMENT_INLINE_EX( BOOL, LineStart)
IMPLEMENT_INLINE_EX( BOOL, LineStarted)
IMPLEMENT_INLINE_EX( BOOL, IOLineStartInput)
IMPLEMENT_INLINE_EX( BOOL, IOCustomerAlarmInput)
IMPLEMENT_INLINE_EX( BOOL, IOExtAlarmInput)
IMPLEMENT_INLINE_EX( BOOL, IOLineSetpointEnableInput)
IMPLEMENT_INLINE_EX( BOOL, IOLineInvSetpointEnableInput)
IMPLEMENT_INLINE_EX( BOOL, IOAlarmShutDownInput)		// ShutDown extern
IMPLEMENT_INLINE_EX( BOOL, IOWindowsShutDownInput)		// Windows ShutDown extern

IMPLEMENT_INLINE_EX( BOOL, IOSyncErrorExtern)		// IOSyncErrorExtern
IMPLEMENT_INLINE_EX( BOOL, IOSyncError)				// SyncErrorState
IMPLEMENT_INLINE_EX( BOOL, IORampUpInput)
IMPLEMENT_INLINE_EX( BOOL, IORampDownInput)
IMPLEMENT_INLINE_EX( BOOL, IOMainFeederInput)
IMPLEMENT_INLINE_EX( BOOL, IOSideFeeder1Input)
IMPLEMENT_INLINE_EX( BOOL, IOSideFeeder2Input)
IMPLEMENT_INLINE_EX( BOOL, IOOperateAutoInput)				// Automatikbetrieb
IMPLEMENT_INLINE_EX( BOOL, IOOperateManualInput)				// Handbetrieb
IMPLEMENT_INLINE_EX( BOOL, IOUPSLowInput);	// Totalisator impulse
IMPLEMENT_INLINE_EX(BOOL, IOOperateAutoOutput)			// IO operate auto output
IMPLEMENT_INLINE_EX(BOOL, IOServiceModeOutput)			// IO operate service output


IMPLEMENT_INLINE_EX( BOOL, IOAlarm1QuitInput);			// Alarmquit extern 1
IMPLEMENT_INLINE_EX( BOOL, IOAlarm03QuitInput);			// Alarmquit extern 0-3
IMPLEMENT_INLINE_EX( BOOL, IOAlarm0Output);			    // Alarm 0
IMPLEMENT_INLINE_EX( BOOL, IOAlarm1Output);			    // Alarm 1
IMPLEMENT_INLINE_EX( BOOL, IOAlarm2Output);			    // Alarm 2
IMPLEMENT_INLINE_EX( BOOL, IOAlarm3Output);			    // Alarm 3
IMPLEMENT_INLINE_EX( BOOL, IOTotalizerPulseOutput);	// Totalisator impulse
IMPLEMENT_INLINE_EX(float32_t, IOLineSetpointInput);		// ext. Liniensollwert


IMPLEMENT_INLINE_EX( BOOL, ServiceMode)
IMPLEMENT_INLINE_EX( BOOL, AlarmShutDown)
IMPLEMENT_INLINE_EX( BOOL, RampUp)
IMPLEMENT_INLINE_EX( BOOL, RampDown)
IMPLEMENT_INLINE_EX( BOOL, ExtLineSetpointState)
IMPLEMENT_INLINE_EX( BOOL, UPSLowInputState)					// UPS low
IMPLEMENT_INLINE_EX( BOOL, ExtLineInvSetpointState)
IMPLEMENT_INLINE_EX( float32_t, Hysterese)
IMPLEMENT_INLINE_EX( float32_t, Skalierung)
IMPLEMENT_INLINE_EX( float32_t, RegeneratPercentage)
IMPLEMENT_INLINE_EX( float32_t, RecipeSetpoint)
IMPLEMENT_INLINE_EX( float32_t, ManualSetpoint);
IMPLEMENT_INLINE_EX(float32_t, ExternalSetpoint);			// externer  Liniensollwert


IMPLEMENT_INLINE_EX( float32_t, LineSetpoint)
IMPLEMENT_INLINE_EX( float32_t, LineMassflow)
IMPLEMENT_INLINE_EX( float32_t, LineMassflowMF)
IMPLEMENT_INLINE_EX( float32_t, LineMassflowSF1)
IMPLEMENT_INLINE_EX(float32_t,  LineMassflowSF2)
//IMPLEMENT_INLINE_EX( float32_t, DosiererMischung)
IMPLEMENT_INLINE_EX( float32_t, MaxLeistung)
IMPLEMENT_INLINE_EX( float32_t, MinTotband)
IMPLEMENT_INLINE_EX( uint32_t,  Filterung)
IMPLEMENT_INLINE_EX( float32_t, RampStep)
IMPLEMENT_INLINE_EX( uint32_t,  RampDelay)
IMPLEMENT_INLINE_EX(float32_t, TotalizerAll)
IMPLEMENT_INLINE_EX(float32_t, TotalizerLine)
IMPLEMENT_INLINE_EX (float32_t, TotalizerMF)
IMPLEMENT_INLINE_EX( float32_t, TotalizerSF1)
IMPLEMENT_INLINE_EX( float32_t, TotalizerSF2)
IMPLEMENT_INLINE_EX( uint32_t, ProductionTime)
IMPLEMENT_INLINE_EX( BOOL,	Alarm03QuitState);			// Alarmquit 0-3
IMPLEMENT_INLINE_EX( BOOL,	Alarm1QuitState);			// Alarmquit 1

IMPLEMENT_INLINE_EX( BOOL,  Alarm0State)                // Alarmquit 0
IMPLEMENT_INLINE_EX( BOOL,	Alarm1State);			    // Alarmquit 1
IMPLEMENT_INLINE_EX( BOOL,	Alarm2State);			    // Alarmquit 2
IMPLEMENT_INLINE_EX( BOOL,	Alarm3State);			    // Alarmquit 3
IMPLEMENT_INLINE_EX( BOOL,  IOLineReleaseOutput)			// Was aktuell am Ausgang 3 anliegt
IMPLEMENT_INLINE_EX(uint32_t, FeederAlarm)
IMPLEMENT_INLINE_EX(float32_t, TotalizerPulseStep)						// Schrittweite Totalisatorimpuls [kg]
IMPLEMENT_INLINE_EX(uint32_t, TotalizerPulseDuration)					// Dauer des Impulses [s]

IMPLEMENT_INLINE_EX( float32_t, MBLineSetpoint);
IMPLEMENT_INLINE_EX( float32_t, MBLinePercentage);
IMPLEMENT_INLINE_EX(float32_t, ExtLineSetpoint)
IMPLEMENT_INLINE_EX(float32_t, ExtLineOffset)



IMPLEMENT_INLINE_EX_TSTRING(Name)
IMPLEMENT_INLINE_EX_TSTRING(ANNumber)
IMPLEMENT_INLINE_EX(uint64_t, QMNumber)


IMPLEMENT_INLINE_EX(BOOL, ModuleExternalSetpoint)
IMPLEMENT_INLINE_EX(BOOL, ModuleSetpointRamp)
IMPLEMENT_INLINE_EX(BOOL, ModuleLineSetpoint)
IMPLEMENT_INLINE_EX(BOOL, ModuleLocalSetpoint)
IMPLEMENT_INLINE_EX(BOOL, ModuleBatcher)
IMPLEMENT_INLINE_EX(BOOL, ModuleTotalizer)
IMPLEMENT_INLINE_EX(BOOL, ModuleTotalizerPulse)


IMPLEMENT_INLINE_EX(BOOL, LineReleaseEnable);      // Enable Line Release
IMPLEMENT_INLINE_EX(BOOL, LineStartEnable);        // Enable Line Start
IMPLEMENT_INLINE_EX(BOOL, LineRelease);
IMPLEMENT_INLINE_EX(BOOL, ExtLineSetpointEnable)
IMPLEMENT_INLINE_EX(BOOL, ExtLineInvSetpointEnable)

IMPLEMENT_INLINE_EXPB( PBAlarm03Quit)
IMPLEMENT_INLINE_EXPB( PBAlarm1Quit)
IMPLEMENT_INLINE_EXPB( PBRampUp)
IMPLEMENT_INLINE_EXPB( PBRampDown)
IMPLEMENT_INLINE_EXPB( PBLineStart)
IMPLEMENT_INLINE_EXPB( PBLineStop)
IMPLEMENT_INLINE_EXPB( PBClearTotalizer)
IMPLEMENT_INLINE_EXPB( PBLineShutDown)
IMPLEMENT_INLINE_EXPB( PBServiceMode)

IMPLEMENT_INLINE_EXPB(PBAlarm0)
IMPLEMENT_INLINE_EXPB(PBAlarm1)
IMPLEMENT_INLINE_EXPB(PBAlarm2)
IMPLEMENT_INLINE_EXPB(PBAlarm3)


//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
int32_t CDose::EXSetLineDoseWaitStart(const int32_t k, const BOOL b)
{
	assert(k >= 0 && k < GLOBALDOSEMAXCOUNT);
	assert((b == 1) || (b == 0));
	auto& item = mEGlobal.get();
	auto value = item.LineDoseWaitStart.Get();
	SETBIT(value, k, b);
	if (item.LineDoseWaitStart.Test(value))
	{
		std::lock_guard<std::mutex> lock(mEGlobal);
		item.LineDoseWaitStart.Set(value);
	}
	return DOSIERUNG_OK;
}


//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
int32_t CDose::EXDeleteLineDoseWaitStart(void)
{
	std::lock_guard<std::mutex> lock(mEGlobal);
	auto& item = mEGlobal.get();
	item.LineDoseWaitStart.Reset();
	return DOSIERUNG_OK;
}
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
int32_t CDose::EXGetAnyLineDoseWaitStart(BOOL* b) const
{
	const auto& item = mEGlobal.get();
	const auto& value = item.LineDoseWaitStart.Get();
	*b = BOOL(value != 0);
	return DOSIERUNG_OK;
}


//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
int32_t CDose::EXGetLineDoseWaitStart(const int32_t k, BOOL* b) const
{
	assert(k >= 0 && k < GLOBALDOSEMAXCOUNT);
	const auto& item = mEGlobal.get();
	const auto& value = item.LineDoseWaitStart.Get();
	*b = ISBIT(value, k);
	return DOSIERUNG_OK;
}


//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 int32_t CDose :: EXSetLineDoseStarted      (  const int32_t k, const BOOL b)
{
	assert(k >= 0 && k < GLOBALDOSEMAXCOUNT);
	assert((b == 1) || (b == 0));
	auto& item = mEGlobal.get();
	auto value = item.LineDoseStarted.Get();
    SETBIT(value, k, b);
	if (item.LineDoseStarted.Test(value))
	{
		std::lock_guard<std::mutex> lock(mEGlobal);
		item.LineDoseStarted.Set(value);
	}
	return DOSIERUNG_OK;
}


//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 int32_t CDose :: EXDeleteLineDoseStarted      ( void )
{
	std::lock_guard<std::mutex> lock(mEGlobal);
	auto& item = mEGlobal.get();
	item.LineDoseStarted.Reset();
	return DOSIERUNG_OK;
}


//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 int32_t CDose :: EXGetLineDoseStarted      ( const int32_t k, BOOL *b) const
{
	assert(k >= 0 && k < GLOBALDOSEMAXCOUNT);
	const auto& item = mEGlobal.get();
	const auto& value = item.LineDoseStarted.Get();
	*b = ISBIT(value, k);
	return DOSIERUNG_OK;
}
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 int32_t CDose :: EXGetAnyLineDoseStarted   ( BOOL *b) const
{
	const auto& item = mEGlobal.get();
	const auto& value = item.LineDoseStarted.Get();
	*b = BOOL(value != 0);
	return DOSIERUNG_OK;
}
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 int32_t CDose :: EXSetDoseStarted     ( const int32_t k, const BOOL b)
{
	assert(k >= 0 && k < GLOBALDOSEMAXCOUNT);
	auto& item = mEGlobal.get();
	auto value = item.DoseStarted.Get();
	SETBIT(value, k, b);
	if (item.DoseStarted.Test(value))
	{
		std::lock_guard<std::mutex> lock(mEGlobal);
		item.DoseStarted.Set(value);
	}
	return DOSIERUNG_OK;
}
 //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 int32_t CDose :: EXDeleteDoseStarted     ( void )
{
	std::lock_guard<std::mutex> lock(mEGlobal);
	auto& item = mEGlobal.get();
	item.DoseStarted.Reset();
	return DOSIERUNG_OK;
}
 //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 int32_t CDose :: EXGetDoseStarted     ( const int32_t k, BOOL *b) const
{
	assert(k >= 0 && k < GLOBALDOSEMAXCOUNT);
	const auto& item = mEGlobal.get();
	const auto& value = item.DoseStarted.Get();
	*b = ISBIT(value, k);
	return DOSIERUNG_OK;
}
 //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 int32_t CDose :: EXGetAnyDoseStarted  ( BOOL *b) const
{
	const auto& item = mEGlobal.get();
	const auto& value = item.DoseStarted.Get();
	*b = BOOL(value != 0);
	return DOSIERUNG_OK;
}


