//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Declaration of module LogoInputSignalMap
///
/// @file   LogoInputSignalMap.h
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
#include "BASE/include/LogoInputSignal.h"

namespace base
{
    using TLogoInputSignalMap = CXMLStringEnumMap< eLogoInputSignal >;
    class CLogoInputSignalMap : public TLogoInputSignalMap
    {
    public:
        CLogoInputSignalMap() : TLogoInputSignalMap(
            {
                { eLogoInputSignal::eInputSignalNone, "NotUsed"},
                { eLogoInputSignal::eInputSignalPulseCounter, "PulseCounter"},
                { eLogoInputSignal::eInputSignalAnalogIO, "AnalogInput"}
            })
        {
            assert(_S32(m_Map.size()) == _S32(eLogoInputSignal::eMax));
        }
    };
};

