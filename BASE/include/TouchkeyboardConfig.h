//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Declaration of module TouchkeyboardConfig
///
/// @file   TouchkeyboardConfig.h
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
#include "BASE/include/Point.h"
#include "BASE/include/Size.h"

namespace base
{
    class CTouchkeyboardConfig
    {
    public:
        INTSIZE m_NumericSize;;
        INTSIZE m_AlphaSize;;
        INTPOINT m_AlphaPos;
        INTPOINT m_NumericPos;
        std::string szKeyboardName;

    public:
        CTouchkeyboardConfig() :   m_NumericSize{  }
                                 , m_AlphaSize  {  }
                                 , m_AlphaPos   {  }
                                 , m_NumericPos {  }
        {}
    };

};

