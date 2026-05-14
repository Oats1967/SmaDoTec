//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Declaration of module UserClassMap
///
/// @file   UserClassMap.h
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
#include "BASE/include/XMLStringEnumMap.h"
#include "BASE/Utils/public/UserClass.h"

namespace base
{
    using TUserClassMap = CXMLStringEnumMap< utils::eUserClass >;
    class CUserClassMap : public TUserClassMap
    {
    public:
        CUserClassMap() : TUserClassMap(
            {
                { utils::eUserClass::eCLASSSUPERVISOR, "SUPERVISOR" },
                { utils::eUserClass::eCLASSADMINISTRATOR, "ADMINISTRATOR" },
                { utils::eUserClass::eCLASSSTANDARDUSER, "STANDARD" },
                { utils::eUserClass::eCLASSUSER, "USER" }
            })
        {
            assert(_S32(m_Map.size()) == _S32(utils::eUserClass::eCLASSMAX));
        }
    };
};

