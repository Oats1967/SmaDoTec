//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Declaration of module AdsRefillSignalControl
///
/// @file   AdsRefillSignalControl.h
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

#include "AdsMassflowControl.h"
#include "AdsDoseButton.h"
#include "AdsDoseSensor.h"


class CAdsRefillSignalControl : public CAdsMassflowControl
{
	using CBaseClass = CAdsMassflowControl;

	AdsSensor::CAdsDoseDigitalOutput  m_RefillOutput;
	AdsSensor::CAdsDoseAnalogOutput   m_RefillFeeder;


public:
	CAdsRefillSignalControl(const int32_t index);
	void Init() override;
	void Exit() override;

	BOOL SetRefillRequest(const BOOL value);
	BOOL SetRefillFeeder(const float32_t value);
};
//*********************************************************************************************
//*********************************************************************************************
inline BOOL CAdsRefillSignalControl::SetRefillRequest(const BOOL value)
{
	assert(CBaseClass::IsInit());
	return m_RefillOutput.UpdateState(value);
}
//*********************************************************************************************
//*********************************************************************************************
inline BOOL CAdsRefillSignalControl::SetRefillFeeder(const float32_t value)
{
	assert(CBaseClass::IsInit());
	return m_RefillFeeder.UpdateState(value);
}


