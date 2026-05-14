//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Declaration of module MassflowSelectMap
///
/// @file   MassflowSelectMap.h
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

#include <string>
#include "BASE/types.h"
#include "BASE/include/XMLStringEnumMap.h"
#include "BASE/include/MassflowSelect.h"

namespace base
{
using TMassflowSelectMap = CXMLStringEnumMap< eMassflowSelect >;
class CMassflowSelectMap : public TMassflowSelectMap
{
public:
    CMassflowSelectMap() : TMassflowSelectMap(
        { { eMassflowSelect::eVIEWMASSFLOW, std::string("Massflow") },
          { eMassflowSelect::eVIEWSETPOINT, std::string("Setpoint") },
          { eMassflowSelect::eVIEWDRIVECOMMAND, std::string("Drivecommand") },
          { eMassflowSelect::eVIEWROTSPEED, std::string("RotSpeed") },
          { eMassflowSelect::eVIEWWEIGHT, std::string("Netweight") },
          { eMassflowSelect::eVIEWTOTALIZER, std::string("Totalizer") },
          { eMassflowSelect::eVIEWDOSEPERFORMANCE, std::string("Doseperformance") }
        })
    {
        assert(_S32(m_Map.size()) == cMassflowSelectMax);
    }
};


};

