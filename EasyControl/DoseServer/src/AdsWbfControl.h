//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Declaration of module AdsWbfControl
///
/// @file   AdsWbfControl.h
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

#include <cassert>
#include "AdsMassflowControl.h"
#include "AdsDoseButton.h"


class CAdsWbfControl : public CAdsMassflowControl
{
	using CBaseClass = CAdsMassflowControl;

	AdsSensor::CAdsDoseDigitalOutput	m_SetBeltLoadLimit;

public:
	CAdsWbfControl(const int32_t index);

	void Init() override;
	void Exit() override;
	BOOL SetBeltLoadLimit(const BOOL value);
};
//*********************************************************************************************
//*********************************************************************************************
inline BOOL CAdsWbfControl::SetBeltLoadLimit(const BOOL value)
{
	assert(CBaseClass::IsInit());
	return m_SetBeltLoadLimit.UpdateState(value);
}




