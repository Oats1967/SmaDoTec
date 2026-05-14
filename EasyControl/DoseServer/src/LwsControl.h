//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Declaration of module LwsControl.h
///
/// @file   LwsControl.h
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


class CLwsControl : public CLwfControl
{
protected:
	float32_t CalcDriveCommand(const float32_t fNomwert) const override;

public:
	CLwsControl(int32_t s, loadcell::ILCModuleInterface& rlc) : CLwfControl(s, rlc)
	{}
	~CLwsControl(void) override = default;
};


//*********************************************************************************************
//*********************************************************************************************
float32_t CLwsControl::CalcDriveCommand(const float32_t fNomwert) const
{
	return _F32(FDIV0(fNomwert, m_dActualDoseperformance));
}


