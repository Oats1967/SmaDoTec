//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Declaration of module RecorderConfig
///
/// @file   RecorderConfig.h
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

namespace base
{
    class CRecorderConfig
    {
    public:
        std::string m_RecorderPath;
        uint32_t    m_RecorderSampeTime;
        BOOL        m_RecorderActive;

    public:
        CRecorderConfig() : m_RecorderSampeTime(0), m_RecorderActive(FALSE)
        {}

        virtual ~CRecorderConfig() = default;
        CRecorderConfig (const CRecorderConfig&) = default;
        CRecorderConfig& operator = (const CRecorderConfig&) = default;

        BOOL operator == (const CRecorderConfig& cfg) const
        {
            return  (m_RecorderPath == cfg.m_RecorderPath) &&
                    (m_RecorderSampeTime == cfg.m_RecorderSampeTime) &&
                    (m_RecorderActive == cfg.m_RecorderActive);
        }

        BOOL operator != (const CRecorderConfig& cfg) const
        {
            return ! operator == (cfg);
        }
    };

};

