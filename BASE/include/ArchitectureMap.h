//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Declaration of module ArchitectureMap
///
/// @file   ArchitectureMap.h
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

#include "XMLStringEnumMap.h"
#include "BASE/include/ArchitectureTypes.h"



namespace base
{
    using  TArchitectureMap = CXMLStringEnumMap<eArchitecture>;
    class CArchitectureMap : public TArchitectureMap
    {
    public:
        CArchitectureMap() : TArchitectureMap(
            {
               { eArchitecture::eMaster, "Master" } ,
               { eArchitecture::eSlave,  "Slave" },
               { eArchitecture::eMixed,  "Mixed" }
            })
        {
            assert(_S32(m_Map.size()) == _S32(eArchitecture::eMax));
        }
    };
};