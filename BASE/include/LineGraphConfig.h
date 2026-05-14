//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Declaration of module LineGraphConfig
///
/// @file   LineGraphConfig.h
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
#include <vector>
#include "BASE/types.h"

namespace base
{
    class CLineGraphConfig
    {
    public:
        struct CfgPair
        {
            std::string m_szName;
            std::string m_szRecPath;
            std::string m_szLogPath;

            BOOL operator == (const CfgPair& rPair) const
            {
                return (m_szName == rPair.m_szName) && (m_szRecPath == rPair.m_szRecPath) && (m_szLogPath == rPair.m_szLogPath);
            }
        };
        using VectorConfig = std::vector < CfgPair >;

    public:
        VectorConfig m_field;


    public:
        CLineGraphConfig()
        {}

        virtual ~CLineGraphConfig() = default;
        CLineGraphConfig(const CLineGraphConfig&) = default;
        CLineGraphConfig& operator = (const CLineGraphConfig&) = default;

        BOOL operator == (const CLineGraphConfig& cfg) const
        {
            return  (m_field == cfg.m_field);
        }

        BOOL operator != (const CLineGraphConfig& cfg) const
        {
            return ! operator == (cfg);
        }

        const CfgPair* GetLineListItem(const std::string& szName)
        {
            auto ptr = std::find_if(m_field.cbegin(), m_field.cend(), [&szName](const auto& rItem) { return rItem.m_szName == szName; });
            return (ptr == m_field.cend()) ? nullptr : &(*ptr);
        }

    };

};

