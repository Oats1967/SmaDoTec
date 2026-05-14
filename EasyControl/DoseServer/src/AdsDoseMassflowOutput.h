//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Declaration of module AdsDoseMassflowOutput
///
/// @file   AdsDoseMassflowOutput.h
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

namespace AdsSensor
{
	class CAdsDoseMassflowOutput : public CAdsDoseAnalogOutput
	{
		using CBaseClass = CAdsDoseAnalogOutput;

	public:
		CAdsDoseMassflowOutput(const int32_t index);

		BOOL SetMassflow(const float32_t); // input in kg/h
	};

};