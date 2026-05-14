//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Declaration of module UserClass
///
/// @file   UserClass.h
///
///
/// @coypright(c)  Ing.büro Hafer
///                Branderweg 8A
///                D-91058 Erlangen
///
/// @author        Detlef Hafer
///
//------------------------------------------------------------------------------------
#pragma once

#include <array>
#include "BASE/types.h"

namespace base
{
namespace utils
{
	enum class eUserClass : uint32_t
	{
		eCLASSSUPERVISOR = 0,
		eCLASSADMINISTRATOR,
		eCLASSSTANDARDUSER,
		eCLASSUSER,
		eCLASSMAX
	};
};
};





