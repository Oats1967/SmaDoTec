//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Declaration of module AdsSetpointControl
///
/// @file   AdsSetpointControl.h
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
#include "AdsLineButton.h"

class CAdsSetpointControl : public CAdsWrapperControl
{
	using CBaseClass = CAdsWrapperControl;

	AdsSensor::CAdsLineAnalogOutput m_Massflow;

public:
	CAdsSetpointControl();

	void Init() override;
	void Exit() override;

	BOOL AdsLineSetMassflow();
};
