//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Declaration of module EasyControlConfig
///
/// @file   EasyControlConfig.h
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

    class CEasyControlConfig
    {
    public:
        std::string m_RecorderConfigFile;
        std::string m_RecorderDescriptionFile;
        std::string m_RecipePath;
        std::string m_AlarmLogPath;
        std::string m_DoseServerFile;
        std::string m_SystemAlarmFile;
        std::string m_LayoutConfigFile;
        std::string m_UserConfigFile;
        std::string m_TouckKeyboardConfigFile;
        std::string m_DiagramConfigFile;
        std::string m_ProductDatabaseFile;
        eLanguage   m_Language;


    public:
        CEasyControlConfig() : m_Language(eLanguage::eGERMAN)
        {}

        virtual ~CEasyControlConfig() = default;
        CEasyControlConfig(const CEasyControlConfig&) = default;
        CEasyControlConfig& operator = (const CEasyControlConfig&) = default;

        BOOL operator == (const CEasyControlConfig& cfg) const
        {
            return  ( m_RecorderConfigFile == cfg.m_RecorderConfigFile) &&
                    ( m_RecorderDescriptionFile == cfg.m_RecorderDescriptionFile) &&
                    ( m_RecipePath == cfg.m_RecipePath ) &&
                    ( m_AlarmLogPath == cfg.m_AlarmLogPath ) && 
                    ( m_DoseServerFile == cfg.m_DoseServerFile) &&
                    ( m_SystemAlarmFile == cfg.m_SystemAlarmFile) &&
                    // ( m_SystemConfigFile == cfg.m_SystemConfigFile) &&
                    ( m_LayoutConfigFile == cfg.m_LayoutConfigFile) &&
                    ( m_UserConfigFile == cfg.m_UserConfigFile) &&
                    ( m_TouckKeyboardConfigFile == cfg.m_TouckKeyboardConfigFile) &&
                    ( m_DiagramConfigFile == cfg.m_DiagramConfigFile) &&
                    ( m_ProductDatabaseFile == cfg.m_ProductDatabaseFile) &&
                    ( m_Language == cfg.m_Language);
        }

        BOOL operator != (const CEasyControlConfig& cfg) const
        {
            return ! operator == (cfg);
        }

    };

};

