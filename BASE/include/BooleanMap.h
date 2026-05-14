//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Declaration of module BooleanMap
///
/// @file   BooleanMap.h
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

namespace base
{
using CBooleanMap = CXMLStringEnumMap< BOOL >;

class CBoolMap : public CBooleanMap
{
public:
    CBoolMap() : CBooleanMap(
        { 
            { BOOL(TRUE), std::string("TRUE") }, 
            { BOOL(FALSE), std::string("FALSE") } 
        })
    {}
};


class COnOffMap : public CBooleanMap
{
public:
    COnOffMap() : CBooleanMap( 
        { 
            { BOOL(TRUE), std::string("ON") }, 
            { BOOL(FALSE), std::string("OFF") } 
        })
    {}
};


//-----------------------------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------------------
class CYesNoMap : public CBooleanMap
{
public:
    CYesNoMap() : CBooleanMap(
        { 
            { BOOL(TRUE), std::string("YES") }, 
            { BOOL(FALSE), std::string("NO") } 
        })
    {}
};

};

