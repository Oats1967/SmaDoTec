//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Declaration of module IfoControl
///         Overfeeding control
///
/// @file   IfoControl.h
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

#include "IfuControl.h"


class CIfoControl : public CIfuControl
{
	using CBaseClass = CIfuControl;

	/*
				|    -> StepHighReached
		High	+
				|	 -> StepMaxReached
		Max  	+
				|	 -> StepMinReached
		Min		+
				|	-> StepLowReached
		Low		+
				|	-> StepEmptyReached

	*/
protected:
	void EnterMinReached() override;
	void OnMinReached() override;

public:
	CIfoControl(int32_t id) : CBaseClass(id)
	{}

	~CIfoControl(void) override = default;
};

