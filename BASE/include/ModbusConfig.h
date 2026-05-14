//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Implementation of module ModbusConfig
///
/// @file   ModbusConfig.cpp
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
class CModbusConfig
{
    std::string m_LocalIP;
    uint32_t m_Port;
    uint32_t m_SlaveID;
    uint32_t m_Timeout;

public:
    CModbusConfig() : m_LocalIP("")
        , m_Port(0)
        , m_SlaveID(0)
        , m_Timeout(0)
    {}
    ~CModbusConfig() = default;

    SETGET(const std::string&, LocalIP);
    SETGET(uint32_t, Port);
    SETGET(uint32_t, SlaveID);
    SETGET(uint32_t, Timeout);
};

};

