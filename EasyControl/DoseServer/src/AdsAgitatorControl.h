//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Declaration of module AdsAgitatorControl
///
/// @file   AdsAgitatorControl.h
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

#include "AdsDoseButton.h"


class CAdsAgitatorControl : public CAdsWrapperControl
{
	using CBaseClass = CAdsWrapperControl;

	AdsSensor::CAdsDoseAnalogOutput    m_AgitatorOutput;

public:
	CAdsAgitatorControl(const int32_t index);
	void Init() override;
	void Exit() override;
	void Update();
	BOOL SetAgitator(const float32_t value);
};
//*********************************************************************************************
//*********************************************************************************************
inline void CAdsAgitatorControl::Exit()
{
	assert(CBaseClass::IsInit());
	m_AgitatorOutput.Exit();
	CBaseClass::Exit();
}

