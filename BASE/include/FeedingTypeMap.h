//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Declaration of module FeedingTypeMap
///
/// @file   FeedingTypeMap.h
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
#include "BASE/include/FeedingType.h"

namespace base
{
    using TFeedingTypeMap = CXMLStringEnumMap< eFeedingType >;
    class CFeedingTypeMap : public TFeedingTypeMap
    {
    public:
        CFeedingTypeMap() :TFeedingTypeMap(
            {
                { eFeedingType::FF_NONE, "-" },
                { eFeedingType::FF_MF, "MF" },
                { eFeedingType::FF_SF1, "SF1" },
                { eFeedingType::FF_SF2, "SF2" },
            })
        {
            assert(_S32(m_Map.size()) == _S32(eFeedingType::FF_MAX));
        }
    };
};

