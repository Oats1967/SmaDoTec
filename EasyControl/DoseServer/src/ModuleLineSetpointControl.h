//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Declaration of module ModuleLineSetpointControl
///
/// @file   ModuleLineSetpointControl.h
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


class CModuleLineSetpointControl : public CModuleSetpointControl
{
	using CBaseClass = CModuleSetpointControl;

private:
	static BOOL IsValidDose(const int32_t index);
	static BOOL SetLineSetpoint	( float32_t fNewLineSetpoint);
	static void CalcMassflow		(void);
	static void Control			(void);
	static void Init();

	CModuleLineSetpointControl() = delete;

public:
	CModuleLineSetpointControl(int32_t id);
	~CModuleLineSetpointControl(void) override = default;
	BOOL Execute			( void ) override;
};

