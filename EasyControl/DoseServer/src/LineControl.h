//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Declaration of module LineControl
///
/// @file   LineControl.h
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

#include <tuple>
#include <vector>
#include <array>
#include "BASE/include/LineConfig.h"
#include "BASE/include/Statemachine.h"
#include "ModuleLineSetpointControl.h"
#include "ModuleLocalSetpointControl.h"
#include "ModuleExtSetpointControl.h"
#include "ModuleRampControl.h"
#include "ModuleBatcherControl.h"
#include "ModuleTotalizerControl.h"
#include "ModuleTotalizerPulseControl.h"
#include "AdsLineControl.h"



class CLineControl : public base::CTimerStatemachine<>
{
	using CBaseClass = base::CTimerStatemachine<>;

	typedef int32_t(*GetStateFunc) (BOOL*);
	typedef int32_t(*SetStateFunc) (const BOOL);
	typedef int32_t(*GetPBFunc) (BOOL*);
	typedef int32_t(*GetQuitStateFunc) (BOOL*);
	typedef int32_t(*ResetPBFunc) ();

	static const int32_t cAlarmSize = 4;
	using MethodArray = std::array<std::tuple<GetStateFunc, SetStateFunc, GetPBFunc, ResetPBFunc, GetQuitStateFunc>, cAlarmSize>;
	static const MethodArray c_CheckAlarmMethods;

public:
	using TModuleList = std::vector< base::CStatemachine<>*>;
private:
	const base::CLineConfig&		r_Config;
	CModuleLineSetpointControl		m_ModuleLineSetpointCtrl;
	CModuleLocalSetpointControl		m_ModuleLocalSetpointCtrl;
	CModuleExtSetpointControl		m_ModuleExtSetpointCtrl;
	CModuleRampControl				m_ModuleRampCtrl;
	CModuleBatcherControl			m_ModuleBatcherCtrl;
	CModuleTotalizerControl			m_ModuleTotalizerCtrl;
	CModuleTotalizerPulseControl	m_ModuleTotalizerPulseCtrl;
	CAdsLineControl					m_AdsClient;

private:
	TModuleList			m_ModuleList;
	BOOL				m_bLineDoseStarted;
	BOOL				m_bIOAlarm1QuitInputLast;
	BOOL				m_bIOAlarm03QuitInputLast;
	BOOL				m_bIOStartInputLast;
	BOOL				m_bIOAlarmShutDownLast;
	uint32_t			m_tUPSLowStart;

private:
	BOOL GetAlarmShutdown	( void );
	BOOL GetSyncError( void);
	BOOL GetCustomAlarm(void);
	BOOL GetWindowsShutDown(void);
	BOOL GetExtAlarm(void);
	BOOL GetUPSAlarm(void);
	void CheckAlarmState(void);

	void CheckAlarm03Quit(void);
	void CheckAlarm1Quit(void);
	void CheckAlarm(void);
	void CheckAllAlarms(void);
	void GetLineRelease(void);


private:
	BOOL Control(void);
	void CheckStop(void);
	BOOL GetClearAlarm(void);


	void GetStart(void);

private:
	void SetIDAll(int32_t id);
	void InitExecuteAll();
	void ExitExecuteAll();
	void ExecuteAll();
	void StartAll(uint32_t t);
	void StopAll();

public:
	CLineControl(int32_t id, const base ::CLineConfig& rConfig);
	~CLineControl(void) = default;

	void SetID( int32_t s)
	{
		CBaseClass:: SetID(s);
		SetIDAll(s); 
	}

	BOOL InitExecute ( void) override;
	BOOL ExitExecute ( void) override;
	BOOL Execute	 ( void) override;
	BOOL Start		 ( uint32_t t = 0) override;
	BOOL Stop		 ( void ) override;
};


