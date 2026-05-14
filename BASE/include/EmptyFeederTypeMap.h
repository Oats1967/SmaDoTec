//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Declaration of module EmptyFeederTypeMap
///
/// @file   EmptyFeederTypeMap.h
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
#include "BASE/include/StringEnumMap.h"
#include "BASE/include/EmptyFeederType.h"

namespace base
{
    using TEmptyFeederTypeMap = CXMLStringEnumMap< eEmptyFeederType >;
    class CEmptyFeederTypeMap : public TEmptyFeederTypeMap
    {
    public:
        CEmptyFeederTypeMap() : TEmptyFeederTypeMap(
            {
                { eEmptyFeederType::eNone, "None" },
                { eEmptyFeederType::eService, "Service" }
            })
        {
            assert(_S32(m_Map.size()) == _S32(eEmptyFeederType::eMax));
        }
    };
};

