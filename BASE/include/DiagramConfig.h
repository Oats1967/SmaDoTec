//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Declaration of module DiagramConfig
///
/// @file   DiagramConfig.h
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

#include <array>
#include <tuple>
#include <vector>
#include "BASE/types.h"
#include "BASE/include/AxisScale.h"

namespace base
{
    enum class eTimeMode : uint16_t
    {
        TG_NONE = 0,
        TG_SECONDS,
        TG_DATE,
        TG_TODAY
    };

    enum class eDiagramType : uint32_t
    {
        eUnknown = 0,
        e2DLine,
        e3DLine,
        eTable,
    };

    enum class eSelectionType : uint32_t
    {
        eSelectionUnknown = 0,
        eSelectionSingle,
        eSelectionMultiple
    };

    using tViewMultipleSelect   = std::vector< base::eMassflowSelect>;
    
    class CDiagramConfig
    {
        int32_t          m_ID;
        eTimeMode        m_TimeInterval;
        MassflowBitfield m_DiagramSelect;
        eDiagramType     m_DiagramType;
        CAxisScale       m_AxisScale;

        public:
            CDiagramConfig() : m_ID{ -1 }
                , m_TimeInterval{ eTimeMode::TG_SECONDS }
                , m_DiagramSelect{  }
                , m_DiagramType { eDiagramType ::eUnknown }
                , m_AxisScale{}
            {}

            tViewMultipleSelect ExtractSelection() const;
            base::eMassflowSelect GetSingleSelection() const;
            void SetSingleSelection(const base::eMassflowSelect iSelect);
            void ClearSelection();
            uint32_t GetSelectionCount() const;
            base::eSelectionType GetSelectionType() const;

            SETGET(int32_t, ID)
            SETGET(eTimeMode, TimeInterval)
            SETGET(const MassflowBitfield&, DiagramSelect)
            SETGET(eDiagramType, DiagramType)
            CAxisScale& GetAxisScale()
            {  return m_AxisScale;   }


            BOOL operator == (const CDiagramConfig& cfg) const
            {
#if 0
                return  ( m_ID == cfg.m_ID) &&
                        (m_TimeInterval == cfg.m_TimeInterval) &&
                        (m_DiagramSelect.allflags == cfg.m_DiagramSelect.allflags) &&
                        (m_DiagramType == cfg.m_DiagramType) &&
#endif
                return  (m_AxisScale == cfg.m_AxisScale);
            }

            BOOL operator != (const CDiagramConfig& cfg) const
            {  return !(*this == cfg);     }

    };

};


