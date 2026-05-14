//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Declaration of module LCSubType
///
/// @file   LCSubType.h
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
	enum class eLcSubType : int32_t
	{
		eUnknown = 0,
		eLDU68P1,
		eLDU69P1,
		ePESA,
		eLAUMAS,
		eKTRON,
		eMAXCOUNT,
	};
};



