//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Declaration of module MemOp
///
/// @file   MemOp.h
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

#include <cstring>

namespace base
{
namespace utils
{
inline void baseZeroMemory(void *buffer, size_t length)
{
    std::memset(buffer, 0, length);
}

template <typename T>
inline void baseZeroMemory(T& buffer)
{
    std::memset(&buffer, 0, sizeof(buffer));
}


};
};




