//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Declaration of module DoseServerConfig
///
/// @file   DoseServerConfig.h
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

#include <string>
#include "BASE/types.h"


namespace base
{
    class CDoseServerConfig
    {
    public:
        std::string m_path2SystemConfig;
        std::string m_path2RemoteServer;
        std::string m_path2DataFile;
        std::string m_path2LogoFile;
        std::string m_path2IOConfig;
        std::string m_path2LineConfig;

        CDoseServerConfig() = default;
        ~CDoseServerConfig() = default;
    };
};

