//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Declaration of module ModuleTotalizerPulseControl
///
/// @file   ModuleTotalizerPulseControl.h
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
#include "TotalizerPulseThread.h"


class CModuleTotalizerPulseControl : public base::CStatemachine<>
{
	using CBaseClass = base::CStatemachine<>;

	uint32_t m_TriggerCount;
	float32_t m_StartTotalizer;
	float32_t m_LastTotalizer;
	float32_t m_ImpulseStep;
	float32_t m_Totalizer;

	CTotalizerPulseThread m_PulseThread;

private:
	CModuleTotalizerPulseControl() = delete;

	static BOOL IsAnyDoseStarted();

	void TriggerPulse(uint32_t t);

protected:

public:
	CModuleTotalizerPulseControl(int32_t id);
	~CModuleTotalizerPulseControl(void) override = default;
	BOOL Start(const uint32_t t) override;
	BOOL Execute() override;
	BOOL InitExecute() override;
	BOOL ExitExecute() override;
};

