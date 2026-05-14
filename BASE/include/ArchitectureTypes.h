//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Declaration of module Architecture
///
/// @file   Architecture.h
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
    enum class eArchitecture : uint16_t
    {
        eMaster,
        eSlave,
        eMixed,
        eMax,
    };
};

