//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Declaration of module LanguageMap
///
/// @file   LanguageMap.h
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
#include "BASE/include/Language.h"



namespace base
{
    using TLanguageMap = CXMLStringEnumMap< eLanguage >;
    class CLanguageMap : public TLanguageMap
    {
        public:
            CLanguageMap() :CXMLStringEnumMap(
                {
                   { eLanguage::eGERMAN, "German" } ,
                   { eLanguage::eENGLISH, "English" } ,
                   { eLanguage::eFRENCH, "French" } ,
                   { eLanguage::eITALIAN, "Italian" } ,
                   { eLanguage::eDUTCH, "Dutch" } ,
                   { eLanguage::eTURKISH, "Turkish" } ,
                   { eLanguage::ePOLISH,    "Polish" },
                   { eLanguage::eSPANISH,  "Spanish" },
                   { eLanguage::eRUSSIAN,  "Russian" },
                   { eLanguage::eCHINESE,  "Chinese" },
                   { eLanguage::eUKRAINIAN,  "Ukrainian" },
                   { eLanguage::eHUNGARIAN,  "Hungarian" },
                   { eLanguage::eGREEK,  "Greek" },
                })
            {
                assert(_S32(m_Map.size()) == _S32(eLanguage::eMAXLANGUAGE));
            }
    };
};