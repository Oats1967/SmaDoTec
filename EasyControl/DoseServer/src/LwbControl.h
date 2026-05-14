//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Declaration of module LwbControl
///
/// @file   LwbControl.h
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

#include "RefillControlGrav.h"
#include "WeightBatchControl.h"
#include "AgitatorControl.h"


class CLwbControl : public CWeightBatchControl
{
	CRefillControlGrav		 m_RefillControl;
	CAgitatorControl		 m_AgitatorControl;

protected:
	void	  StartBatch() override;
	BOOL	  IsSwitchPointReached() const override;
	BOOL	  IsReturnGrossStream() const override;
	void	  CalcSwitchPoint() override;
	float32_t UpdateTotalizer() override;
	BOOL	  IsBatchFinished(void) const override;


public:
	explicit CLwbControl(int32_t s, loadcell::ILCModuleInterface& rlc);
	CLwbControl() = delete;
	~CLwbControl(void) override = default;
};



