//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Declaration of module XmlTypes
///
/// @file   XmlTypes.h
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

#include <vector>
#include "BASE/types.h"
#include "BASE/Utils/public/StringUtil.h"


namespace base
{
namespace xml
{
    using xmlentry = std::vector<std::pair<std::string, std::string>>;
};

#ifndef STATIC_KEY
#define STATIC_KEY(_a, _b) static const char* _a = _b;
#endif

};




