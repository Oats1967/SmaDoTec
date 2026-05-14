//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Declaration of module Timer.h
///
/// @file   Timer.h
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

#include <cstdio>
#include <string>
#include <cassert>

namespace base
{
namespace utils
{

template< typename... Args >
std::string string_sprintf(const char* format, Args... args) 
{
    int length = std::snprintf(nullptr, 0, format, args...);
    assert(length >= 0);

    char* buf = new char[length + 1];
    std::snprintf(buf, length + 1, format, args...);

    std::string str(buf);
    delete[] buf;
    return str;
}

};
};


