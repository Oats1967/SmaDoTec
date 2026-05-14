//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Declaration of module LogoInputSignal
///
/// @file   LogoLocking.h
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

#include "BASE/types.h"

namespace base
{
	enum class eLogoInputSignal : uint32_t
	{
		eInputSignalNone = 0,
		eInputSignalPulseCounter,
		eInputSignalAnalogIO,
		eMax
	};
};



