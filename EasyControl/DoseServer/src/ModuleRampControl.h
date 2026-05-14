//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Declaration of module ModuleRampControl.h
///
/// @file   ModuleRampControl.h
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
#include "BASE/include/ModuleSetpointRampConfig.h"
#include "AdsRampControl.h"



class CModuleRampControl : public base::CTimerStatemachine<>
{
	using CBaseClass = base::CTimerStatemachine<>;

private:
	CAdsRampControl m_AdsClient;
	const base::CModuleSetpointRampConfig& c_Config;
	int32_t		m_sStepRampUp;
    int32_t		m_sStepRampDown;
    float32_t   m_fSetpointMean;
    uint32_t	m_t0;

private:
	BOOL CheckRampUp		( void );
	BOOL CheckRampDown		( void );

	float32_t GetLineSetpoint(void) const;

	static void Init(void);

public:
	CModuleRampControl(int32_t id, const base::CModuleSetpointRampConfig& cfg);
	~CModuleRampControl(void) override = default;
	BOOL Execute			( void ) override;
	BOOL InitExecute(void) override;
};

