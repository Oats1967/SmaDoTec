//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Declaration of module AlarmCategoryMap
///
/// @file   AlarmCategoryMap.h
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

#include <cassert>
#include "XMLStringEnumMap.h"
#include "BASE/include/AlarmTypes.h"



namespace base
{
    using TAlarmCategoryMap = CXMLStringEnumMap<eAlarmCategory>;
    class CAlarmCategoryMap : public TAlarmCategoryMap
    {
    public:
        CAlarmCategoryMap() : TAlarmCategoryMap(
            {
                { eAlarmCategory::categoryAM, "AM" },
                { eAlarmCategory::categoryS,  "S" },
                { eAlarmCategory::categoryFL, "FL" },
                { eAlarmCategory::categorySD, "SD" },
                { eAlarmCategory::categoryRA, "RA" }
            })
        {
            assert(_S32(m_Map.size()) == _S32(eAlarmCategory::categoryMax));
        }
    };
};