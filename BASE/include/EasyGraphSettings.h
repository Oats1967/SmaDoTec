//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Declaration of module EasyGraphSettings
///
/// @file   EasyGraphSettings.h
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
#include "BASE/types.h"
#include "BASE/include/LineCategory.h"
#include "BASE/include/MassflowSelect.h"
#include "Date2Show.h"

namespace base
{
    struct LineAttribute
    {
        int32_t m_LineWidth = 1;
        BOOL        m_Visible   = TRUE;
        LineCategory m_Category  = LineCategory::eLine;
        uint32_t m_Color    = 0;

        BOOL operator == (const LineAttribute& rA) const
        {
            return  (m_LineWidth == rA.m_LineWidth) && 
                    (m_Visible == rA.m_Visible) &&
                    (m_Category == rA.m_Category) &&
                    (m_Color == rA.m_Color);
        }
        BOOL operator != (const LineAttribute& rA) const
        {
            return ! operator ==(rA);
        }
    };

    class CEasyGraphSettings
    {
    public:
        DateToShow m_ShowTime;
        int32_t m_ActiveLine;
        int32_t m_ActiveFeeder;
        uint32_t m_RealMonitoringRefreshTime;
        uint32_t m_RealMonitoringHistoryMinutes;
        std::array< LineAttribute, cMassflowSelectMax> m_Attribues;
    public:
        CEasyGraphSettings() : m_ShowTime{ }
            , m_ActiveLine{ 0 }
            , m_ActiveFeeder{ 0 }
            , m_RealMonitoringRefreshTime{ 5U }
            , m_RealMonitoringHistoryMinutes{ 30U }
            ,  m_Attribues{ 0 }
        {}

        virtual ~CEasyGraphSettings() = default;
        CEasyGraphSettings(const CEasyGraphSettings&) = default;
        CEasyGraphSettings& operator = (const CEasyGraphSettings&) = default;

        BOOL operator == (const CEasyGraphSettings& cfg) const
        {
            return  (m_ShowTime == cfg.m_ShowTime) &&
                    (m_ActiveLine == cfg.m_ActiveFeeder) &&
                    (m_ActiveFeeder == cfg.m_ActiveLine) &&
                    (m_RealMonitoringRefreshTime == cfg.m_RealMonitoringRefreshTime) &&
                    (m_RealMonitoringHistoryMinutes == cfg.m_RealMonitoringHistoryMinutes) &&
                    (m_Attribues == cfg.m_Attribues);
        }

        BOOL operator != (const CEasyGraphSettings& cfg) const
        {
            return ! operator == (cfg);
        }

    };

};

