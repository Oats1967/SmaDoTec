//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Declaration of module ModuleLocalSetpointControl
///
/// @file   ModuleLocalSetpointControl.h
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

#include "ModuleSetpointControl.h"


class CModuleLocalSetpointControl : public CModuleSetpointControl
{
	using CBaseClass = CModuleSetpointControl;

private:
	void CalcMassflow(void);
	void CalcSetpoint(void);

	static BOOL IsValidDose(const int32_t index);
	static BOOL IsValidDoseStarted(const int32_t index);
	static void Init();


	CModuleLocalSetpointControl() = delete;


public:
	CModuleLocalSetpointControl(int32_t id);
	~CModuleLocalSetpointControl(void) override = default;
	BOOL Execute			( void ) override;
};

