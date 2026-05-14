//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Declaration of module DoseTypeMap
///
/// @file   DoseTypeMap.h
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

#include "BASE/types.h"
#include "BASE/include/XMLStringEnumMap.h"
#include "BASE/include/DoseType.h"

namespace base
{
    using  TDoseTypeMap = CXMLStringEnumMap< eDoseType >;
    class CDoseTypeMap : public TDoseTypeMap
    {
        public:
            CDoseTypeMap() :TDoseTypeMap(
                {
                    { eDoseType::DOSETYPE_NONE, "-" },
                    { eDoseType::DOSETYPE_LWS, "LWS" },
                    { eDoseType::DOSETYPE_LWV, "LWV" },
                    { eDoseType::DOSETYPE_LWP, "LWP" },
                    { eDoseType::DOSETYPE_LWB, "LWB" },
                    { eDoseType::DOSETYPE_ICB, "ICB" },
                    { eDoseType::DOSETYPE_GWB, "GWB" },
                    { eDoseType::DOSETYPE_WBF, "WBF" },
                    { eDoseType::DOSETYPE_PID, "PID" },
                    { eDoseType::DOSETYPE_ICP, "ICP" },
                    { eDoseType::DOSETYPE_WBM, "WBM" },
                    { eDoseType::DOSETYPE_VOL, "VOL" },
                    { eDoseType::DOSETYPE_LCL, "LCL" },
                    { eDoseType::DOSETYPE_IFL, "IFL"  },
                    { eDoseType::DOSETYPE_IFSO, "IFO" },
                    { eDoseType::DOSETYPE_IFSU, "IFU" } 
                })
            {
                assert(m_Map.size() == _S32(eDoseType::DOSETYPE_MAX));
            }
    };
};
