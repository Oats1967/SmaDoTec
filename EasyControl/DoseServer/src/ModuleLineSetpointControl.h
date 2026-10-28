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
#include "BASE/Utils/public/FloatRingBuffer.h"



class CModuleLineSetpointControl : public CModuleSetpointControl
{
	using CBaseClass = CModuleSetpointControl;

private:
	static const uint32_t c_CheckHystereseCounter = 20u;

	base::utils::CFloatRingBuffer < c_CheckHystereseCounter / 4U> m_SetpointHistory;
	uint32_t m_SetpointHistoryCounter;

private:
	BOOL CalcLineSetpoint(float32_t fOldSetpoint, float32_t *fNewSetpoint);
	BOOL SetLineSetpoint(float32_t fNewLineSetpoint);
	void Init();
	void ResetHistoryBuffer();
	void Control(void);

private:
	static BOOL IsValidDose(const int32_t index);
	static void CalcMassflow		(void);

	CModuleLineSetpointControl() = delete;

public:
	CModuleLineSetpointControl(int32_t id);
	~CModuleLineSetpointControl(void) override = default;
	BOOL Execute			( void ) override;
};

