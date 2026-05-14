//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Declaration of module MassflowAttributes.h
///
/// @file   MassflowAttributes.h
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

#include "types.h"
#include "BASE/Include/MassflowSelect.h"

namespace base
{
namespace utils
{

template<typename T, uint32_t N>
struct CMassflowAttributes
{
    using ViewItem = std::array <T, GLOBALDOSEMAXCOUNT>;
    std::array< T, N> m_Attribute;

    const auto& Get(const eMassflowSelect select, const uint32_t index) const
    {
        return m_Attribute[index][_S32(select)];
    }

    void Set(const eMassflowSelect select, const uint32_t index, const auto value)
    {
        m_Attribute[index][_S32(select)] = value;
    }

    CMassflowAttributes() : m_Attribute{ 0 }
    {
        ViewItem a1;
        a1.fill(0);
        m_Attribute.fill(a1);
    }
};


};
};



    

