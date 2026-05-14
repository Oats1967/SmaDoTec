//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Declaration of module RemoteServerConfig
///
/// @file   RemoteServerConfig.h
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

#include <array>
#include <utility>
#include <string>
#include "BASE/types.h"
#include "BASE/include/RemoteConfig.h"

namespace base
{
    class CRemoteServerConfig
    {
    public:
        std::string m_DevicePath;
        std::string m_TablePath;
        RemoteServerProtocol m_Protocol;

        CRemoteServerConfig() : m_Protocol(RemoteServerProtocol::REMOTE_PROTOCOL_NONE)
        {}
        ~CRemoteServerConfig() = default;
    };
};


