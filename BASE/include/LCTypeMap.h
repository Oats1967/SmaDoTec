//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Declaration of module LCTypeMap
///
/// @file   LCTypeMap.h
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

#include <cassert>
#include "BASE/types.h"
#include "BASE/include/XMLStringEnumMap.h"
#include "BASE/include/LCType.h"

namespace base
{
    using TLCTypeMap = CXMLStringEnumMap< eLcType >;
    class CLCTypeMap : public TLCTypeMap
    {
    public:
        CLCTypeMap() : TLCTypeMap(
            {
                { eLcType::LC_EMPTY, "-" },
                { eLcType::LC_PESA, "PESA" },
                { eLcType::LC_SOEMER, "SOEMER" },
                { eLcType::LC_LAUMAS, "LAUMAS" },
                { eLcType::LC_KTRON, "KTRON" },
            })
        {
            assert(_S32(m_Map.size()) == cMaxLCTyp);
        }
    };

};

