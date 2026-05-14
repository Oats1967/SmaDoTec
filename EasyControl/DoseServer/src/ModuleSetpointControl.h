//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Declaration of module ModuleSetpointControl
///
/// @file   ModuleSetpointControl.h
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
#include "AdsSetpointControl.h"


class CModuleSetpointControl : public base::CStatemachine<>
{
	using CBaseClass = base::CStatemachine<>;

private:
	CAdsSetpointControl m_AdsClient;

private:
	static void CalcLineInfeedMassflow(void);
	static void Init();

public:
	CModuleSetpointControl(int32_t id);
	~CModuleSetpointControl(void) override = default;

	BOOL Execute			( void ) override;
	BOOL InitExecute(void) override;
	BOOL ExitExecute() override;

};

