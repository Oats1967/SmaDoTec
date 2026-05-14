//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Declaration of module LogoLocking
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
	enum class eLogoLocking : uint32_t
	{
		eLockingNone = 0,
		eLockingInternal,
		eLockingExternal,
		eLockMax
	};
};



