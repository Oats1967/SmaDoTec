//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Declaration of module AdsEmptyFeederTaskControl
///
/// @file   AdsEmptyFeederTaskControl.h
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
#include "AdsDoseButton.h"


class CAdsEmptyFeederTaskControl : public CAdsWrapperControl
{
	using CBaseClass = CAdsWrapperControl;

	AdsSensor::CAdsDoseDigitalOutput  m_SetEmptyFeeder;

public:
	CAdsEmptyFeederTaskControl(const int32_t index);
	void Init() override;
	void Exit() override;

	BOOL SetEmptyFeeder(const BOOL);
};
//*********************************************************************************************
//*********************************************************************************************
inline BOOL CAdsEmptyFeederTaskControl::SetEmptyFeeder(const BOOL bAlarm)
{
	assert(CBaseClass::IsInit());
	return m_SetEmptyFeeder.UpdateState(bAlarm);
}

