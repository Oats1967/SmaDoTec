//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Declaration of module LogoLockingMap
///
/// @file   LogoLockingMap.h
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
#include "BASE/include/LogoLocking.h"

namespace base
{
    using TLogoLockingMap = CXMLStringEnumMap< eLogoLocking >;
    class CLogoLockingMap : public TLogoLockingMap
    {
        public:
            CLogoLockingMap() :CXMLStringEnumMap(
                {
                    { eLogoLocking::eLockingNone, "NotUsed"},
                    { eLogoLocking::eLockingInternal, "internal"},
                    { eLogoLocking::eLockingExternal, "external"}
                })
            {
                assert(_S32(m_Map.size()) == _S32(eLogoLocking::eLockMax));
            }

    };
};

