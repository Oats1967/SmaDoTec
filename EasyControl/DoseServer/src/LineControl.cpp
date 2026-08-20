//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Implementation of module LineControl
///
/// @file   LineControl.cpp
///
///
/// @coypright Ing.büro Hafer
///            Branderweg 8A
///            D-91058 Erlangen
///
/// @author    Detlef Hafer
///
//------------------------------------------------------------------------------------
#include <cassert>
#include "LineControl.h"
#include "DoseDataLib/include/DoseData.h"
#include "Logger/include/Logger.h"



enum
{
	eGetStateFunc = 0,
	eSetStateFunc,
	eGetPBFunc,
	eResetPBFunc,
	eQuitStateFunc
};

#define FGETSTATEMETHOD(_a) std::get<eGetStateFunc>(_a)
#define FSETSTATEMETHOD(_a) std::get<eSetStateFunc>(_a)
#define FGETPBMETHOD(_a) std::get<eGetPBFunc>(_a)
#define FRESETPBMETHOD(_a) std::get<eResetPBFunc>(_a)
#define FGETQUITSTATEMETHOD(_a) std::get<eQuitStateFunc>(_a)

const CLineControl::MethodArray CLineControl::c_CheckAlarmMethods{
{
	{ Dose_EXGetAlarm0State, Dose_EXSetAlarm0State, Dose_EXPBGetPBAlarm0, Dose_EXPBResetPBAlarm0, Dose_EXGetAlarm03QuitState},
	{ Dose_EXGetAlarm1State, Dose_EXSetAlarm1State, Dose_EXPBGetPBAlarm1, Dose_EXPBResetPBAlarm1, Dose_EXGetAlarm1QuitState},
	{ Dose_EXGetAlarm2State, Dose_EXSetAlarm2State, Dose_EXPBGetPBAlarm2, Dose_EXPBResetPBAlarm2, Dose_EXGetAlarm03QuitState},
	{ Dose_EXGetAlarm3State, Dose_EXSetAlarm3State, Dose_EXPBGetPBAlarm3, Dose_EXPBResetPBAlarm3, Dose_EXGetAlarm03QuitState},
} };

//----------------------------------------------------------------------------------------
CLineControl::CLineControl(int32_t id, const base::CLineConfig& rConfig) : CBaseClass(id)
, r_Config { rConfig }
, m_ModuleLineSetpointCtrl{ id }
, m_ModuleLocalSetpointCtrl{ id }
, m_ModuleExtSetpointCtrl{ id, rConfig.GetModuleExternalSetpointCfg() }
, m_ModuleRampCtrl{ id, rConfig.GetModuleSetpointRampCfg() }
, m_ModuleBatcherCtrl{ id }
, m_ModuleTotalizerCtrl{ id }
, m_ModuleTotalizerPulseCtrl{ id }
, m_ModuleList{ }
, m_AdsClient{}
, m_bIOStartInputLast{ FALSE }
, m_bLineDoseStarted{ FALSE }
, m_tUPSLowStart{ 0L }
, m_WatchDogTime{ 0L }
, m_bIOAlarm1QuitInputLast{ FALSE }
, m_bIOAlarm03QuitInputLast{ FALSE }
, m_bIOAlarmShutDownLast{ FALSE }
{
	BOOL bSetpointNeeded  = FALSE;
	{
		auto bEnable = r_Config.GetModuleExternalSetpointCfg().GetEnable();
		Dose_EXSetModuleExternalSetpoint(bEnable);
		if (bEnable)
		{
			bSetpointNeeded = TRUE;
			m_ModuleList.push_back(&m_ModuleExtSetpointCtrl);
		}
	}

	{
		auto bEnable = r_Config.GetModuleSetpointRampCfg().GetEnable();
		Dose_EXSetModuleSetpointRamp(bEnable);
		if (bEnable)
		{
			bSetpointNeeded = TRUE;
			m_ModuleList.push_back(&m_ModuleRampCtrl);
		}
	}
	{
		BOOL bLineEnable = r_Config.GetModuleLineSetpointCfg().GetEnable();
		if ( ( !bLineEnable ) && bSetpointNeeded)
		{
			assert(FALSE);
			LOGERROR("ERROR in configuration : ModuleLineSetpoint added !");
		}
		Dose_EXSetModuleLineSetpoint(bLineEnable || bSetpointNeeded);
		Dose_EXSetLineReleaseEnable(bLineEnable && r_Config.GetModuleLineSetpointCfg().GetLineRelease());      // Enable Line Release
		Dose_EXSetLineStartEnable(bLineEnable && r_Config.GetModuleLineSetpointCfg().GetLineStart());      // Enable Line Release
		if (bLineEnable || bSetpointNeeded)
		{
			m_ModuleList.push_back(&m_ModuleLineSetpointCtrl);
		}
	}
	{
		BOOL bLineEnable = FALSE;
		Dose_EXGetModuleLineSetpoint(&bLineEnable);
		BOOL bEnable = r_Config.GetModuleLocalSetpointCfg().GetEnable();
		Dose_EXSetModuleLocalSetpoint((!bLineEnable) && bEnable);
		if (bEnable)
		{
			if (bLineEnable)
			{
				assert(FALSE);
				LOGERROR("ERROR in configuration : ModuleLocalSetpoint and ModuleLineSetpoint enabled !!")
			}
			else
			{
				m_ModuleList.push_back(&m_ModuleLocalSetpointCtrl);
			}
		}
	}
	{
		auto bEnable = r_Config.GetModuleBatcherCfg().GetEnable();
		Dose_EXSetModuleBatcher(bEnable);
		if (bEnable)
		{
			m_ModuleList.push_back(&m_ModuleBatcherCtrl);
		}
	}
	BOOL bTotalizerModule = FALSE;
	{
		auto bEnable = r_Config.GetModuleTotalizerCfg().GetEnable();
		Dose_EXSetModuleTotalizer(bEnable);
		if (bEnable)
		{
			m_ModuleList.push_back(&m_ModuleTotalizerCtrl);
			bTotalizerModule = TRUE;
		}
	}
	{
		auto bEnable = r_Config.GetModuleTotalizerPulseCfg().GetEnable();
		if (bEnable && ( ! bTotalizerModule) )
		{
			assert(FALSE);
			LOGERROR("ERROR in configuration : ModuleLineTotalizer must be added !");
		}
		Dose_EXSetModuleTotalizerPulse(bEnable);
		if (bEnable)
		{
			m_ModuleList.push_back(&m_ModuleTotalizerPulseCtrl);
		}
	}
}

//*****************************************************************************************
//*****************************************************************************************
void CLineControl::SetIDAll(int32_t id)
{
	for (auto& rItem : m_ModuleList)
	{
		rItem->SetID(id);
	}
}
//*****************************************************************************************
//*****************************************************************************************
void CLineControl::InitExecuteAll()
{
	for (auto& rItem : m_ModuleList)
	{
		rItem->InitExecute();
	}
}
//*****************************************************************************************
//*****************************************************************************************
inline void CLineControl::ExitExecuteAll()
{
	for (auto& rItem : m_ModuleList)
	{
		rItem->ExitExecute();
	}
}
//*****************************************************************************************
//*****************************************************************************************
void CLineControl::ExecuteAll()
{
	for (auto& rItem : m_ModuleList)
	{
		rItem->Execute();
	}
}
//*****************************************************************************************
//*****************************************************************************************
void CLineControl::StartAll(uint32_t t)
{
	for (auto& rItem : m_ModuleList)
	{
		rItem->Start(t);
	}
}
//*****************************************************************************************
//*****************************************************************************************
void CLineControl::StopAll()
{
	for (auto& rItem : m_ModuleList)
	{
		rItem->Stop();
	}
}
//*********************************************************************************************************************
//*********************************************************************************************************************
void CLineControl::CheckAlarm1Quit(void)
{
	BOOL bAlarm1Quit = FALSE;

	BOOL bIOAlarm1QuitInput = FALSE;
	Dose_EXGetIOAlarm1QuitInput(&bIOAlarm1QuitInput);
	//m_AdsClient.AdsLineGetAlarmQuit1(bIOAlarm1QuitInput);

	if (bIOAlarm1QuitInput || (bIOAlarm1QuitInput != m_bIOAlarm1QuitInputLast))
	{
		m_bIOAlarm1QuitInputLast = bIOAlarm1QuitInput;
		bAlarm1Quit = bIOAlarm1QuitInput;
	}
	else
	{
		BOOL bChanged = FALSE;
		Dose_EXPBGetPBAlarm1Quit(&bChanged);
		if (bChanged)
		{
			bAlarm1Quit = TRUE;
		}
	}
	Dose_EXPBResetPBAlarm1Quit();
	Dose_EXSetAlarm1QuitState(bAlarm1Quit);
}
//*********************************************************************************************************************
//*********************************************************************************************************************
void CLineControl::CheckAlarm03Quit(void)
{
	BOOL bAlarm03Quit = FALSE;

	BOOL bIOAlarm03QuitInput = FALSE;
	Dose_EXGetIOAlarm03QuitInput(&bIOAlarm03QuitInput);
	//m_AdsClient.AdsLineGetAlarmQuit03(bIOAlarm03QuitInput);

	if (bIOAlarm03QuitInput || (bIOAlarm03QuitInput != m_bIOAlarm03QuitInputLast))
	{
		m_bIOAlarm03QuitInputLast = bAlarm03Quit;
		bAlarm03Quit = bIOAlarm03QuitInput;
	}
	else
	{
		BOOL bChanged = FALSE;
		Dose_EXPBGetPBAlarm03Quit(&bChanged);
		if (bChanged)
		{
			bAlarm03Quit = TRUE;
		}
	}
	Dose_EXPBResetPBAlarm03Quit();
	Dose_EXSetAlarm03QuitState(bAlarm03Quit);
}
//*********************************************************************************************************************
//*********************************************************************************************************************
void CLineControl::CheckAlarmState(void)
{
	for (const auto& rItem : c_CheckAlarmMethods)
	{
		BOOL bChanged = FALSE;
		FGETPBMETHOD(rItem) (&bChanged);  // Dose_EXPBGetPBAlarm0
		if (bChanged)
		{
			FRESETPBMETHOD(rItem)();	  // Dose_EXPBResetPBAlarm0
			FSETSTATEMETHOD(rItem)(TRUE); // Dose_EXSetAlarm0State
		}
	}
}
//*********************************************************************************************************************
//*********************************************************************************************************************
void CLineControl::CheckAlarm(void)
{
	uint32_t ulFeeder = 0;
	Dose_EXGetFeederAlarm(&ulFeeder);
	const auto bFeederAlarm = BOOL(ulFeeder != 0);

	int32_t index = 0;
	for (const auto& rItem : c_CheckAlarmMethods)
	{
		BOOL bAlarmState = FALSE;
		if (bFeederAlarm)
		{
			FGETSTATEMETHOD(rItem)(&bAlarmState);  // Dose_EXGetAlarm0State
			if (bAlarmState)
			{
				BOOL bAlarmQuit = FALSE;
				FGETQUITSTATEMETHOD(rItem)(&bAlarmQuit); // Dose_EXGetAlarm03QuitState
				if (bAlarmQuit)
				{
					bAlarmState = FALSE;
				}
			}
		}
		FSETSTATEMETHOD(rItem)(bAlarmState); // Dose_EXSetAlarm0State
		m_AdsClient.LineSetAlarm(index, bAlarmState);
		index++;
	}
	// Shutdown abprüfen
	{
		BOOL bAlarmQuit = FALSE;
		Dose_EXGetAlarm03QuitState(&bAlarmQuit);
		if (bAlarmQuit)
		{
			Dose_EXSetAlarmShutDown(FALSE);
		}
	}
}
//*********************************************************************************************************************
//*********************************************************************************************************************
void CLineControl::CheckAllAlarms(void)
{
	CheckAlarmState();

	CheckAlarm1Quit();
	CheckAlarm03Quit();
	CheckAlarm();
	GetClearAlarm();
}

//*********************************************************************************************************************
//*********************************************************************************************************************
void CLineControl::GetLineRelease(void)
{
	BOOL bEnable = FALSE;
	Dose_EXGetLineReleaseEnable(&bEnable);
	if (bEnable)
	{
		BOOL bStart = FALSE;
		Dose_EXGetLineRelease(&bStart);
		m_AdsClient.LineSetRelease(bStart);
	}
}
//*********************************************************************************************************************
//*********************************************************************************************************************
BOOL CLineControl :: GetAlarmShutdown ( void)
{
	BOOL bAlarmShutDown = FALSE;

	Dose_EXGetAlarmShutDown(&bAlarmShutDown);
	BOOL bAlarmShutDownExt = FALSE;
	Dose_EXGetIOAlarmShutDownInput(&bAlarmShutDownExt);
	if (bAlarmShutDownExt || (m_bIOAlarmShutDownLast != bAlarmShutDownExt))
	{
		m_bIOAlarmShutDownLast = bAlarmShutDownExt;
		bAlarmShutDown = bAlarmShutDownExt;
	}
	else
	{
		BOOL bChanged = FALSE;
		Dose_EXPBGetPBLineShutDown(&bChanged);
		if (bChanged)
		{
			bAlarmShutDown = TRUE;
		}
	}
	Dose_EXPBResetPBLineShutDown();
	Dose_EXSetAlarmShutDown(bAlarmShutDown);
	return bAlarmShutDown;
}
//*********************************************************************************************************************
//*********************************************************************************************************************
inline BOOL CLineControl::GetUPSAlarm(void)
{
#if _DEBUG
	const uint32_t c_WaitTime = 10; //  60U * 30U;
	BOOL bLineStarted = TRUE;
#else
	const uint32_t c_WaitTime = 60U * 30U;
#endif
	BOOL bUPSLow = FALSE;
	Dose_EXGetUPSLowInputState(&bUPSLow);

	if (m_st >= m_tUPSLowStart + c_WaitTime)
	{
		BOOL bLineStarted = FALSE;
		Dose_EXGetAnyDoseStarted(&bLineStarted);

		if ( ! bLineStarted)
		{
			Dose_EXGetIOUPSLowInput(&bUPSLow);
			Dose_EXSetUPSLowInputState(bUPSLow);
			m_tUPSLowStart = m_st;
		}
	}
	return bUPSLow;
}

//*********************************************************************************************************************
//*********************************************************************************************************************
inline BOOL CLineControl :: GetExtAlarm ( void)
{
	BOOL bAlarm = FALSE;

	Dose_EXGetIOExtAlarmInput(&bAlarm);
	//m_AdsClient.AdsLineGetExtAlarm(bAlarm);
	return bAlarm;
}
//*********************************************************************************************************************
//*********************************************************************************************************************
inline BOOL CLineControl :: GetCustomAlarm ( void)
{
	BOOL bAlarm = FALSE;

	Dose_EXGetIOCustomerAlarmInput(&bAlarm);
	//m_AdsClient.AdsLineGetCustomAlarm(bAlarm);
	return bAlarm;
}
//*********************************************************************************************************************
//*********************************************************************************************************************
inline BOOL CLineControl::GetWindowsShutDown(void)
{
	BOOL bShutDown = FALSE;

	Dose_EXGetIOWindowsShutDownInput(&bShutDown);
	//m_AdsClient.AdsLineGetWindowsShutDown(bShutDown);
	return bShutDown;
}
//*********************************************************************************************************************
//*********************************************************************************************************************
inline BOOL CLineControl :: GetSyncError ( void)
{
	BOOL bSyncError = FALSE;
	m_AdsClient.AdsGetSyncError(bSyncError);
	return bSyncError;
}
//*********************************************************************************************************************
//*********************************************************************************************************************
void CLineControl::GetStart(void)
{
	BOOL bEnable = FALSE;
	Dose_EXGetLineStartEnable(&bEnable);
	if (bEnable)
	{
		BOOL bStartOld = FALSE;
		Dose_EXGetLineStart(&bStartOld);

		BOOL bStart = bStartOld;
		BOOL bIOStartInput = FALSE;
		Dose_EXGetIOLineStartInput(&bIOStartInput);
		if (bIOStartInput || (bIOStartInput != m_bIOStartInputLast))
		{
			m_bIOStartInputLast = bIOStartInput;
			bStart = bIOStartInput;
			bStartOld = !bStart;
		}
		else
		{
			BOOL bMBLineStart = FALSE;
			Dose_EXPBGetPBLineStart(&bMBLineStart);
			if (bMBLineStart)
			{
				bStart		= TRUE;
				bStartOld   = FALSE;
			}
			BOOL bMBLineStop = FALSE;
			Dose_EXPBGetPBLineStop(&bMBLineStop);
			if (bMBLineStop)
			{
				bStart    = FALSE;
				bStartOld = TRUE;
			}
		}
		Dose_EXPBResetPBLineStart();
		Dose_EXPBResetPBLineStop();
		if (bStart != bStartOld)
		{
			Dose_EXSetLineStart(bStart);
	#if _DEBUG
			BOOL bChanged = FALSE;
			Dose_EXGetChangedLineStart(&bChanged);
			//assert(bChanged);
	#endif
		}
	}
}
//*********************************************************************************************************************
//*********************************************************************************************************************
BOOL CLineControl :: InitExecute ( void)
{
	auto result = CBaseClass::InitExecute();
	if (result)
	{
		m_AdsClient.Init();

		InitExecuteAll();
		Dose_EXDeleteDoseStarted();
		Dose_EXDeleteLineDoseStarted();
		Dose_EXDeleteLineDoseWaitStart();
		Dose_EXInitLineStarted(FALSE);
		Dose_EXInitLineStart(FALSE);
		Dose_EXInitAlarmShutDown(FALSE);
		Dose_EXInitAlarm0State(FALSE);
		Dose_EXInitAlarm1State(FALSE);
		Dose_EXInitAlarm2State(FALSE);
		Dose_EXInitAlarm3State(FALSE);
		Dose_EXInitAlarm03QuitState(FALSE);

		Dose_EXPBResetPBLineStart();
		Dose_EXPBResetPBLineStop();
		Dose_EXPBResetPBClearTotalizer();
		Dose_EXPBResetPBLineShutDown();
		Dose_EXPBResetPBAlarm03Quit();
		Dose_EXPBResetPBAlarm1Quit();

		Dose_EXPBResetPBAlarm0();
		Dose_EXPBResetPBAlarm1();
		Dose_EXPBResetPBAlarm2();
		Dose_EXPBResetPBAlarm3();

		Dose_EXInitUPSLowInputState(FALSE);

		m_bLineDoseStarted = FALSE;

		Dose_EXGetIOLineStartInput(&m_bIOStartInputLast);
		Dose_EXGetIOAlarm1QuitInput(&m_bIOAlarm1QuitInputLast);
		Dose_EXGetIOAlarm03QuitInput(&m_bIOAlarm03QuitInputLast);
		Dose_EXGetIOAlarmShutDownInput(&m_bIOAlarmShutDownLast);

		assert(! m_bIOStartInputLast);
		assert(! m_bIOAlarm1QuitInputLast);
		assert(! m_bIOAlarm03QuitInputLast);
		assert(! m_bIOAlarmShutDownLast);

	}
	return result;
}
//*********************************************************************************************************************
//*********************************************************************************************************************
BOOL CLineControl :: ExitExecute ( void)
{
	auto result = CBaseClass::IsInit();
	if (result)
	{
		Stop();
		ExitExecuteAll();
		m_AdsClient.Exit();
		CBaseClass::ExitExecute();
	}
	return result;
}

//*********************************************************************************************************************
//*********************************************************************************************************************
BOOL CLineControl :: Start ( const uint32_t t)
{
	auto result = CBaseClass::Start(t);
	if (result)
	{
		StartAll(m_st);
		m_bLineDoseStarted = FALSE;
		Dose_EXSetLineStarted(TRUE);
	}
	return result;
}
//*********************************************************************************************************************
//*********************************************************************************************************************
BOOL CLineControl :: Stop ( void)
{
	BOOL result = FALSE;

	if ( IsStarted() || m_bLineDoseStarted)
	{
		m_AdsClient.LineSetRelease(FALSE);
		Dose_EXInitLineStarted(FALSE);
		Dose_EXInitLineStart(FALSE);
		StopAll();
		m_bLineDoseStarted = FALSE;
		result = CBaseClass::Stop();
	}
	return result;
}
//*********************************************************************************************************************
//*********************************************************************************************************************
void CLineControl::CheckStop(void)
{
	BOOL	bLineStarted		= FALSE;
	BOOL	bLineDoseWaitStart = FALSE;

	Dose_EXGetAnyLineDoseStarted(&bLineStarted);
	Dose_EXGetAnyLineDoseWaitStart(&bLineDoseWaitStart);
	bLineStarted |= bLineDoseWaitStart;

	if ( ! bLineStarted)
	{
		if (m_bLineDoseStarted)
		{
			Stop();
		}
	}
	else
	{
		m_bLineDoseStarted = TRUE;
	}
}

//*********************************************************************************************************************
//*********************************************************************************************************************
BOOL CLineControl::GetClearAlarm(void)
{
	BOOL bAlarm03Quit = FALSE;

	Dose_EXGetAlarm03QuitState(&bAlarm03Quit);
	if (bAlarm03Quit)
	{
		Dose_EXInitIOExtAlarmInput(FALSE);
		Dose_EXInitIOCustomerAlarmInput(FALSE);
		Dose_EXInitIOAlarmShutDownInput(FALSE);
		Dose_EXInitIOLineReleaseOutput(FALSE);
		Dose_EXInitIOWindowsShutDownInput(FALSE);
		Dose_EXInitIOUPSLowInput(FALSE);
		Dose_EXInitUPSLowInputState(FALSE);
		//Dose_EXInitAlarmShutDown(FALSE);
		m_tUPSLowStart = m_st;
	}
	return bAlarm03Quit;
}


//*********************************************************************************************************************
//*********************************************************************************************************************
BOOL CLineControl :: Control ()
{
	BOOL bAlarmShutDown = GetAlarmShutdown();
	if ( bAlarmShutDown )
	{
		Stop ();
		return TRUE;
	}
	BOOL bWindowsShutDown = GetWindowsShutDown();
	if (bWindowsShutDown)
	{
		Stop();
		return TRUE;
	}
	BOOL bSyncError = GetSyncError();
	if ( bSyncError )
	{
		Stop ();
		return TRUE;
	}
	GetLineRelease();

	GetCustomAlarm();
	// Vorlagenalarm
	GetExtAlarm();

	GetUPSAlarm();

	GetStart();


	BOOL bChanged = FALSE;
	Dose_EXGetChangedLineStart(&bChanged);
	if (bChanged)
	{
		BOOL bStart = FALSE;
		Dose_EXPopLineStart(&bStart);
		if (bStart)
		{
			Start();
		}
		else
		{	
			assert(IsStarted());
			Stop();
		}
	}
	// Freigabe
	CheckStop();
	return TRUE;
}
//*********************************************************************************************************************
//*********************************************************************************************************************
void CLineControl::WatchDog()
{
	if (m_st != m_WatchDogTime)
	{
		m_WatchDogTime = m_st;
		BOOL bValue = FALSE;
		Dose_EXGetIOWatchDogOutput(&bValue);
		m_AdsClient.LineSetWatchDog(!bValue);
	}
}
//*********************************************************************************************************************************
//*********************************************************************************************************************************
BOOL CLineControl :: Execute ()
{
	auto result = CBaseClass::Execute();
	if (result)
	{
		m_AdsClient.UpdateState();

		BOOL bValue = FALSE;
		Dose_EXGetIOOperateAutoInput(&bValue);
		m_AdsClient.LineSetOperateAuto(bValue);

		BOOL bServiceMode = FALSE;
		Dose_EXGetServiceMode(&bServiceMode);
		m_AdsClient.LineSetOperateService(bServiceMode);

		ExecuteAll();
		CheckAllAlarms();
		Control();
		WatchDog();
	}
	return result;
}

