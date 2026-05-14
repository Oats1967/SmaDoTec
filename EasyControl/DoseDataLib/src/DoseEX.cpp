#include <cassert>
#include "Dose.h"
#include "DoseDataImplMacro.h"

IMPLEMENT_DOSEEX_STRING(std::string, Name)
IMPLEMENT_DOSEEX_STRING(std::string, ANNumber)
IMPLEMENT_DOSEEX(uint64_t, QMNumber)

IMPLEMENT_DOSEEX(BOOL, LineStart)
IMPLEMENT_DOSEEX(BOOL, IOLineStartInput)
IMPLEMENT_DOSEEX(BOOL, LineStarted)
IMPLEMENT_DOSEEX(BOOL, IOCustomerAlarmInput)
IMPLEMENT_DOSEEX(BOOL, IOExtAlarmInput)
IMPLEMENT_DOSEEX(float32_t, Hysterese)
IMPLEMENT_DOSEEX(float32_t, Skalierung)
IMPLEMENT_DOSEEX(float32_t, TotalizerAll)
IMPLEMENT_DOSEEX(float32_t, TotalizerLine)
IMPLEMENT_DOSEEX(float32_t, TotalizerMF)
IMPLEMENT_DOSEEX(float32_t, TotalizerSF1)
IMPLEMENT_DOSEEX(float32_t, TotalizerSF2)
IMPLEMENT_DOSEEX(float32_t, LineSetpoint)
IMPLEMENT_DOSEEX(float32_t, LineMassflow)
IMPLEMENT_DOSEEX(float32_t, LineMassflowMF)
IMPLEMENT_DOSEEX(float32_t, LineMassflowSF1)
IMPLEMENT_DOSEEX(float32_t, LineMassflowSF2)
IMPLEMENT_DOSEEX(float32_t, RegeneratPercentage)
IMPLEMENT_DOSEEX(float32_t, RecipeSetpoint)
IMPLEMENT_DOSEEX(float32_t, ManualSetpoint)
IMPLEMENT_DOSEEX(float32_t, ExternalSetpoint)

IMPLEMENT_DOSEEX(uint32_t, ProductionTime)
//IMPLEMENT_DOSEEX(float32_t, DosiererMischung)
IMPLEMENT_DOSEEX(BOOL, ExtLineSetpointState)
IMPLEMENT_DOSEEX(BOOL, UPSLowInputState)					// UPS low
IMPLEMENT_DOSEEX(BOOL, ExtLineInvSetpointState)
IMPLEMENT_DOSEEX(BOOL, IOLineSetpointEnableInput)
IMPLEMENT_DOSEEX(BOOL, IOLineInvSetpointEnableInput)
IMPLEMENT_DOSEEX(float32_t, RampStep)
IMPLEMENT_DOSEEX(float32_t, MaxLeistung)
IMPLEMENT_DOSEEX(float32_t, MinTotband)
IMPLEMENT_DOSEEX(uint32_t,  Filterung)
IMPLEMENT_DOSEEX(uint32_t, RampDelay)
IMPLEMENT_DOSEEX(BOOL, IORampUpInput)
IMPLEMENT_DOSEEX(BOOL, IORampDownInput)
IMPLEMENT_DOSEEX(BOOL, RampUp)
IMPLEMENT_DOSEEX(BOOL, RampDown)
IMPLEMENT_DOSEEX(BOOL, IOSideFeeder1Input)
IMPLEMENT_DOSEEX(BOOL, IOSideFeeder2Input)
IMPLEMENT_DOSEEX(BOOL, IOMainFeederInput)
IMPLEMENT_DOSEEX(BOOL, IOOperateAutoInput)
IMPLEMENT_DOSEEX(BOOL, IOOperateManualInput)
IMPLEMENT_DOSEEX(BOOL, IOUPSLowInput)	// Totalisator impulse
IMPLEMENT_DOSEEX(BOOL, IOOperateAutoOutput)			// IO operate auto output
IMPLEMENT_DOSEEX(BOOL, IOServiceModeOutput)			// IO operate service output


IMPLEMENT_DOSEEX(BOOL, AlarmShutDown)
IMPLEMENT_DOSEEX(BOOL, IOAlarmShutDownInput)		// ShutDown extern
IMPLEMENT_DOSEEX(BOOL, IOWindowsShutDownInput)		// Windows ShutDown extern
IMPLEMENT_DOSEEX(BOOL, IOSyncErrorExtern)			
IMPLEMENT_DOSEEX(BOOL, IOSyncError)					// SyncErrorState
IMPLEMENT_DOSEEX(BOOL, ServiceMode)
IMPLEMENT_DOSEEX(BOOL, IOAlarm0Output)
IMPLEMENT_DOSEEX(BOOL, IOAlarm1Output)
IMPLEMENT_DOSEEX(BOOL, IOAlarm2Output)
IMPLEMENT_DOSEEX(BOOL, IOAlarm3Output)
IMPLEMENT_DOSEEX(BOOL, IOTotalizerPulseOutput);	// Totalisator impulse
IMPLEMENT_DOSEEX(float32_t, IOLineSetpointInput);		// ext. Liniensollwert



IMPLEMENT_DOSEEX(BOOL, Alarm0State)
IMPLEMENT_DOSEEX(BOOL, Alarm1State)
IMPLEMENT_DOSEEX(BOOL, Alarm2State)
IMPLEMENT_DOSEEX(BOOL, Alarm3State)
IMPLEMENT_DOSEEX(BOOL, IOLineReleaseOutput)
IMPLEMENT_DOSEEX(uint32_t, FeederAlarm)
IMPLEMENT_DOSEEX(float32_t, TotalizerPulseStep)						// Schrittweite Totalisatorimpuls [kg]
IMPLEMENT_DOSEEX(uint32_t,  TotalizerPulseDuration)					// Dauer des Impulses [s]


IMPLEMENT_DOSEEX(BOOL, Alarm1QuitState)
IMPLEMENT_DOSEEX(BOOL, Alarm03QuitState)
IMPLEMENT_DOSEEX(BOOL, IOAlarm1QuitInput)
IMPLEMENT_DOSEEX(BOOL, IOAlarm03QuitInput)

//IMPLEMENT_DOSEEX(BOOL, MBLineSetpointExist);
IMPLEMENT_DOSEEX(float32_t,MBLineSetpoint);
IMPLEMENT_DOSEEX(float32_t,MBLinePercentage);
IMPLEMENT_DOSEEX(float32_t, ExtLineSetpoint)
IMPLEMENT_DOSEEX(float32_t, ExtLineOffset)



IMPLEMENT_DOSEEX(BOOL, ModuleExternalSetpoint)
IMPLEMENT_DOSEEX(BOOL, ModuleSetpointRamp)
IMPLEMENT_DOSEEX(BOOL, ModuleLineSetpoint)
IMPLEMENT_DOSEEX(BOOL, ModuleLocalSetpoint)
IMPLEMENT_DOSEEX(BOOL, ModuleBatcher)
IMPLEMENT_DOSEEX(BOOL, ModuleTotalizer)
IMPLEMENT_DOSEEX(BOOL, ModuleTotalizerPulse)

IMPLEMENT_DOSEEX(BOOL, LineReleaseEnable);      // Enable Line Release
IMPLEMENT_DOSEEX(BOOL, LineStartEnable);        // Enable Line Start
IMPLEMENT_DOSEEX(BOOL, LineRelease)
IMPLEMENT_DOSEEX(BOOL, ExtLineSetpointEnable)
IMPLEMENT_DOSEEX(BOOL, ExtLineInvSetpointEnable)


IMPLEMENT_DOSEEXPB(PBAlarm1Quit)
IMPLEMENT_DOSEEXPB(PBAlarm03Quit)
IMPLEMENT_DOSEEXPB(PBRampUp)
IMPLEMENT_DOSEEXPB(PBRampDown)
IMPLEMENT_DOSEEXPB(PBLineStart)
IMPLEMENT_DOSEEXPB(PBLineStop)
IMPLEMENT_DOSEEXPB(PBClearTotalizer)
IMPLEMENT_DOSEEXPB(PBLineShutDown)
IMPLEMENT_DOSEEXPB(PBAlarm0)
IMPLEMENT_DOSEEXPB(PBAlarm1)
IMPLEMENT_DOSEEXPB(PBAlarm2)
IMPLEMENT_DOSEEXPB(PBAlarm3)
IMPLEMENT_DOSEEXPB(PBServiceMode)



//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
int32_t Dose_EXSetLineDoseWaitStart(const int32_t k, const BOOL b)
{
	assert(DOSEPOINTER);
	return DOSEREF.EXSetLineDoseWaitStart(k, b);
}
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
int32_t Dose_EXGetLineDoseWaitStart(const int32_t k, BOOL* b)
{
	assert(DOSEPOINTER);
	return DOSEREF.EXGetLineDoseWaitStart(k, b);
}
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
int32_t Dose_EXDeleteLineDoseWaitStart(void)
{
	assert(DOSEPOINTER);
	return DOSEREF.EXDeleteLineDoseWaitStart();
}
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
int32_t Dose_EXGetAnyLineDoseWaitStart(BOOL* b)
{
	assert(DOSEPOINTER);
	return DOSEREF.EXGetAnyLineDoseWaitStart(b);
}



//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
int32_t Dose_EXSetLineDoseStarted       (const int32_t k, const BOOL b)
{
	assert(DOSEPOINTER);
	return DOSEREF.EXSetLineDoseStarted   (k, b);
}
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
int32_t Dose_EXGetLineDoseStarted(const int32_t k, BOOL* b)
{
	assert(DOSEPOINTER);
	return DOSEREF.EXGetLineDoseStarted(k, b);
}
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
int32_t Dose_EXDeleteLineDoseStarted      (void)
{
	assert(DOSEPOINTER);
	return DOSEREF.EXDeleteLineDoseStarted   ();
}

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
int32_t Dose_EXGetAnyLineDoseStarted    (BOOL *b)
{
	assert(DOSEPOINTER);
	return DOSEREF.EXGetAnyLineDoseStarted (b);
}


//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
int32_t Dose_EXSetDoseStarted      (const int32_t k, const BOOL b)
{
	assert(DOSEPOINTER);
	return DOSEREF.EXSetDoseStarted  (k, b);
}
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
int32_t Dose_EXDeleteDoseStarted     ( void )
{
	assert(DOSEPOINTER);
	return DOSEREF.EXDeleteDoseStarted  ();
}

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
int32_t Dose_EXGetDoseStarted      (const int32_t k, BOOL *b)
{
	assert(DOSEPOINTER);
	return DOSEREF.EXGetDoseStarted  (k, b);
}
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
int32_t Dose_EXGetAnyDoseStarted   (BOOL *b)
{
	assert(DOSEPOINTER);
	return DOSEREF.EXGetAnyDoseStarted(b);
}

