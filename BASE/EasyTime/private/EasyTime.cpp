//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Declaration of module Tracer.h
///
/// @file   EasyTime.h
///
///
/// @coypright Ing.büro Hafer
///            Branderweg 8A
///            D-91058 Erlangen
///
/// @author    Detlef Hafer
///
//------------------------------------------------------------------------------------
#include "BASE/types.h"
#include <windows.h>

namespace base
{
namespace time
{
    uint64_t GetTimeU64()
	{
		return ::GetTickCount64();
	}
};
};
