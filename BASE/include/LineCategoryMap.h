//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Declaration of module LineCategoryMap
///
/// @file   LineCategoryMap.h
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
#include "BASE/include/LineCategory.h"

namespace base
{
using TLineCategoryMap = CXMLStringEnumMap< LineCategory >;
class CLineCategoryMap : public TLineCategoryMap
{
public:
    CLineCategoryMap() : TLineCategoryMap(
        { 
            { LineCategory::eLine, std::string("Line") },
            { LineCategory::eArea, std::string("Area") }
        })
    {}
};

};

