//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Declaration of module AdsRampControl
///
/// @file   AdsRampControl.h
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

#include "AdsWrapperControl.h"
#include "AdsLineSensor.h"


class CAdsRampControl : public CAdsWrapperControl
{
	using CBaseClass = CAdsWrapperControl;

	AdsSensor::CAdsLineDigitalInput  m_RampUp;
	AdsSensor::CAdsLineDigitalInput  m_RampDown;


public:
	CAdsRampControl();

	void Init() override;

	BOOL AdsLineGetRampDown(BOOL& value);
	BOOL AdsLineGetRampUp(BOOL& value);
};

//*********************************************************************************************
//*********************************************************************************************
inline BOOL CAdsRampControl::AdsLineGetRampDown(BOOL& value)
{
	assert(CBaseClass::IsInit());
	return m_RampDown.UpdateState(value);
}
//*********************************************************************************************
//*********************************************************************************************
inline BOOL CAdsRampControl::AdsLineGetRampUp(BOOL& value)
{
	assert(CBaseClass::IsInit());
	return m_RampUp.UpdateState(value);
}

