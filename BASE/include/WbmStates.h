//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Declaration of module WbmStates
///
/// @file   WbmStates.h
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


enum class eWbmState : uint32_t 
{ 
    WBFMINBELTLOAD_NOT_REACHED = 0,
    WBFMINBELTLOAD_REACHED,
    WBFMINBELTLOAD_INVALID = 0xFFFFFFFF
};



};

