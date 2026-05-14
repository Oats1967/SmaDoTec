//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Declaration of module DiagramSettingsConfig
///
/// @file   DiagramSettingsConfig.h
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
#include <array>
#include "BASE/Include/MassflowSelect.h"

namespace base
{
    struct CDiagramSettingsConfig
    {
        using TrendPair = std::pair<float32_t, float32_t>;
        std::array<TrendPair, cViewSelectMax> m_Attributes;

        const float32_t getfrom(base::eMassflowSelect select) const
        { return m_Attributes[_S32(select)].first;   }

        const float32_t getto(base::eMassflowSelect select) const
        {  return m_Attributes[_S32(select)].second;    }

        void setfrom(base::eMassflowSelect select, const float32_t value)
        {  m_Attributes[_S32(select)].first = value;   }

        void  setto(base::eMassflowSelect select, const float32_t value)
        {  m_Attributes[_S32(select)].second = value;    }
    };
};


