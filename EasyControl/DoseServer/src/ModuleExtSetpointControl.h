//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Declaration of module ModuleExtSetpointControl
///
/// @file   ModuleExtSetpointControl.h
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

#include "BASE/include/Statemachine.h"
#include "BASE/include/ModuleExternalSetpointConfig.h"
#include "AdsExtSetpointControl.h"


class CModuleExtSetpointControl : public base::CStatemachine<>
{
	using CBaseClass = base::CStatemachine<>;

private:
	const base::CModuleExternalSetpointConfig& c_Config;
	CAdsExtSetpointControl m_AdsClient;
	float32_t   m_fSetpointMean;
	BOOL		m_bIOLineInvSetpointEnableInputLast;
	BOOL		m_bIOLineSetpointEnableInputLast;

private:
	BOOL CheckExtLineSetpointState	( void );
	BOOL CheckExtLineInvSetpointState(void);
	static void Init ( void);
	static float32_t GetExtLineSetpoint();

public:
	CModuleExtSetpointControl(int32_t id, const base::CModuleExternalSetpointConfig& cfg);
	~CModuleExtSetpointControl(void) override = default;

	BOOL Execute			( void ) override;
	BOOL InitExecute(void) override;
	BOOL ExitExecute(void) override;
};

