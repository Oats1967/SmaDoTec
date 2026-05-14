//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Implementation of module SimS7.c
///
/// @file   SimS7.c
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
#include "BASE/Base.h"


namespace base
{
class CDeviceConfig
{
    std::string m_RemoteIP;
    uint32_t m_Port;
    uint32_t m_Rack;
    uint32_t m_Slot;
    uint32_t m_Netlink;
    uint32_t m_Timeout;

public:
    CDeviceConfig() : m_RemoteIP("")
        , m_Port(0)
        , m_Rack(0)
        , m_Slot(0)
        , m_Netlink(0)
        , m_Timeout(0)
    {}
    ~CDeviceConfig() = default;

    SETGET(const std::string&, RemoteIP);
    SETGET(uint32_t, Port);
    SETGET(uint32_t, Rack);
    SETGET(uint32_t, Slot);
    SETGET(uint32_t, Netlink);
    SETGET(uint32_t, Timeout);
};

};

