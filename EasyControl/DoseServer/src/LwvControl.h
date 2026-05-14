//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Declaration of module LwvControl.h
///
/// @file   LwvControl.h
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

#include "LwfControl.h"


class CLwvControl : public CLwfControl
{
protected:
	float32_t CalcDriveCommand(const float32_t fNomwert) const override;

public:
	CLwvControl(int32_t s, loadcell::ILCModuleInterface& rlc) : CLwfControl(s, rlc)
	{}
	~CLwvControl(void) override = default;
};


//*********************************************************************************************
//*********************************************************************************************
float32_t CLwvControl::CalcDriveCommand(const float32_t fNomwert) const
{
	return m_MeterCtrl.CalcDriveCommand(fNomwert);
}


