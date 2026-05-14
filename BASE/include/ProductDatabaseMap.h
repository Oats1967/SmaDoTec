//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Declaration of module ProductDatabaseMap
///
/// @file   ProductDatabaseMap.h
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

#include <string>
#include "StringEnumMap.h"


namespace base
{
    using CProductDatabaseMap = CStringEnumMap< uint64_t, std::string >;
};