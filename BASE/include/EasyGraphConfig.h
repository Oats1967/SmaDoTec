//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Declaration of module EasyGraphConfig
///
/// @file   EasyGraphConfig.h
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
#include "BASE/include/Language.h"

namespace base
{

    class CEasyGraphConfig
    {
    public:
        std::string m_LineConfigFile;
        std::string m_ProductDatabaseFile;
        std::string m_SettingsFile;
        std::string m_RecorderDescriptionFile;
        std::string m_TouckKeyboardConfigFile;
        eLanguage   m_Language;

    public:
        CEasyGraphConfig() : m_Language(eLanguage::eGERMAN)
        {}

        virtual ~CEasyGraphConfig() = default;
        CEasyGraphConfig(const CEasyGraphConfig&) = default;
        CEasyGraphConfig& operator = (const CEasyGraphConfig&) = default;

        BOOL operator == (const CEasyGraphConfig& cfg) const
        {
            return  ( m_LineConfigFile == cfg.m_LineConfigFile ) &&
                    ( m_ProductDatabaseFile == cfg.m_ProductDatabaseFile ) &&
                    ( m_SettingsFile == cfg.m_SettingsFile ) &&
                    ( m_RecorderDescriptionFile == cfg.m_RecorderDescriptionFile) &&
                    ( m_TouckKeyboardConfigFile == cfg.m_TouckKeyboardConfigFile) &&
                    ( m_Language == cfg.m_Language);
        }

        BOOL operator != (const CEasyGraphConfig& cfg) const
        {
            return ! operator == (cfg);
        }

    };

};

