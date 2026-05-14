//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Declaration of module ModuleTotalizerControl
///
/// @file   ModuleTotalizerControl.h
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


class CModuleTotalizerControl : public base::CTimerStatemachine<>
{
	using CBaseClass = base::CTimerStatemachine<>;

	uint32_t			m_tProducedTime;
	uint32_t			m_tStartTime;
	uint32_t			m_tNext;

private:
	CModuleTotalizerControl() = delete;
	void SetNewProductionTime();
	void CalcTotalizer(void);
	void ClearTotalizer(void);
	BOOL GetLineClearTotalizer(void);

	static BOOL IsValidDose(const int32_t index);
	static BOOL IsLineDose(const int32_t index);
	static BOOL IsAnyDoseStarted();

protected:

public:
	CModuleTotalizerControl(int32_t id);
	~CModuleTotalizerControl(void) override = default;
	BOOL Start(const uint32_t t) override;
	BOOL Stop() override;
	BOOL Execute() override;
};

