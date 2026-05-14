//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Declaration of module LCTypeDetectionMap
///
/// @file   LCTypeDetectionMap.h
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
#include "BASE/include/LCTypeDetection.h"

namespace base
{
    using TLCTypeDetectionMap = CXMLStringEnumMap< eLcTypeDetection >;
    class CLCTypeDetectionMap : public TLCTypeDetectionMap
    {
    public:
        CLCTypeDetectionMap() : TLCTypeDetectionMap(
            {
                { eLcTypeDetection::LC_DET_NONE, "-" },
                { eLcTypeDetection::LC_DET_AUTO, "AUTO" },
                { eLcTypeDetection::LC_DET_PESA, "PESA" },
                { eLcTypeDetection::LC_DET_SOEMER, "SOEMER" },
                { eLcTypeDetection::LC_DET_LAUMAS, "LAUMAS" },
                { eLcTypeDetection::LC_DET_KTRON, "KTRON" },
            })
        {
            assert(_S32(m_Map.size()) == cMaxLCTypDetection);
        }
    };

};

