//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Declaration of module Endian
///
/// @file   Endian.h
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

#include "BASE/types.h"

namespace base
{

template<typename T>
class CEndianBase
{
#if _MSC_VER
#pragma warning ( push ) 
#pragma warning ( disable : 4701 )
#pragma warning ( disable : 4789 )
#endif
    BOOL m_bigendian;
 private:

    static T Swap(const T& b)
    {
        T n;

        switch (sizeof(T))
        {
        case 8: //64-bit
            ((uint8_t*)&n)[0] = ((uint8_t*)&b)[7];
            ((uint8_t*)&n)[1] = ((uint8_t*)&b)[6];
            ((uint8_t*)&n)[2] = ((uint8_t*)&b)[5];
            ((uint8_t*)&n)[3] = ((uint8_t*)&b)[4];
            ((uint8_t*)&n)[4] = ((uint8_t*)&b)[3];
            ((uint8_t*)&n)[5] = ((uint8_t*)&b)[2];
            ((uint8_t*)&n)[6] = ((uint8_t*)&b)[1];
            ((uint8_t*)&n)[7] = ((uint8_t*)&b)[0];
            break;
        case 4: //32-bit
            ((uint8_t*)&n)[0] = ((uint8_t*)&b)[3];
            ((uint8_t*)&n)[1] = ((uint8_t*)&b)[2];
            ((uint8_t*)&n)[2] = ((uint8_t*)&b)[1];
            ((uint8_t*)&n)[3] = ((uint8_t*)&b)[0];
            break;
        case 2: //16-bit
            ((uint8_t*)&n)[0] = ((uint8_t*)&b)[1];
            ((uint8_t*)&n)[1] = ((uint8_t*)&b)[0];
            break;
        case 1: //8-bit
            ((uint8_t*)&n)[0] = ((uint8_t*)&b)[0];
            break;
        default:
            assert(0);  //Endian swap is only defined for 2, 4, and 8-byte types
            break;
        }
        return n;
    }

public:
    CEndianBase(const BOOL bigendian = FALSE) : m_bigendian(bigendian)
    {}

    T Get(const T& value)
    {
        T result = value;
        if (m_bigendian)
        {
            result = Swap(value);
        }
        return result;
     }
#if _MSC_VER
#pragma warning ( pop )
#endif

};
};