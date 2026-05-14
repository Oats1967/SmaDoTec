//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Declaration of module GwbControl
///
/// @file   GwbControl.h
///
///
/// @coypright Ing.b�ro Hafer
///            Branderweg 8A
///            D-91058 Erlangen
///
/// @author    Detlef Hafer
///
//------------------------------------------------------------------------------------
#pragma once

#include "WeightBatchControl.h"



class CGwbControl : public CWeightBatchControl
{
protected:
	CWeightControl	m_ExtWeightCtrl;

protected:
	void GetExtWeight(void);
	void ScaleInit( void );

protected:
	void	  StartBatch() override;
	BOOL	  IsSwitchPointReached() const override;
	BOOL	  IsReturnGrossStream() const override;
	BOOL	  IsBatchFinished(void) const override;
	void	  CalcSwitchPoint() override;
	float32_t UpdateTotalizer() override;
	void	  GetWeight(void) override;

public:
	explicit CGwbControl(int32_t s, loadcell::ILCModuleInterface& rlc) : CWeightBatchControl(s, rlc)
		, m_ExtWeightCtrl{ s}
	{
		m_ExtWeightCtrl.registerLoadcell(rlc);
		m_ExtWeightCtrl.registerAlarmManager(this->m_aAlarm);
	}

	CGwbControl() = delete;
	~CGwbControl(void) override = default;

	BOOL InitExecute(void) override;
	BOOL ExitExecute(void) override;
	BOOL Execute(void) override;

	BOOL Start(const uint32_t t) override;
	BOOL Stop() override;
};



