//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Declaration of module LCSubTypeMap
///
/// @file   LCSubTypeMap.h
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
#include "BASE/include/LCSubType.h"

namespace base
{
    using TLCSubTypeMap = CXMLStringEnumMap< eLcSubType >;
    class CLCSubTypeMap : public TLCSubTypeMap
    {
    public:
        CLCSubTypeMap() : TLCSubTypeMap(
            {
                { eLcSubType::eUnknown, "-" },
                { eLcSubType::eLDU68P1, "LDU 68.1" },
                { eLcSubType::eLDU69P1, "LDU 69.1" },
                { eLcSubType::ePESA, "PESA" },
                { eLcSubType::eLAUMAS, "LAUMAS" },
                { eLcSubType::eKTRON, "KTRON" }
            })
        {
            assert(_S32(m_Map.size()) == _S32(eLcSubType::eMAXCOUNT));
        }
    };
};

